#include "Config.h"
ConfigFile::ConfigFile()
{
	fileName = "";
}

ConfigFile::ConfigFile(string configName) : fileName(configName)
{
	Load(configName);
}

ConfigFile::ConfigFile(ifstream& file) : ConfigFile()
{
	Load(file);
}

bool ConfigFile::Load(ifstream& file)
{
	if (!file.is_open())
		return false;

	string line;

	for (int i = 0; getline(file, line); i++)
	{
		string key, value;
		string lowerLine = line;
		transform(lowerLine.begin(), lowerLine.end(), lowerLine.begin(), ::tolower);
		istringstream iss(line);

		int keyStart = lowerLine.find_first_of(CONFIG_VALID_KEY_CHARS);
		int keyEnd = lowerLine.find_first_not_of(CONFIG_VALID_KEY_CHARS, keyStart);
		if (keyStart == string::npos || keyEnd == string::npos) continue;
		key = lowerLine.substr(keyStart, keyEnd);

		int valueStart = lowerLine.find_first_not_of(CONFIG_WHITESPACES, keyEnd + 1);
		if (valueStart == string::npos || valueStart == line.length() + 1) continue;
		value = line.substr(valueStart, string::npos); //Tak aby nie bylo juz tylko lowercase

		AddKeyValueToMap(key, value);
	}

	return true;
}

bool ConfigFile::Load(string fileName)
{
	ifstream file;
	file.open(fileName, ifstream::in);
	bool ret = Load(file);

	if (file.is_open()) file.close();
	return ret;
}

bool ConfigFile::Load()
{
	return Load(fileName);
}

bool ConfigFile::Save(bool sortAlphabetically)
{
	if (fileName.size() == 0) return false;
	return Save(fileName, sortAlphabetically);
}

bool ConfigFile::Save(ofstream& file, bool sortAlphabetically)
{
	if (!file.is_open())
		return false;

	if (sortAlphabetically)
	{
		for (auto it : configMap)
		{
			file << it.first << " " << it.second << endl;
		}
	}
	else
	{
		for (auto key : orderVector)
		{
			file << key << " " << configMap[key] << endl;
		}
	}


	return true;
}

bool ConfigFile::Save(string fileName, bool sortAlphabetically)
{
	ofstream file;
	file.open(fileName, ifstream::trunc);
	bool ret = Save(file, sortAlphabetically);

	if (file.is_open()) file.close();
	return ret;
}

//Getters
string ConfigFile::GetString(string key, string noKeyValue)
{
	try
	{
		return configMap.at(key);
	}
	catch (...)
	{
		SetString(key, noKeyValue);
		return noKeyValue;
	}
}

bool ConfigFile::GetBool(string key, bool noKeyValue)
{
	try
	{
		string val = configMap.at(key);
		if (val == "1" || val == "true") return true;
		else if (val == "0" || val == "false") return false;

		SetBool(key, noKeyValue);
		return noKeyValue;
	}
	catch (...)
	{
		SetBool(key, noKeyValue);
		return noKeyValue;
	}
}

int ConfigFile::GetInt(string key, int noKeyValue)
{
	try
	{
		string val = configMap.at(key);
		int retVal = stoi(val);
		return retVal;
	}
	catch (...)
	{
		SetInt(key, noKeyValue);
		return noKeyValue;
	}
}

unsigned int ConfigFile::GetUInt(string key, int noKeyValue)
{
	try
	{
		string val = configMap.at(key);
		unsigned int retVal = stoul(val);
		return retVal;
	}
	catch (...)
	{
		SetUInt(key, noKeyValue);
		return noKeyValue;
	}
}

__int64 ConfigFile::GetInt64(string key, __int64 noKeyValue)
{
	try
	{
		string val = configMap.at(key);
		__int64 retVal = _strtoi64(val.c_str(), nullptr, 0);
		return retVal;
	}
	catch (...)
	{
		SetInt64(key, noKeyValue);
		return noKeyValue;
	}
}

float ConfigFile::GetFloat(string key, float noKeyValue)
{
	try
	{
		string val = configMap.at(key);
		float retVal = stof(val);
		return retVal;
	}
	catch (...)
	{
		SetFloat(key, noKeyValue);
		return noKeyValue;
	}
}

double ConfigFile::GetDouble(string key, double noKeyValue)
{
	try
	{
		string val = configMap.at(key);
		double retVal = stod(val);
		return retVal;
	}
	catch (...)
	{
		SetDouble(key, noKeyValue);
		return noKeyValue;
	}
}

unsigned int ConfigFile::GetPtr32(string key, unsigned int noKeyValue)
{
	try
	{
		string val = configMap.at(key);
		unsigned int retVal = stoul(val);
		return retVal;
	}
	catch (...)
	{
		SetPtr32(key, noKeyValue);
		return noKeyValue;
	}
}

unsigned __int64 ConfigFile::GetPtr64(string key, unsigned __int64 noKeyValue)
{
	try
	{
		string val = configMap.at(key);
		unsigned __int64 retVal = _strtoui64(val.c_str(), nullptr, 0);
		return retVal;
	}
	catch (...)
	{
		SetPtr64(key, noKeyValue);
		return noKeyValue;
	}
}

unsigned int ConfigFile::GetVKCode(string key, unsigned int noKeyValue)
{
	try
	{
		string val = configMap.at(key);
		transform(val.begin(), val.end(), val.begin(), ::tolower);
		unsigned int vkCode = GetKeyMap()->at(val);
		return vkCode;
	}
	catch (...)
	{
		SetVKCode(key, noKeyValue);
		return noKeyValue;
	}
}

//Setters
void ConfigFile::SetString(string key, string value)
{
	AddKeyValueToMap(key, value);
}

void ConfigFile::SetBool(string key, bool value)
{
	AddKeyValueToMap(key, value ? "true" : "false");
}

void ConfigFile::SetInt(string key, int value)
{
	AddKeyValueToMap(key, to_string(value));
}

void ConfigFile::SetUInt(string key, unsigned int value)
{
	AddKeyValueToMap(key, to_string(value));
}

void ConfigFile::SetInt64(string key, __int64 value)
{
	char* buffer = new char[17];
	_i64toa_s(value, buffer, 17, 10);
	string number(buffer);
	AddKeyValueToMap(key, number);
	delete[] buffer;
}

void ConfigFile::SetFloat(string key, float value)
{
	stringstream ss;
	ss << setprecision(numeric_limits<float>::digits10) << value;
	AddKeyValueToMap(key, ss.str());
}

void ConfigFile::SetDouble(string key, double value)
{
	stringstream ss;
	ss << setprecision(numeric_limits<double>::digits10) << value;
	AddKeyValueToMap(key, ss.str());
}

void ConfigFile::SetPtr32(string key, unsigned int value)
{
	ostringstream oss;
	oss << ios::hex << ios::uppercase << value;
	AddKeyValueToMap(key, oss.str());
}

void ConfigFile::SetPtr64(string key, unsigned __int64 value)
{
	char* buffer = new char[17];
	_ui64toa_s(value, buffer, 17, 16);
	string number("0x");
	number.append(buffer);
	AddKeyValueToMap(key, number);
	delete[] buffer;
}

