#ifndef _ESPESSENTIALS_H_1436481
#define _ESPESSENTIALS_H_1436481

#include "Wifi.h"
#include "WebServer.h"
#include "OTA.h"

#define ESSENTIALS_BAUD 115200

void handleESPEssentials();
void initESPEssentials(String projectName, int baudRate = ESSENTIALS_BAUD, String otaPassword = "");
void initESPEssentials(String projectName, String otaPassword);

#endif