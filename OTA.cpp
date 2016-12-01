#include "OTA.h"

void OTAClass::init(char *hostname, char *password, uint16_t port)
{
	if(!Wifi.isConnected() || Wifi.isAP())
	{
		Serial.println("[OTA] No wifi connection to router. OTA will not be available!");
		return;
	}

	if(hostname != "")
    	setHostname(hostname);
	if(password != "")
		setPassword((const char *)password);
	if(port)
		setPort(port);

	onStart([]()
	{
		Serial.println("[OTA] Start");
	});
	onEnd([]()
	{
		Serial.println("\n[OTA] End");
	});
	onProgress([](unsigned int progress, unsigned int total)
	{
		Serial.printf("[OTA] Progress: %u%%\r", (progress / (total / 100)));
	});
	onError([](ota_error_t error)
	{
		Serial.printf("[OTA] Error[%u]: ", error);
		if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
		else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
		else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
		else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
		else if (error == OTA_END_ERROR) Serial.println("End Failed");
	});

	begin();
	Serial.println("[OTA] OTA started");
}

OTAClass OTA;