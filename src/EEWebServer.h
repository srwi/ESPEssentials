#pragma once

#include <Arduino.h>

#if defined(ESP32)
	#include <FS.h>
	#include <HTTPUpdateServer.h>
	#include <WebServer.h>

	typedef WebServer EEBaseWebServerClass;
	typedef HTTPUpdateServer HTTPUpdateServerType;
#elif defined(ESP8266)
	#include <ESP8266HTTPUpdateServer.h>
	#include <ESP8266WebServer.h>

	typedef ESP8266WebServer EEBaseWebServerClass;
	typedef ESP8266HTTPUpdateServer HTTPUpdateServerType;
#endif

namespace ESPEssentials
{

class EEWebServerClass : public EEBaseWebServerClass
{
	public:
		EEWebServerClass(int port);
		void init(String updatePassword = "");
		bool handleFileRead(String path);
		bool isBusy();

	private:
		File fsUploadFile;
		THandlerFunction _handle_file_create;
		THandlerFunction _handle_file_delete;
		THandlerFunction _handle_file_list;
		THandlerFunction _handle_file_upload;
		bool webserverBusy = false;
		HTTPUpdateServerType httpUpdater;

		String formatBytes(size_t bytes);
		String getContentType(String filename);
		void handleFileUpload();
		void handleFileDelete();
		void handleFileCreate();
		void handleFileList();
};

extern EEWebServerClass WebServer;

} // namespace ESPEssentials
