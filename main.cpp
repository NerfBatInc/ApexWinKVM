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
float glowcolor[3] = { 000.0f, 000.0f, 000.0f };
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



//Rand smoothing
int randomstuff = 0;




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

		//run load settings once 

		for (static bool once = true; once; once = false) {
			std::ifstream config("Config.txt");

			if (config.is_open())
			{


				config >> std::boolalpha >> firing_range;
				config >> aim;
				config >> std::boolalpha >> esp;
				config >> std::boolalpha >> item_glow;
				config >> std::boolalpha >> player_glow;
				config >> std::boolalpha >> aim_no_recoil;
				config >> max_dist;
				config >> smooth;
				config >> max_fov;
				config >> bone;
				config >> glowr;
				config >> glowg;
				config >> glowb;
				config >> glowtype;
				config >> glowtype2;
				config >> glowcolor[0];
				config >> glowcolor[1];
				config >> glowcolor[2];
				//config >> item_current; // no idea how to imput a string of words 


				config.close();
			}
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
		system(XorStr("taskkill /F /T /IM Nvspcaps64.exe")); //custom overlay process name
	return 0;
}



