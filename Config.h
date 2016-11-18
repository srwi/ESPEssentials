#ifndef _CONFIG_H_471412218
#define _CONFIG_H_471412218

#include <Arduino.h>
#include <LinkedList.h>
#include <FS.h>

class ConfigClass
{
	private:
		struct config_element
		{
			String name;
			String value;
		};
		const String config_filename = "/config.txt";
		LinkedList<config_element> config_list;
		String filename;
		bool is_initialized = false;

	public:
		bool init();
		bool save();
		void set(String name, String value);
		String getString(String element);
		float getFloat(String element);
		int getInt(String element);
		bool getBool(String element);
		bool remove(String name);
		bool contains(String name);
};

extern ConfigClass Config;

#endif