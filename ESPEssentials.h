#pragma once

#include "EEOTA.h"
#include "EEWifi.h"

#include <Arduino.h>

#define ESSENTIALS_BAUD 115200

namespace ESPEssentials
{

void handle();
void init(String projectName, int baudRate = ESSENTIALS_BAUD, String otaPassword = "");
void init(String projectName, String otaPassword);

} // namespace ESPEssentials
