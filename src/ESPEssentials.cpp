#include "ESPEssentials.h"

#include "EESerialOut.h"

namespace ESPEssentials
{

void handle()
{
	Wifi.process();
	WebServer.handleClient();
	OTA.handle();
}

void init(String projectName, int baudRate, String updatePassword)
{
	Serial.begin(baudRate);
	PRINTLN("");

	if(Wifi.autoConnect((projectName + " Setup").c_str()))
	{
		WebServer.init(updatePassword);
		OTA.init(projectName.c_str(), updatePassword.c_str());
	}
}

void init(String projectName, String updatePassword)
{
	init(projectName, ESSENTIALS_BAUD, updatePassword);
}

} // namespace ESPEssentials
