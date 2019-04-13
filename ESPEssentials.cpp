#include "ESPEssentials.h"

void handleESPEssentials()
{
	Wifi.process();
	WebServer.handleClient();
	OTA.handle();
}

void initESPEssentials(String projectName)
{
	Serial.begin(115200);
	Serial.println("");

	Wifi.autoConnect((projectName + " Setup").c_str());
	WebServer.init();
	OTA.init((projectName + " (PW: okay)").c_str(), "okay");
}