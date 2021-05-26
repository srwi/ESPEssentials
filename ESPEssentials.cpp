#include "ESPEssentials.h"

#include "Wifi.h"
#include "OTA.h"
#include "SerialOut.h"
#include "WebServer.h"

void handleESPEssentials()
{
	Wifi.process();
	WebServer.handleClient();
	OTA.handle();
}

void initESPEssentials(String projectName, int baudRate, String otaPassword)
{
	Serial.begin(baudRate);
	PRINTLN("");

	if(Wifi.autoConnect((projectName + " Setup").c_str()))
	{
		WebServer.init();
		OTA.init(projectName.c_str(), otaPassword.c_str());
	}
}

void initESPEssentials(String projectName, String otaPassword)
{
	initESPEssentials(projectName, ESSENTIALS_BAUD, otaPassword);
}
