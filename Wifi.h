#ifndef _WIFI_H_471412218
#define _WIFI_H_471412218

#include <WiFiManager.h>

class WifiClass : public WiFiManager
{
	public:
		WifiClass();
};

extern WifiClass Wifi;

#endif