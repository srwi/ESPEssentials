#pragma once

#include <ArduinoOTA.h>

namespace ESPEssentials
{

class OTAClass : public ArduinoOTAClass
{
	public:
		void init(char const *hostname = "", char const *password = "", uint16_t port = 0);
};

extern OTAClass OTA;

} // namespace ESPEssentials
