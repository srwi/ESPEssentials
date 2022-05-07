#include "EEOTA.h"
#include "EESerialOut.h"

namespace ESPEssentials
{

void OTAClass::init(char const *hostname, char const *password, uint16_t port)
{
	if(strcmp(hostname, "") != 0)
		setHostname(hostname);
	if(strcmp(password, "") != 0)
		setPassword((const char *)password);
	if(port)
		setPort(port);

	m_isBusy = false;

	onStart([&]()
	{
		PRINTLN("[OTA] Start");
		m_isBusy = true;
	});
	onEnd([&]()
	{
		PRINTLN("\n[OTA] End");
		m_isBusy = false;
	});
	onProgress([](unsigned int progress, unsigned int total)
	{
		PRINTF("[OTA] Progress: %u%%\r", (progress / (total / 100)));
	});
	onError([&](ota_error_t error)
	{
		PRINTF("[OTA] Error[%u]: ", error);

		switch(error)
		{
			case OTA_AUTH_ERROR:
				PRINTLN("Auth Failed");
				break;
			case OTA_BEGIN_ERROR:
				PRINTLN("Begin Failed");
				break;
			case OTA_CONNECT_ERROR:
				PRINTLN("Connect Failed");
				break;
			case OTA_RECEIVE_ERROR:
				PRINTLN("Receive Failed");
				break;
			case OTA_END_ERROR:
				PRINTLN("End Failed");
				break;
		}

		m_isBusy = false;
	});

	begin();
	PRINTLN("[OTA] OTA started");
}

bool OTAClass::isBusy()
{
	return m_isBusy;
}

OTAClass OTA;

} // namespace ESPEssentials
