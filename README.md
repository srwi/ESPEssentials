# ESPEssentials

## What does it enable?
- Serial output (Baudrate: 115200)
- Wifi connection management using [WifiManager](https://github.com/tzapu/WiFiManager). Use Development branch version!
- Webserver including file upload and editing (Access via http://&lt;device-ip&gt;/edit)
- OTA update
- Web update (Access via http://&lt;device-ip&gt;/update)

## Usage
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
- To edit files inside your browser upload the [_edit.htm.gz_](https://github.com/stnkl/ESPEssentials/blob/master/data/edit.htm.gz) to the root directory.
