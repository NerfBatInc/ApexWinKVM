#include "main.h"

typedef struct player
{
	float dist = 0;
	int entity_team = 0;
	float boxMiddle = 0;
	float h_y = 0;
	float width = 0;
	float height = 0;
	float b_x = 0;
	float b_y = 0;
	bool knocked = false;
	bool visible = false;
	int health = 0;
	int shield = 0;
	//seer
	int maxshield = 0;
	int armortype = 0;
	char name[33] = { 0 };
}player;

uint32_t check = 0xABCD;






int aim_key = VK_RBUTTON;
int aim_key2 = VK_LBUTTON;
int shoot_key = VK_LBUTTON;
int shoot_key2 = VK_RBUTTON;



bool firing_range = false;
bool use_nvidia = true;
bool active = true;
bool ready = false;
extern visuals v;
int aim = 2; //read
bool esp = true; //read
bool item_glow = true;
bool player_glow = true;
bool aim_no_recoil = true;
bool aiming = false; //read
uint64_t g_Base = 0; //write
float max_dist = 3800.0f * 40.0f; //read
float smooth = 100.0f;
float max_fov = 15.0f;
int bone = 2;
//glow color and brightness
float glowr = 0.0f;
float glowg = 120.0f;
float glowb = 120.0f;
int glowtype = 1;
int glowtype2 = 2;


bool thirdperson = false;
int spectators = 0; //write
int allied_spectators = 0; //write

//chargerifle hack

bool chargerifle = false;
bool shooting = false; //read

bool valid = false; //write
bool next2 = false; //read write

uint64_t add[27];

bool k_f5 = 0;
bool k_f6 = 0;
bool k_f7 = 0;
bool k_f8 = 0;
bool k_f9 = 0;
bool k_f10 = 0;

bool k_f20 = 0;

bool k_f100 = 0;

//save and load testing

//Rand smoothing
int randomstuff = 0;

//Save and Load void


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

void ConfigFile::SetVKCode(string key, unsigned int value)
{
	try
	{
		string keyName = GetVKMap()->at(value);
		AddKeyValueToMap(key, keyName);
	}
	catch (...)
	{
		AddKeyValueToMap(key, "KEY DOES NOT EXIST");
	}
}

//Keymap
unordered_map<string, unsigned int>* ConfigFile::GetKeyMap()
{
	static unordered_map<string, unsigned int>* keyMap = nullptr;

	if (!keyMap)
	{
		keyMap = new unordered_map<string, unsigned int>;
		(*keyMap)["none"] = 0x00;
		(*keyMap)["lbutton"] = 0x01;
		(*keyMap)["rbutton"] = 0x02;
		(*keyMap)["cancel"] = 0x03;
		(*keyMap)["mbutton"] = 0x04;
		(*keyMap)["xbutton1"] = 0x05;
		(*keyMap)["xbutton2"] = 0x06;
		(*keyMap)["back"] = 0x08;
		(*keyMap)["tab"] = 0x09;
		(*keyMap)["clear"] = 0x0c;
		(*keyMap)["return"] = 0x0d;
		(*keyMap)["shift"] = 0x10;
		(*keyMap)["control"] = 0x11;
		(*keyMap)["menu"] = 0x12;
		(*keyMap)["pause"] = 0x13;
		(*keyMap)["capital"] = 0x14;
		(*keyMap)["hangul"] = 0x15;
		(*keyMap)["junja"] = 0x17;
		(*keyMap)["final"] = 0x18;
		(*keyMap)["hanja"] = 0x19;
		(*keyMap)["escape"] = 0x1b;
		(*keyMap)["convert"] = 0x1c;
		(*keyMap)["nonconvert"] = 0x1d;
		(*keyMap)["accept"] = 0x1e;
		(*keyMap)["modechange"] = 0x1f;
		(*keyMap)["space"] = 0x20;
		(*keyMap)["prior"] = 0x21;
		(*keyMap)["next"] = 0x22;
		(*keyMap)["end"] = 0x23;
		(*keyMap)["home"] = 0x24;
		(*keyMap)["left"] = 0x25;
		(*keyMap)["up"] = 0x26;
		(*keyMap)["right"] = 0x27;
		(*keyMap)["down"] = 0x28;
		(*keyMap)["select"] = 0x29;
		(*keyMap)["print"] = 0x2a;
		(*keyMap)["execute"] = 0x2b;
		(*keyMap)["snapshot"] = 0x2c;
		(*keyMap)["insert"] = 0x2d;
		(*keyMap)["delete"] = 0x2e;
		(*keyMap)["help"] = 0x2f;
		(*keyMap)["0"] = 0x30;
		(*keyMap)["1"] = 0x31;
		(*keyMap)["2"] = 0x32;
		(*keyMap)["3"] = 0x33;
		(*keyMap)["4"] = 0x34;
		(*keyMap)["5"] = 0x35;
		(*keyMap)["6"] = 0x36;
		(*keyMap)["7"] = 0x37;
		(*keyMap)["8"] = 0x38;
		(*keyMap)["9"] = 0x39;
		(*keyMap)["a"] = 0x41;
		(*keyMap)["b"] = 0x42;
		(*keyMap)["c"] = 0x43;
		(*keyMap)["d"] = 0x44;
		(*keyMap)["e"] = 0x45;
		(*keyMap)["f"] = 0x46;
		(*keyMap)["g"] = 0x47;
		(*keyMap)["h"] = 0x48;
		(*keyMap)["i"] = 0x49;
		(*keyMap)["j"] = 0x4a;
		(*keyMap)["k"] = 0x4b;
		(*keyMap)["l"] = 0x4c;
		(*keyMap)["m"] = 0x4d;
		(*keyMap)["n"] = 0x4e;
		(*keyMap)["o"] = 0x4f;
		(*keyMap)["p"] = 0x50;
		(*keyMap)["q"] = 0x51;
		(*keyMap)["r"] = 0x52;
		(*keyMap)["s"] = 0x53;
		(*keyMap)["t"] = 0x54;
		(*keyMap)["u"] = 0x55;
		(*keyMap)["v"] = 0x56;
		(*keyMap)["w"] = 0x57;
		(*keyMap)["x"] = 0x58;
		(*keyMap)["y"] = 0x59;
		(*keyMap)["z"] = 0x5a;
		(*keyMap)["lwin"] = 0x5b;
		(*keyMap)["rwin"] = 0x5c;
		(*keyMap)["apps"] = 0x5d;
		(*keyMap)["sleep"] = 0x5f;
		(*keyMap)["numpad0"] = 0x60;
		(*keyMap)["numpad1"] = 0x61;
		(*keyMap)["numpad2"] = 0x62;
		(*keyMap)["numpad3"] = 0x63;
		(*keyMap)["numpad4"] = 0x64;
		(*keyMap)["numpad5"] = 0x65;
		(*keyMap)["numpad6"] = 0x66;
		(*keyMap)["numpad7"] = 0x67;
		(*keyMap)["numpad8"] = 0x68;
		(*keyMap)["numpad9"] = 0x69;
		(*keyMap)["multiply"] = 0x6a;
		(*keyMap)["add"] = 0x6b;
		(*keyMap)["separator"] = 0x6c;
		(*keyMap)["subtract"] = 0x6d;
		(*keyMap)["decimal"] = 0x6e;
		(*keyMap)["divide"] = 0x6f;
		(*keyMap)["f1"] = 0x70;
		(*keyMap)["f2"] = 0x71;
		(*keyMap)["f3"] = 0x72;
		(*keyMap)["f4"] = 0x73;
		(*keyMap)["f5"] = 0x74;
		(*keyMap)["f6"] = 0x75;
		(*keyMap)["f7"] = 0x76;
		(*keyMap)["f8"] = 0x77;
		(*keyMap)["f9"] = 0x78;
		(*keyMap)["f10"] = 0x79;
		(*keyMap)["f11"] = 0x7a;
		(*keyMap)["f12"] = 0x7b;
		(*keyMap)["f13"] = 0x7c;
		(*keyMap)["f14"] = 0x7d;
		(*keyMap)["f15"] = 0x7e;
		(*keyMap)["f16"] = 0x7f;
		(*keyMap)["f17"] = 0x80;
		(*keyMap)["f18"] = 0x81;
		(*keyMap)["f19"] = 0x82;
		(*keyMap)["f20"] = 0x83;
		(*keyMap)["f21"] = 0x84;
		(*keyMap)["f22"] = 0x85;
		(*keyMap)["f23"] = 0x86;
		(*keyMap)["f24"] = 0x87;
		(*keyMap)["numlock"] = 0x90;
		(*keyMap)["scroll"] = 0x91;
		(*keyMap)["lshift"] = 0xa0;
		(*keyMap)["rshift"] = 0xa1;
		(*keyMap)["lcontrol"] = 0xa2;
		(*keyMap)["rcontrol"] = 0xa3;
		(*keyMap)["lmenu"] = 0xa4;
		(*keyMap)["rmenu"] = 0xa5;
		(*keyMap)["browser_back"] = 0xa6;
		(*keyMap)["browser_forward"] = 0xa7;
		(*keyMap)["browser_refresh"] = 0xa8;
		(*keyMap)["browser_stop"] = 0xa9;
		(*keyMap)["browser_search"] = 0xaa;
		(*keyMap)["browser_favorites"] = 0xab;
		(*keyMap)["browser_home"] = 0xac;
		(*keyMap)["volume_mute"] = 0xad;
		(*keyMap)["volume_down"] = 0xae;
		(*keyMap)["volume_up"] = 0xaf;
		(*keyMap)["media_next_track"] = 0xb0;
		(*keyMap)["media_prev_track"] = 0xb1;
		(*keyMap)["media_stop"] = 0xb2;
		(*keyMap)["media_play_pause"] = 0xb3;
		(*keyMap)["launch_mail"] = 0xb4;
		(*keyMap)["launch_media_select"] = 0xb5;
		(*keyMap)["launch_app1"] = 0xb6;
		(*keyMap)["launch_app2"] = 0xb7;
		(*keyMap)["oem_1"] = 0xba;
		(*keyMap)["oem_plus"] = 0xbb;
		(*keyMap)["oem_comma"] = 0xbc;
		(*keyMap)["oem_minus"] = 0xbd;
		(*keyMap)["oem_period"] = 0xbe;
		(*keyMap)["oem_2"] = 0xbf;
		(*keyMap)["oem_3"] = 0xc0;
		(*keyMap)["oem_4"] = 0xdb;
		(*keyMap)["oem_5"] = 0xdc;
		(*keyMap)["oem_6"] = 0xdd;
		(*keyMap)["oem_7"] = 0xde;
		(*keyMap)["oem_8"] = 0xdf;


	}

	return keyMap;
}

unordered_map<unsigned int, string>* ConfigFile::GetVKMap()
{
	static unordered_map<unsigned int, string>* valueMap = nullptr;

	if (!valueMap)
	{
		valueMap = new unordered_map<unsigned int, string>();
		auto keyMap = GetKeyMap();
		for (auto it : *keyMap)
		{
			(*valueMap)[it.second] = it.first;
		}
	}
	return valueMap;
}

bool ConfigFile::AddKeyValueToMap(string key, string value)
{
	auto it = configMap.insert(pair<string, string>(key, value));
	if (it.second) orderVector.push_back(key);
	else (*it.first).second = value;

	return it.second;
}


void configLoad(std::string con)
{
	con.append(".cfg");

	ConfigFile cfg = ConfigFile::ConfigFile(con);

	firing_range = cfg.GetBool("Glow Type", false);
	aim = cfg.GetInt("Glow Type", 2);
	esp = cfg.GetBool("Glow Type", true);
	item_glow = cfg.GetBool("Glow Type", true);
	player_glow = cfg.GetBool("Glow Type", true);
	aim_no_recoil = cfg.GetBool("Glow Type", true);
	max_dist = cfg.GetFloat("Glow Type", 3800.0f * 40.0f);
	smooth = cfg.GetFloat("Glow Type", 100.0f);
	max_fov = cfg.GetFloat("Glow Type", 15.0f);
	bone = cfg.GetInt("Glow Type", 2);
	glowr = cfg.GetFloat("Glow Type", 0.0f);
	glowg = cfg.GetFloat("Glow Type", 120.0f);
	glowb = cfg.GetFloat("Glow Type", 120.0f);
	glowtype = cfg.GetInt("Glow Type", 1);
	glowtype2 = cfg.GetInt("Glow Type", 2);
}

void configSave(std::string con)
{
	con.append(".cfg");

	ConfigFile cfg(con);

	cfg.SetBool("Glow Type", firing_range);
	cfg.SetInt("Glow Type", aim);
	cfg.SetBool("Glow Type", esp);
	cfg.SetBool("Glow Type", item_glow);
	cfg.SetBool("Glow Type", player_glow);
	cfg.SetBool("Glow Type", aim_no_recoil);
	cfg.SetFloat("Glow Type", max_dist);
	cfg.SetFloat("Glow Type", smooth);
	cfg.SetFloat("Glow Type", max_fov);
	cfg.SetInt("Glow Type", bone);
	cfg.SetFloat("Glow Type", glowr);
	cfg.SetFloat("Glow Type", glowg);
	cfg.SetFloat("Glow Type", glowb);
	cfg.SetInt("Glow Type", glowtype);
	cfg.SetInt("Glow Type", glowtype2);
}
//random smoothing from 90 to 149 with 2-3 bone id
void randomBone100() {
	int bonearray[2] = { 2, 3 };
	int randVal = rand() % 2;
	bone = bonearray[randVal];
	int smootharray[139]{ 90.0, 91.0, 92.0, 93.0, 94.0, 95.0, 96.0, 97.0, 98.0, 99.0, 100.0, 101.0, 102.0, 103.0, 104.0, 105.0, 106.0, 107.0, 108.0, 109.0, 110.0, 111.0, 112.0, 113.0, 114.0, 115.0, 116.0, 117.0, 118.0, 119.0, 120.0, 121.0, 122.0, 123.0, 124.0, 125.0, 126.0, 127.0, 128.0, 129.0, 130.0, 131.0, 132.0, 133.0, 134.0, 135.0, 136.0, 137.0, 138.0, 139.0, 140.0, 141.0, 142.0, 143.0, 144.0, 145.0, 146.0, 147.0, 148.0 };
	int randSmooth = rand() % 50;
	smooth = smootharray[randSmooth];
	
}

//random smoothing from 30 to 50 with 1-3 bone id
//void randomBone20() {
//	int bonearray[3] = { 1, 2, 3 };
//	int randVal = rand() % 3;
//	bone = bonearray[randVal];
//	int smootharray[21]{ 30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50 };
//	int randSmooth = rand() % 21;
//	smooth = smootharray[randSmooth];
//
//}



bool IsKeyDown(int vk)
{
	return (GetAsyncKeyState(vk) & 0x8000) != 0;
}

player players[100];






void Overlay::RenderEsp()
{
	next2 = false;
	if (g_Base != 0 && esp)
	{
		memset(players, 0, sizeof(players));
		while (!next2 && esp)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		if (next2 && valid)
		{
			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::SetNextWindowSize(ImVec2((float)getWidth(), (float)getHeight()));
			ImGui::Begin(XorStr("##esp"), (bool*)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus);

			for (int i = 0; i < 100; i++)
			{
				if (players[i].health > 0)
				{
					std::string distance = std::to_string(players[i].dist / 39.62);
					distance = distance.substr(0, distance.find('.')) + "m(" + std::to_string(players[i].entity_team) + ")";
					if (v.box)
					{
						if (players[i].visible)
						{
							if (players[i].dist < 1600.0f)
								DrawBox(RED, players[i].boxMiddle, players[i].h_y, players[i].width, players[i].height); //BOX
							else
								DrawBox(ORANGE, players[i].boxMiddle, players[i].h_y, players[i].width, players[i].height); //BOX
						}
						else
						{
							DrawBox(WHITE, players[i].boxMiddle, players[i].h_y, players[i].width, players[i].height); //white if player not visible
						}
					}

					if(v.line)
						DrawLine(ImVec2((float)(getWidth() / 2), (float)getHeight()), ImVec2(players[i].b_x, players[i].b_y), BLUE, 1); //LINE FROM MIDDLE SCREEN

					if (v.distance)
					{
						if (players[i].knocked)
							String(ImVec2(players[i].boxMiddle, (players[i].b_y + 1)), RED, distance.c_str());  //DISTANCEs			else
							String(ImVec2(players[i].boxMiddle, (players[i].b_y + 1)), GREEN, distance.c_str());  //DISTANCE
					}

					if(v.healthbar)
						if (players[i].dist < 16000.0f)
						{

							DrawSeerLikeHealth((players[i].b_x - (players[i].width / 2.0f) + 5), (players[i].b_y - players[i].height - 30), players[i].shield, players[i].maxshield, players[i].armortype, players[i].health); //health bar					
						}
					//if(v.name)
						//String(ImVec2(players[i].boxMiddle, (players[i].b_y - players[i].height - 15)), WHITE, players[i].name);


				}
			}

			//FOV




			ImGui::End();
		}
	}
}

int main(int argc, char** argv)
{
	add[0] = (uintptr_t)&check;
	add[1] = (uintptr_t)&aim;
	add[2] = (uintptr_t)&esp;
	add[3] = (uintptr_t)&aiming;
	add[4] = (uintptr_t)&g_Base;
	add[5] = (uintptr_t)&next2;
	add[6] = (uintptr_t)&players[0];
	add[7] = (uintptr_t)&valid;
	add[8] = (uintptr_t)&max_dist;
	add[9] = (uintptr_t)&item_glow;
	add[10] = (uintptr_t)&player_glow;
	add[11] = (uintptr_t)&aim_no_recoil;
	add[12] = (uintptr_t)&smooth;
	add[13] = (uintptr_t)&max_fov;
	add[14] = (uintptr_t)&bone;
	add[15] = (uintptr_t)&thirdperson;
	add[16] = (uintptr_t)&spectators;
	add[17] = (uintptr_t)&allied_spectators;
	add[18] = (uintptr_t)&chargerifle;
	add[19] = (uintptr_t)&shooting;
	add[20] = (uintptr_t)&glowr;
	add[21] = (uintptr_t)&glowg;
	add[22] = (uintptr_t)&glowb;
	add[23] = (uintptr_t)&firing_range;
	add[24] = (uintptr_t)&glowtype;
	add[25] = (uintptr_t)&glowtype2;
	printf(XorStr("Game Version 3.0.10.29 |-| Firing Range Ver |-| Add me offset: 0x%I64x\n"), (uint64_t)&add[0] - (uint64_t)GetModuleHandle(NULL));

	Overlay ov1 = Overlay();
	ov1.Start();
	printf(XorStr("Waiting for chestburster ....The Monkey is dead!\n"));
	while (check == 0xABCD)
	{
		if (IsKeyDown(VK_F4))
		{
			active = false;
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	if (active)
	{
		ready = true;
		printf(XorStr("Ready To Cure\n"));
	}
		
	while (active)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		if (IsKeyDown(VK_F4))
		{
			active = false;
		}




		if (IsKeyDown(VK_F5))
			chargerifle = true;

		if (IsKeyDown(VK_F6) && k_f6 == 0)
		{
			k_f6 = 1;
			switch (aim)
			{
			case 0:
				aim = 1;
				break;
			case 1:
				aim = 2;
				break;
			case 2:
				aim = 0;
				break;
			default:
				break;
			}
		}
		else if (!IsKeyDown(VK_F6) && k_f6 == 1)
		{
			k_f6 = 0;
		}

	

		if (IsKeyDown(VK_F9) && k_f100 == 1)
		{
			k_f100 = 0;
			k_f20 = 1;
			k_f8 = 1;
		}
		else  if (IsKeyDown(VK_F10) && k_f100 == 0)
		{
			k_f20 = 0;
			k_f100 = 1;
			k_f8 = 1;
			
		}




		if (k_f100 == 1)
			randomBone100(); //function call
		if (k_f20 == 1)
			randomBone100(); //function call


		if (IsKeyDown(VK_F11))
		{
			k_f20 = 0;
			k_f100 = 0;
			bone = 1;
			smooth = 100;
			
		}




		if (IsKeyDown(aim_key))
		
			aiming = true;


		else if (IsKeyDown(aim_key2))
			aiming = true;
		else
			aiming = false;

		if (IsKeyDown(shoot_key))
		{
			shooting = true;
			
		}

		if (IsKeyDown(shoot_key2))
		{
			shooting = true;
			
		}


		
	
		

	}
	ready = false;
	ov1.Clear();
	if(!use_nvidia)
		system(XorStr("taskkill /F /T /IM MonkeyCure.exe")); //custom overlay process name
	return 0;
}



