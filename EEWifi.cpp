#include "EEWifi.h"

namespace ESPEssentials
{

EEWifiClass::EEWifiClass()
{
	setConfigPortalBlocking(false);
}

EEWifiClass Wifi;

} // namespace ESPEssentials
