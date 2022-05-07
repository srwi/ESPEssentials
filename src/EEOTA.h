#pragma once

#include <ArduinoOTA.h>

namespace ESPEssentials
{

class OTAClass : public ArduinoOTAClass
{
	public:
		void init(char const *hostname = "", char const *password = "", uint16_t port = 0);
		bool isBusy();

	private:
		bool m_isBusy;
};

extern OTAClass OTA;

} // namespace ESPEssentials
