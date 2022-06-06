#include "EEWebServer.h"

#include "EESerialOut.h"
#include "EESuspendTimerGuard.h"

#if defined(ESP32)
	#include <Update.h>
	#include <WiFi.h>
#elif defined(ESP8266)
	#include <ESP8266WiFi.h>
#endif

#if defined(USE_SPIFFS)
	#if defined(ESP32)
		#include <SPIFFS.h>
	#elif defined(ESP8266)
		#include <FS.h>
	#endif
	#define FILESYSTEM SPIFFS
#else
	#include <LittleFS.h>
	#define FILESYSTEM LittleFS
#endif

#include <WiFiUdp.h>

namespace ESPEssentials
{

EEWebServerClass::EEWebServerClass(int port = 80)
{
	_handle_file_create = [&]() { handleFileCreate(); };
	_handle_file_delete = [&]() { handleFileDelete(); };
	_handle_file_list = [&]() { handleFileList(); };
	_handle_file_upload = [&]() { handleFileUpload(); };
}

void EEWebServerClass::init(String updatePassword)
{
	SUSPEND_TIMER1();
	if (!FILESYSTEM.begin())
	{
		PRINTLN("[Storage] Couldn't mount file system.");
		return;
	}
	RESUME_TIMER1();

	on("/edit", HTTP_GET, [&]()
	{
		if(!handleFileRead("/edit.htm"))
		{
			String content	=	"<form method='post' enctype='multipart/form-data'>";
			content			+=		"<label>Upload a file (e.g. <i>edit.htm.gz</i>) to flash storage:";
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

	if (updatePassword == "")
		httpUpdater.setup(this, "/update");
	else
		httpUpdater.setup(this, "/update", "admin", updatePassword);

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

	SUSPEND_TIMER1();
	if(FILESYSTEM.exists(pathWithGz) || FILESYSTEM.exists(path))
	{
		if(FILESYSTEM.exists(pathWithGz))
			path += ".gz";

		File file = FILESYSTEM.open(path, "r");
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

	SUSPEND_TIMER1();
	if(_upload.status == UPLOAD_FILE_START)
	{
		String filename = _upload.filename;
		if(!filename.startsWith("/"))
			filename = "/" + filename;
		PRINTLN("[Storage] Uploading file: " + filename);

		fsUploadFile = FILESYSTEM.open(filename, "w");
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

	SUSPEND_TIMER1();
	if(!FILESYSTEM.exists(path))
		return send(404, "text/plain", "Oops, file not found!");

	FILESYSTEM.remove(path);

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

	SUSPEND_TIMER1();
	if(FILESYSTEM.exists(path))
	{
		return send(500, "text/plain", "File already exists!");
	}

	File file = FILESYSTEM.open(path, "w");

	if(file)
		file.close();
	else
		return send(500, "text/plain", "Oops, creating file failed!");

	path = String();
	send(200, "text/plain", "");
}

void EEWebServerClass::handleFileList()
{
	if(!hasArg("dir"))
	{
		send(500, "text/plain", "BAD ARGS");
		return;
	}

	SUSPEND_TIMER1();
#if defined(ESP32)
	String path = arg("dir");
	File dir = FILESYSTEM.open(path);
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
			output += (isDir) ? "dir" : "file";
			output += "\",\"name\":\"";
			output += String(file.path()).substring(1);
			output += "\"}";
			file = dir.openNextFile();
		}
	}
	output += "]";
#elif defined(ESP8266)
	String path = arg("dir");
	Dir dir = FILESYSTEM.openDir(path);
	path = String();

	String output = "[";
	while(dir.next())
	{
		File entry = dir.openFile("r");
		if (output != "[") output += ',';
		bool isDir = false;
		output += "{\"type\":\"";
		output += (isDir) ? "dir" : "file";
		output += "\",\"name\":\"";
		String name = String(entry.name());
		if (name.startsWith("/"))
			output += name.substring(1);
		else
			output += name;
		output += "\"}";
		entry.close();
	}
	output += "]";
#endif
	send(200, "text/json", output);
}

EEWebServerClass WebServer(80);

} // namespace ESPEssentials
