# ESPEssentials

![ESP8266](https://img.shields.io/badge/ESP-8266-000000.svg?colorB=blue)
![ESP32](https://img.shields.io/badge/ESP-32-000000.svg?colorB=blue)
[![arduino-library-badge](https://www.ardu-badge.com/badge/ESPEssentials.svg)](https://www.ardu-badge.com/ESPEssentials)
[![LGPL-2.1 license](https://img.shields.io/github/license/stnkl/ESPEssentials)](https://github.com/stnkl/ESPEssentials/blob/master/LICENSE)

Essentials for the ESP8266 and ESP32 to get you started with your Arduino project without any of the annoying stuff.

## Features
- Serial output
- Wifi connection management using [WiFiManager](https://github.com/tzapu/WiFiManager)
- Webserver including file upload and editing via browser
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
    ESPEssentials::init("Project Name");
}

void loop()
{
    ESPEssentials::handle();
}
```

## Notes

### Serial

By default the baud rate will be 115200. You can specify it with
```cpp
ESPEssentials::init("Project Name", 9600);
```

### WiFiManager

All WiFiManager methods can be accessed through the `Wifi` instance. For example to reset wifi settings:

```cpp
ESPEssentials::Wifi.resetSettings();
```

More information can be found [here](https://github.com/tzapu/WiFiManager).

### Webserver

HTTP requests can be initialized within `setup()`. For example to reset wifi settings by accessing `http://<device-ip>/reset_wifi`:

```cpp
ESPEssentials::WebServer.on("/reset_wifi", HTTP_GET, [&]() {
    ESPEssentials::WebServer.send(200, "text/plain", "Wifi settings reset.");
    ESPEssentials::Wifi.resetSettings();
});
```

*Note:* The routes `/edit`, `/handle_update`, `/list`, `/reboot` and `/update` are already used by ESPEssentials and can not be used.

To edit files inside your browser upload [`/data/edit.htm.gz`](https://github.com/stnkl/ESPEssentials/blob/master/data/edit.htm.gz) to the root directory. The easiest way to do this is by accessing `http://<device-ip>/edit` and uploading it from there. Afterwards you can access the file editor via the same URL to edit files directly on the device.

Each file created/uploaded that way can be accessed via the corresponding URL (e.g. `http://<device-ip>/myPage` to access `/myPage.htm`).

### OTA firmware update

An OTA password can be set up with

```cpp
ESPEssentials::init("Project Name", "hunter2");
```

### Web firmware update

Open `http://<device-ip>/update` and select a `.bin` file to upload a new firmware to the ESP8266/ESP32 from within the web browser. If a password has been supplied to `init` you will be asked to authenticate. The username is `admin`.
