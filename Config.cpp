#include "Config.h"

bool ConfigClass::init()
{
	if(is_initialized)
	{
		return true;
	}

	if (!SPIFFS.begin())
	{
		Serial.println("[Config] Couldn't mount file system.");
		is_initialized = false;
		return false;
	}

	File configFile = SPIFFS.open(config_filename, "r");
	if (!configFile)
	{
		Serial.println("[Config] Opening file '" + config_filename + "' failed.");
		is_initialized = false;
		return false;
	}

	String currentLine, name, value;
	int16_t delimiterPosition;
	while(configFile.available())
	{
		currentLine = configFile.readStringUntil('\n');
		
		// Check current line for settings
		delimiterPosition = currentLine.indexOf("=");
		if(delimiterPosition != -1)
		{
			name = currentLine.substring(0, delimiterPosition);
			name.trim();

			value = currentLine.substring(delimiterPosition + 1);
			value.trim();

			if(name != "" && value != "")
			{
				Serial.println("[Config] Found \"" + name + "\" value: " + value);

				config_element conf_elem = {name, value};
				config_list.add(conf_elem);
			}
		}
	}

	is_initialized = true;
	return true;
}

bool ConfigClass::save()
{
	File configFile = SPIFFS.open(config_filename, "w");
	if(!configFile)
	{
		Serial.println("[Config] Opening file " + config_filename + " for saving failed.");
		return false;
	}
	
	if(config_list.size() == 0)
	{
		Serial.println("[Config] Config list empty. File will be removed.");
		SPIFFS.remove(config_filename);
		return true;
	}

	for(uint16_t i = 0; i < config_list.size(); i++)
	{
		configFile.println(config_list.get(i).name + " = " + config_list.get(i).value);
	}
	configFile.close();
	Serial.println("[Config] Saved config file " + config_filename);
	return true;
}

bool ConfigClass::getBool(String element)
{
	String s = getString(element);
	return s == "true" || s == "1" || s == "yes";
}

float ConfigClass::getFloat(String element)
{
	return getString(element).toFloat();
}

int ConfigClass::getInt(String element)
{
	return getString(element).toInt();
}

String ConfigClass::getString(String element)
{
	for(uint16_t i = 0; i < config_list.size(); i++)
	{
		if(config_list.get(i).name == element)
		{
			return config_list.get(i).value;
		}
	}

	return "";
}

void ConfigClass::set(String name, String value)
{
	config_element conf_elem = {name, value};

	for(uint16_t i = 0; i < config_list.size(); i++)
	{
		if(config_list.get(i).name == name)
		{
			config_list.set(i, conf_elem);
			return;
		}
	}

	config_list.add(conf_elem);
}

bool ConfigClass::remove(String name)
{
	for(uint16_t i = 0; i < config_list.size(); i++)
	{
		if(config_list.get(i).name == name)
		{
			config_list.remove(i);
			return true;
		}
	}

	return false;
}

bool ConfigClass::contains(String name)
{
	for(uint16_t i = 0; i < config_list.size(); i++)
	{
		if(config_list.get(i).name == name)
		{
			return true;
		}
	}

	return false;
}

ConfigClass Config;