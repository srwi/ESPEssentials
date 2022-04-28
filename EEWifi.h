#pragma once

#include <WiFiManager.h>

namespace ESPEssentials
{

class EEWifiClass : public WiFiManager
{
	public:
		EEWifiClass();
};

extern EEWifiClass Wifi;

} // namespace ESPEssentials
