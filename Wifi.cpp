#include "Wifi.h"

WifiClass::WifiClass()
{
	ap_connected = false;
}

bool WifiClass::connect(String ssid, String passphrase)
{
	if(status() == WL_CONNECTED)
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

	if(status() == WL_CONNECTED)
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

	WiFi.mode(WIFI_AP_STA);
	
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

bool WifiClass::autoConnect(char *ap_name)
{
	WifiCredentials credentials = loadCredentials();

	if(credentials.ssid == "" || !connect(credentials.ssid, credentials.passphrase))
	{
		return createAP(ap_name);
	}
	
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

void WifiClass::saveCredentials(String ssid, String passphrase)
{
	EEPROM.begin(64);

	// Read two EEPROM strings with length 32
	for(int i = 0; i < 32; i++)
	{
		EEPROM.write(0 + i, ssid[i]); // ssid_address = 0
	}
	for(int i = 0; i < 32; i++)
	{
		EEPROM.write(32 + i, passphrase[i]); // passphrase_address = 32
	}
	EEPROM.commit();
}

WifiCredentials WifiClass::loadCredentials()
{
	EEPROM.begin(64);
	WifiCredentials credentials;

	// Read two EEPROM strings with length 32
	for(int i = 0; i < 32; i++)
	{
		credentials.ssid += char(EEPROM.read(0 + i)); // ssid_address = 0
	}
	for(int i = 0; i < 32; i++)
	{
		credentials.passphrase += char(EEPROM.read(32 + i)); // passphrase_address = 32
	}

	return credentials;
}

WifiClass Wifi;