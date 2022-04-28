#include "ESPEssentials.h"

#include "EEOTA.h"
#include "EESerialOut.h"
#include "EEWebServer.h"
#include "EEWifi.h"

namespace ESPEssentials
{

void handle()
{
	Wifi.process();
	WebServer.handleClient();
	OTA.handle();
}

void init(String projectName, int baudRate, String otaPassword)
{
	Serial.begin(baudRate);
	PRINTLN("");

	if(Wifi.autoConnect((projectName + " Setup").c_str()))
	{
		WebServer.init();
		OTA.init(projectName.c_str(), otaPassword.c_str());
	}
}

void init(String projectName, String otaPassword)
{
	init(projectName, ESSENTIALS_BAUD, otaPassword);
}

} // namespace ESPEssentials
