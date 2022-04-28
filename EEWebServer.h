#ifndef ESPESSENTIALS_WEBSERVER_H
#define ESPESSENTIALS_WEBSERVER_H

#include <Arduino.h>
#include <ESP8266WebServer.h>

class WebServerClass : public ESP8266WebServer
{
	public:
		WebServerClass(int port);
		void init();
		bool handleFileRead(String path);
		bool isBusy();

	private:
		File fsUploadFile;
		THandlerFunction _handle_file_create;
		THandlerFunction _handle_file_delete;
		THandlerFunction _handle_file_list;
		THandlerFunction _handle_file_upload;
		THandlerFunction _handle_update;
		bool webserverBusy = false;

		String formatBytes(size_t bytes);
		String getContentType(String filename);
		void handleFileUpload();
		void handleFileDelete();
		void handleFileCreate();
		void handleFileList();
		void handleUpdate();
};

extern WebServerClass WebServer;

#endif
