#pragma once

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <Windows.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

#define CONFIG_VALID_KEY_CHARS "abcdefghijklmnopqrstuvwxyz_0123456789"
#define CONFIG_WHITESPACES " \t\r\n\v\b\f"

class ConfigFile
{
public:
	//Creates Config object
	//If file parameter is specified it will load that file
	ConfigFile();
	ConfigFile(string configName);
	ConfigFile(ifstream& file);
	~ConfigFile() = default;

	//Loads file contents to storage.
	//Exisings settings are not removed from storage.
	//If there is setting with the same name in loaded file it will overwrite existing value for that entry
	bool Load(ifstream& file);
	bool Load(string fileName);
	bool Load();

	//Saves contents of internal storage to file
	//If no file specified it will use filename from constructor.
	bool Save(bool sortAlphabetically = false);
	bool Save(ofstream& file, bool sortAlphabetically = false);
	bool Save(string fileName, bool sortAlphabetically = false);

	//Getters funcitons
	//If setting with given name is not found in storage it will return "noKeyValue" and
	//add that setting to storage with default value
	string GetString(string key, string noKeyValue);
	bool GetBool(string key, bool noKeyValue);
	int GetInt(string key, int noKeyValue);
	unsigned int GetUInt(string key, int noKeyValue);
	__int64 GetInt64(string key, __int64 noKeyValue);
	float GetFloat(string key, float noKeyValue);
	double GetDouble(string key, double noKeyValue);
	unsigned int GetPtr32(string key, unsigned int noKeyValue);
	unsigned __int64 GetPtr64(string key, unsigned __int64 noKeyValue);
	unsigned int GetVKCode(string key, unsigned int noKeyValue);

	//Setters for diferent types of setting.
	//If setting already exists in storage only value will be updated.
	//If there is no setting with that name in storage it will be created.
	void SetString(string key, string value);
	void SetBool(string key, bool value);
	void SetInt(string key, int value);
	void SetUInt(string key, unsigned int value);
	void SetInt64(string key, __int64 value);
	void SetFloat(string key, float value);
	void SetDouble(string key, double value);
	void SetPtr32(string key, unsigned int value);
	void SetPtr64(string key, unsigned __int64 value);
	void SetVKCode(string key, unsigned int value);

private:
	unordered_map<string, string> configMap;
	vector<string> orderVector;
	string fileName;
	static unordered_map<string, unsigned int>* GetKeyMap();
	static unordered_map<unsigned int, string>* GetVKMap();
	bool AddKeyValueToMap(string key, string value);
};