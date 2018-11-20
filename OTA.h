#ifndef _OTA_H_714124218
#define _OTA_H_714124218

#include <ArduinoOTA.h>
#include "Wifi.h"

class OTAClass : public ArduinoOTAClass
{
	public:
		void init(char const *hostname = "", char *password = "", uint16_t port = 0);
};

extern OTAClass OTA;

#endif