#ifndef _ESPESSENTIALS_H_1436481
#define _ESPESSENTIALS_H_1436481

#include "Wifi.h"
#include "WebServer.h"
#include "OTA.h"

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
	OTA.init((projectName + " (PW: yes)").c_str(), "yes");
}

#endif