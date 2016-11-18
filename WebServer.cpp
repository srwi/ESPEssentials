#include "WebServer.h"

WebServerClass::WebServerClass(int port = 80) : ESP8266WebServer(port)
{
	_handle_file_create = [&]() { handleFileCreate(); };
	_handle_file_delete = [&]() { handleFileDelete(); };
	_handle_file_list = [&]() { handleFileList(); };
	_handle_file_upload = [&]() { handleFileUpload(); };
	_handle_wifi_credentials = [&]() { handleWifiCredentials(); };
}

void WebServerClass::init()
{
	if (!SPIFFS.begin())
	{
		Serial.println("[Wifi] Couldn't mount file system.");
		return;
	}
	
	on("/edit", HTTP_GET, [&]()
	{
		if(!handleFileRead("/edit.htm"))
			send(404, "text/plain", "Oops, file not found!");
	});
	on("/edit", HTTP_PUT, _handle_file_create);
	on("/edit", HTTP_DELETE, _handle_file_delete);
	on("/edit", HTTP_POST, [&](){ send(200, "text/plain", ""); }, _handle_file_upload);
	on("/list", HTTP_GET, _handle_file_list);
	if(Wifi.isAP())
	{
		on("/", [&]()
		{
			String content = "<html><body><form action='/wifi_credentials' method='POST'>Please enter new wifi credentials:<br>";
			content += "<input type='text' name='SSID' placeholder='Network Name'><br>";
			content += "<input type='password' name='PASSPHRASE' placeholder='Password'><br>";
			content += "<input type='submit' name='SUBMIT' value='Save and restart!'></form>";
			send(200, "text/html", content);
		});
		on("/wifi_credentials", _handle_wifi_credentials);
	}
	// This is actually used to retrieve all other websites from SPIFFS (or sending error 404 if they don't exist)
	onNotFound([&]()
	{
		if(!handleFileRead(uri()))
		{
			Serial.println("[Storage] Couldn't find file at \'" + uri() + "\'" + ".");
			send(404, "text/plain", "Oops, file not found!");
		}
	});
	
	begin();
}

String WebServerClass::formatBytes(size_t bytes)
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

String WebServerClass::getContentType(String filename)
{
	if(hasArg("download")) return "application/octet-stream";
	else if(filename.endsWith(".htm")) return "text/html";
	else if(filename.endsWith(".html")) return "text/html";
	else if(filename.endsWith(".conf")) return "text/html";
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

void WebServerClass::handleWifiCredentials()
{
	// TODO: Implement open wifi networks
	if (hasArg("SSID")/* && hasArg("PASSPHRASE")*/)
	{
		send(404, "text/plain", "The device will now restart...");
		Config.set("ssid", String(arg("SSID")));
		Config.set("passphrase" , String(arg("PASSPHRASE")));
		Config.save();
		Serial.println("[Wifi] Got new credentials through AP:");
		Serial.println("\tSSID: " + arg("SSID"));
		Serial.println("\tPassphrase: " + arg("PASSPHRASE"));
		delay(30);
		ESP.restart();
	}
	else
	{
		send(404, "text/plain", "Oops, you forgot something!");
	}
}

bool WebServerClass::handleFileRead(String path)
{
	Serial.println("[Storage] File read: " + path);
	
	if(path.endsWith("/")) path += "index.htm";
	String contentType = getContentType(path);
	String pathWithGz = path + ".gz";

	if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path))
	{
		if(SPIFFS.exists(pathWithGz))
			path += ".gz";

		File file = SPIFFS.open(path, "r");
		size_t sent = streamFile(file, contentType);
		file.close();

		return true;
	}

	File file = SPIFFS.open(path, "r");
	size_t sent = streamFile(file, contentType);
	file.close();

	return false;
}

void WebServerClass::handleFileUpload()
{
	if(uri() != "/edit")
		return;

	HTTPUpload& _upload = upload();
	
	if(_upload.status == UPLOAD_FILE_START)
	{
		String filename = _upload.filename;
		if(!filename.startsWith("/"))
			filename = "/" + filename;
		Serial.println("[Storage] Uploading file: " + filename);

		fsUploadFile = SPIFFS.open(filename, "w");
		filename = String();
	}
	else if(_upload.status == UPLOAD_FILE_WRITE)
	{
		//Serial.print("[Storage] Receiving..." + _upload.currentSize);
		if(fsUploadFile)
			fsUploadFile.write(_upload.buf, _upload.currentSize);
	}
	else if(_upload.status == UPLOAD_FILE_END)
	{
		if(fsUploadFile)
			fsUploadFile.close();
		Serial.println("[Storage] Received total: " + formatBytes(_upload.totalSize));
	}
}

void WebServerClass::handleFileDelete()
{
	if(args() == 0)
		return send(500, "text/plain", "BAD ARGS");
	
	String path = arg(0);
	Serial.println("[Storage] Deleting file: " + path);
	
	if(path == "/")
		return send(500, "text/plain", "BAD PATH");
	if(!SPIFFS.exists(path))
		return send(404, "text/plain", "Oops, file not found (3)!");
	
	SPIFFS.remove(path);
	
	send(200, "text/plain", "");
	path = String();
}

void WebServerClass::handleFileCreate()
{
	if(args() == 0)
		return send(500, "text/plain", "BAD ARGS");
	
	String path = arg(0);
	Serial.println("[Storage] Creating file: " + path);
	
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

void WebServerClass::handleFileList()
{
	if(!hasArg("dir"))
	{
		send(500, "text/plain", "BAD ARGS");
		return;
	}
	
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

	send(200, "text/json", output);
}

WebServerClass WebServer(80);