# ESPEssentials

[![License: LGPL v2.1](https://img.shields.io/badge/License-LGPL%20v2.1-blue.svg)](https://www.gnu.org/licenses/lgpl-3.0)

Essentials for the ESP8266 to get you started with your Arduino project without any of the annoying stuff.

## Features
- Serial output
- Wifi connection management using [WiFiManager](https://github.com/tzapu/WiFiManager)
- Webserver including file upload and editing
- OTA firmware update
- Web firmware update

## Requirements
- [WiFiManager](https://github.com/tzapu/WiFiManager) &ge; 2.0.0

## Usage

ESPEssentials only needs three lines of additional code:

```cpp
#include <ESPEssentials.h>

void setup()
{
    initESPEssentials("Project Name");
}

void loop()
{
    handleESPEssentials();
}
```

## Notes

### Serial

By default the baud rate will be 115200. You can specify it with
```cpp
initESPEssentials("Project Name", 9600);
```

### WiFiManager

All WiFiManager methods can be accessed through the `Wifi` instance. For example to reset wifi settings:

```cpp
Wifi.resetSettings();
```

More information can be found [here](https://github.com/tzapu/WiFiManager).

### Webserver

HTTP requests can be initialized within `setup()`. For example to reset wifi settings by accessing `http://<device-ip>/reset_wifi`:

```cpp
WebServer.on("/reset_wifi", HTTP_GET, [&]() {
    Wifi.resetSettings();
    WebServer.send(200, "text/plain", "Wifi settings reset.");
});
```

*Note:* The routes `/edit`, `/handle_update`, `/list`, `/reboot` and `/update` are already used by ESPEssentials and can not be used.

To edit files inside your browser upload the [_edit.htm.gz_](https://github.com/stnkl/ESPEssentials/blob/master/data/edit.htm.gz) to the root directory. The easiest way to do this is by accessing `http://<device-ip>/edit` and uploading it from there.

### OTA firmware update

An OTA password can be set up with

```cpp
initESPEssentials("Project Name", "hunter2");
```

### Web firmware update

Open `http://<device-ip>/update` and select a `.bin` file to upload a new firmware to the ESP8266 from within the web browser.
