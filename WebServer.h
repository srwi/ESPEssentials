#ifndef _WEBSERVER_H_471412218
#define _WEBSERVER_H_471412218

#include <Arduino.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include "Wifi.h"
#include "Config.h"

class WebServerClass : public ESP8266WebServer
{
	public:
		WebServerClass(int port);
		void init();
		void handle(); // TODO: needed?

	private:
		File fsUploadFile;
		THandlerFunction _handle_file_create;
		THandlerFunction _handle_file_delete;
		THandlerFunction _handle_file_list;
		THandlerFunction _handle_file_upload;
		THandlerFunction _handle_wifi_credentials;

		String formatBytes(size_t bytes);
		String getContentType(String filename);
		bool handleFileRead(String path);
		void handleFileUpload();
		void handleFileDelete();
		void handleFileCreate();
		void handleFileList();
		void handleWifiCredentials();
};

extern WebServerClass WebServer;

#endif