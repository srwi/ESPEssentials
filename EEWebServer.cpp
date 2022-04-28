#include "EEWebServer.h"

#include "EESerialOut.h"

#if defined(ESP32)
	#include <SPIFFS.h>
	#include <Update.h>
	#include <WiFi.h>
#elif defined(ESP8266)
	#include <ESP8266WiFi.h>
	#include <FS.h>
#endif

#include <WiFiUdp.h>

EEWebServerClass::EEWebServerClass(int port = 80)
{
	_handle_file_create = [&]() { handleFileCreate(); };
	_handle_file_delete = [&]() { handleFileDelete(); };
	_handle_file_list = [&]() { handleFileList(); };
	_handle_file_upload = [&]() { handleFileUpload(); };
	_handle_update = [&]() { handleUpdate(); };
}

void EEWebServerClass::init()
{
	if (!SPIFFS.begin())
	{
		PRINTLN("[Storage] Couldn't mount file system.");
		return;
	}

	on("/edit", HTTP_GET, [&]()
	{
		if(!handleFileRead("/edit.htm"))
		{
			String content	=	"<form method='post' enctype='multipart/form-data'>";
			content			+=		"<label>Upload a file (e.g. <i>edit.htm.gz</i>) to SPIFFS:";
			content			+=			"<input name='datei' type='file'>";
			content			+=		"</label>";
			content			+=		"<button>Upload!</button>";
			content			+=	"</form>";
			send(200, "text/html", content);
		}
	});
	on("/edit", HTTP_PUT, _handle_file_create);
	on("/edit", HTTP_DELETE, _handle_file_delete);
	on("/edit", HTTP_POST, [&]() { send(200, "text/plain", ""); }, _handle_file_upload);
	on("/list", HTTP_GET, _handle_file_list);
	on("/update", HTTP_GET, [&]()
	{
		sendHeader("Connection", "close");
		sendHeader("Access-Control-Allow-Origin", "*");
		String content = "<form method='POST' action='/handle_update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";
		send(200, "text/html", content);
	});
	on("/handle_update", HTTP_POST, [&]()
	{
		sendHeader("Connection", "close");
		sendHeader("Access-Control-Allow-Origin", "*");
		send(200, "text/plain", (Update.hasError())?"FAIL":"OK");
		delay(200);
		ESP.restart();
	}, _handle_update);
	on("reboot", HTTP_GET, [&]()
	{
		send(200, "text/plain", "Rebooting...");
		delay(200);
		ESP.restart();
	});
	onNotFound([&]()
	{
		if(!handleFileRead(uri()))
		{
			PRINTLN("[Storage] Couldn't find file at \'" + uri() + "\'" + ".");
			send(404, "text/plain", "Oops, file not found!");
		}
	});

	begin();
}

bool EEWebServerClass::isBusy()
{
	return webserverBusy;
}

String EEWebServerClass::formatBytes(size_t bytes)
{
	if (bytes < 1024)
		return String(bytes) + "B";
	else if(bytes < (1024 * 1024))
		return String(bytes / 1024.0) + "KB";
	else if(bytes < (1024 * 1024 * 1024))
		return String(bytes / 1024.0 / 1024.0) + "MB";
	else
		return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
}

String EEWebServerClass::getContentType(String filename)
{
	if(hasArg("download")) return "application/octet-stream";
	else if(filename.endsWith(".htm")) return "text/html";
	else if(filename.endsWith(".html")) return "text/html";
	else if(filename.endsWith(".css")) return "text/css";
	else if(filename.endsWith(".js")) return "application/javascript";
	else if(filename.endsWith(".png")) return "image/png";
	else if(filename.endsWith(".gif")) return "image/gif";
	else if(filename.endsWith(".jpg")) return "image/jpeg";
	else if(filename.endsWith(".ico")) return "image/x-icon";
	else if(filename.endsWith(".xml")) return "text/xml";
	else if(filename.endsWith(".pdf")) return "application/x-pdf";
	else if(filename.endsWith(".zip")) return "application/x-zip";
	else if(filename.endsWith(".gz")) return "application/x-gzip";
	return "text/plain";
}

bool EEWebServerClass::handleFileRead(String path)
{
	PRINTLN_VERBOSE("[Storage] File read: " + path);

	webserverBusy = true;
	if(path.endsWith("/")) path += "index.htm";
	String contentType = getContentType(path);
	String pathWithGz = path + ".gz";

	if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path))
	{
		if(SPIFFS.exists(pathWithGz))
			path += ".gz";

		File file = SPIFFS.open(path, "r");
		streamFile(file, contentType);
		file.close();

		webserverBusy = false;
		return true;
	}

	webserverBusy = false;
	return false;
}

void EEWebServerClass::handleFileUpload()
{
	if(uri() != "/edit")
		return;

	HTTPUpload& _upload = upload();

	if(_upload.status == UPLOAD_FILE_START)
	{
		String filename = _upload.filename;
		if(!filename.startsWith("/"))
			filename = "/" + filename;
		PRINTLN("[Storage] Uploading file: " + filename);

		fsUploadFile = SPIFFS.open(filename, "w");
		filename = String();
	}
	else if(_upload.status == UPLOAD_FILE_WRITE)
	{
		if(fsUploadFile)
			fsUploadFile.write(_upload.buf, _upload.currentSize);
	}
	else if(_upload.status == UPLOAD_FILE_END)
	{
		if(fsUploadFile)
			fsUploadFile.close();
		PRINTLN("[Storage] Received total: " + formatBytes(_upload.totalSize));
	}
}

void EEWebServerClass::handleFileDelete()
{
	if(args() == 0)
		return send(500, "text/plain", "BAD ARGS");

	String path = arg(0);
	PRINTLN("[Storage] Deleting file: " + path);

	if(path == "/")
		return send(500, "text/plain", "BAD PATH");
	if(!SPIFFS.exists(path))
		return send(404, "text/plain", "Oops, file not found (3)!");

	SPIFFS.remove(path);

	send(200, "text/plain", "");
	path = String();
}

void EEWebServerClass::handleFileCreate()
{
	if(args() == 0)
		return send(500, "text/plain", "BAD ARGS");

	String path = arg(0);
	PRINTLN("[Storage] Creating file: " + path);

	if(path == "/")
		return send(500, "text/plain", "BAD PATH");
	if(SPIFFS.exists(path))
		return send(500, "text/plain", "File already exists!");

	File file = SPIFFS.open(path, "w");

	if(file)
		file.close();
	else
		return send(500, "text/plain", "Oops, creating file failed!");

	send(200, "text/plain", "");
	path = String();
}

void EEWebServerClass::handleFileList()
{
	if(!hasArg("dir"))
	{
		send(500, "text/plain", "BAD ARGS");
		return;
	}

#if defined(ESP32)
	String path = arg("dir");
	File dir = SPIFFS.open(path);
	path = String();

	String output = "[";
	if (dir.isDirectory())
	{
		File file = dir.openNextFile();
		while(file)
		{
			if (output != "[") output += ',';
			bool isDir = false;
			output += "{\"type\":\"";
			output += (isDir)?"dir":"file";
			output += "\",\"name\":\"";
			output += String(file.path()).substring(1);
			output += "\"}";
			file = dir.openNextFile();
		}
	}
	output += "]";
#elif defined(ESP8266)
	String path = arg("dir");
	Dir dir = SPIFFS.openDir(path);
	path = String();

	String output = "[";
	while(dir.next())
	{
		File entry = dir.openFile("r");
		if (output != "[") output += ',';
		bool isDir = false;
		output += "{\"type\":\"";
		output += (isDir)?"dir":"file";
		output += "\",\"name\":\"";
		output += String(entry.name()).substring(1);
		output += "\"}";
		entry.close();
	}
	output += "]";
#endif

	send(200, "text/json", output);
}

void EEWebServerClass::handleUpdate()
{
	HTTPUpload& _upload = upload();

	if(_upload.status == UPLOAD_FILE_START)
	{
		Serial.setDebugOutput(true);
		PRINTF("Update: %s\n", _upload.filename.c_str());
#if defined(ESP32)
		uint32_t maxSketchSpace = UPDATE_SIZE_UNKNOWN;
#elif defined(ESP8266)
		WiFiUDP::stopAll();
		uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
#endif
		if(!Update.begin(maxSketchSpace))
		{
			Update.printError(Serial);
		}
	}
	else if(_upload.status == UPLOAD_FILE_WRITE)
	{
		if(Update.write(_upload.buf, _upload.currentSize) != _upload.currentSize)
		{
			Update.printError(Serial);
		}
	}
	else if(_upload.status == UPLOAD_FILE_END)
	{
		if(Update.end(true))
		{
			PRINTF("Update success: %u\nRebooting...\n", _upload.totalSize);
		}
		else
		{
			Update.printError(Serial);
		}
		Serial.setDebugOutput(false);
	}
	yield();
}

EEWebServerClass EEWebServer(80);
