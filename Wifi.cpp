#include "Wifi.h"

WifiClass::WifiClass()
{
	ap_connected = false;
}

bool WifiClass::connect(String ssid, String passphrase)
{
	if(isConnected())
	{
		Serial.println("[Wifi] Wifi already connected");
		return true;
	}

	Serial.printf("[Wifi] Connecting to \"%s\"...", ssid.c_str());

	// Don't remember future credentials
	persistent(false);

	mode(WIFI_STA);

	for(uint16_t i = 0; i < WIFI_CONNECT_MAX_ATTEMPTS; i++)
	{
		begin(ssid.c_str(), passphrase.c_str());
		
		if(waitForConnectResult() == WL_CONNECTED)
		{
			break;
		}
		else
		{
			Serial.print(".");
			delay(WIFI_CONNECT_RETRY_TIMEOUT);
		}
	}

	if(isConnected())
	{
		Serial.println("ok!");
		Serial.print("[Wifi] IP Address: ");
		Serial.println(WiFi.localIP());
		is_access_point = false;
		return true;
	}
	else
	{
		Serial.println("failed!");
		return false;
	}
}

bool WifiClass::createAP(char *ap_name)
{
	Serial.print("[Wifi] Starting access point...");

	if(ap_connected = WiFi.softAP(ap_name))
	{
		Serial.println("ok!");
		Serial.println("[Wifi] IP Address: 192.168.4.1");
		return true;
	}
	else
	{
		Serial.println("failed!");
		return false;
	}
}

bool WifiClass::autoConnect(String ssid, String passphrase, char *ap_name)
{
	if(ssid == "" || !connect(ssid, passphrase))
		return createAP(ap_name);
	
	return true;
}

bool WifiClass::isAP()
{
	return ap_connected;
}

void WifiClass::forceRemoveCredentials()
{
	ESP.flashEraseSector(0x3fe);
}

WifiClass Wifi;