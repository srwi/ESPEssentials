#include "EEWifi.h"

namespace ESPEssentials
{

EEWifiClass::EEWifiClass()
{
	setConfigPortalBlocking(false);
}

EEWifiClass EEWifi;

} // namespace ESPEssentials
