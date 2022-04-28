#ifndef ESPESSENTIALS_WIFI_H
#define ESPESSENTIALS_WIFI_H

#include <WiFiManager.h>

class WifiClass : public WiFiManager
{
	public:
		WifiClass();
};

extern WifiClass Wifi;

#endif
