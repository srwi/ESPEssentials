#ifndef _WIFI_H_471412218
#define _WIFI_H_471412218

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <EEPROM.h>

#define WIFI_CONNECT_MAX_ATTEMPTS 3
#define WIFI_CONNECT_RETRY_TIMEOUT 1000

struct WifiCredentials
{
	String ssid;
	String passphrase;
};

class WifiClass : public ESP8266WiFiClass
{
	public:
		WifiClass();
		bool connect(String ssid, String passphrase);
		bool createAP(char *ap_name = "ESP8266 (192.168.4.1)");
		bool autoConnect(char *ap_name = "ESP8266 (192.168.4.1)");
		bool isAP();
		void saveCredentials(String ssid, String passphrase);
		WifiCredentials loadCredentials();
		void forceRemoveCredentials();

	private:
		bool ap_connected;
		bool is_access_point;
		const String config_filename = "/wifi_credentials.txt";
};

extern WifiClass Wifi;

#endif