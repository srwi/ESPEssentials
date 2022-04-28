#ifndef ESPESSENTIALS_OTA_H
#define ESPESSENTIALS_OTA_H

#include "Wifi.h"

#include <ArduinoOTA.h>

class OTAClass : public ArduinoOTAClass
{
	public:
		void init(char const *hostname = "", char const *password = "", uint16_t port = 0);
};

extern OTAClass OTA;

#endif
