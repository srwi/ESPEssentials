#ifndef ESPESSENTIALS_H
#define ESPESSENTIALS_H

#include "Wifi.h"
#include "OTA.h"
#include "WebServer.h"

#define ESSENTIALS_BAUD 115200

void handleESPEssentials();
void initESPEssentials(String projectName, int baudRate = ESSENTIALS_BAUD, String otaPassword = "");
void initESPEssentials(String projectName, String otaPassword);

#endif