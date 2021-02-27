#include "OTA.h"

void OTAClass::init(char const *hostname, char const *password, uint16_t port)
{
	if(strcmp(hostname, "") != 0)
		setHostname(hostname);
	if(strcmp(password, "") != 0)
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

		switch(error)
		{
			case OTA_AUTH_ERROR:
				Serial.println("Auth Failed");
				break;
			case OTA_BEGIN_ERROR:
				Serial.println("Begin Failed");
				break;
			case OTA_CONNECT_ERROR:
				Serial.println("Connect Failed");
				break;
			case OTA_RECEIVE_ERROR:
				Serial.println("Receive Failed");
				break;
			case OTA_END_ERROR:
				Serial.println("End Failed");
				break;
		}
	});

	begin();
	Serial.println("[OTA] OTA started");
}

OTAClass OTA;