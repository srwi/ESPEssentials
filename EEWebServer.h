#pragma once

#include <Arduino.h>

#if defined(ESP32)
	#include <WebServer.h>
	#include <FS.h>

	typedef WebServer EEBaseWebServerClass;
#elif defined(ESP8266)
	#include <ESP8266WebServer.h>

	typedef ESP8266WebServer EEBaseWebServerClass;
#endif

namespace ESPEssentials
{

class EEWebServerClass : public EEBaseWebServerClass
{
	public:
		EEWebServerClass(int port);
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

extern EEWebServerClass WebServer;

} // namespace ESPEssentials
