// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <Psapi.h>

struct Tarray {
	ULONG_PTR adress;
	int count;
	int max;
};

struct FCarouselPirateDesc
{
	int                                                Seed;                                                     // 0x0000(0x0004) (Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData)
	char                            Gender;																		// 0x0004(0x0001) (Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData)
	char                         Ethnicity;																		// 0x0005(0x0001) (Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData)
	char                                      UnknownData00[0x2];												// 0x0006(0x0002) MISSED OFFSET
	float                                              Age;                                                      // 0x0008(0x0004) (Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData)
	float                                              RadialDistance;
	float                                              NormalizedAngle;
	float                                              Dirtiness;                                                // 0x0014(0x0004) (Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData)
	float                                              Wonkiness;                                                // 0x0018(0x0004) (Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData)
	unsigned char                                      UnknownData01[0x4];                                       // 0x001C(0x0004) MISSED OFFSET
	unsigned char                              Outfit[0x8];                                                   // 0x0020(0x0010) (Edit, BlueprintVisible, ZeroConstructor)
};

//main hack thread created on the DllMain
void hackThread(HMODULE hModule) {

	//We create a console instance, we don't need this, but it's useful for displaying things
	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	//We get the memory adress of the LineUp_UI generator and add it by 0x3E8 to get the TArray of pirate structs
	//We typecast it
	auto carouselTArray = *(Tarray*)(0x000001C11407E8B0 + 0x3E8);
	
	std::cout << carouselTArray.adress << std::endl;
	std::cout << carouselTArray.count << std::endl;

	while (!GetAsyncKeyState(VK_END)) {
		for (int i = 0; i < carouselTArray.count; i++) {
			auto pirateDesc = (FCarouselPirateDesc*)(carouselTArray.adress + i * 0x30);
			
			pirateDesc->Wonkiness = 0.0f;
			pirateDesc->Dirtiness = 0.0f;
			//pirateDesc->Gender = 1U;
			pirateDesc->Age = 0.0f;
			pirateDesc->NormalizedAngle = 1.0f; // backwards


			pirateDesc->RadialDistance = 0.5f;
			
		}
	}
	std::cout << "Exit" << std::endl;
	Sleep(100);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
}

//Used to attach the dll to the process we inject to
BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		HANDLE handle = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)hackThread, hModule, NULL, NULL);
		break;
	}
	return 1;
}

