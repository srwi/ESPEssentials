#ifndef _WIFI_H_471412218
#define _WIFI_H_471412218

#include <Arduino.h>
#include <ESP8266WiFi.h>

#define WIFI_CONNECT_MAX_ATTEMPTS 10
#define WIFI_CONNECT_RETRY_TIMEOUT 1000

class WifiClass : public ESP8266WiFiClass
{
	public:
		WifiClass();
		bool connect(String ssid, String passphrase);
		bool createAP(char *ap_name = "ESP8266 (192.168.4.1)");
		bool autoConnect(String ssid, String passphrase, char *ap_name = "ESP8266 (192.168.4.1)");
		bool isAP();
		void forceRemoveCredentials();

	private:
		bool ap_connected;
		bool is_access_point;
};

extern WifiClass Wifi;

#endif