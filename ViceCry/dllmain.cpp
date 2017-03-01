#include "StdAfx.h"

#include "MovingThings.h"
#include "Renderer.h"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	UNREFERENCED_PARAMETER(hinstDLL);
	UNREFERENCED_PARAMETER(lpvReserved);

	if ( fdwReason == DLL_PROCESS_ATTACH )
	{
		using namespace Memory::VP;

//		InjectHook(0x54FA87, MovingThingsInit, PATCH_JUMP);
		InjectHook(0x54FA87, CMovingThings::Init, PATCH_CALL);
		InjectHook(0x54FA8C, 0x54FB87, PATCH_JUMP);
		InjectHook(0x54FC6A, CMovingThings::PostInit, PATCH_JUMP);

		InjectHook(0x54F5C0, CMovingThings::PossiblyAddThisEntity, PATCH_JUMP);
		InjectHook(0x54F453, CMovingThings::Update, PATCH_CALL);
		InjectHook(0x54F458, 0x54F4E6, PATCH_JUMP);

		// Rendering lists
		Patch<BYTE>(0x4C7613, 0x55);
		Patch<BYTE>(0x4C77ED, 0x55);
		Patch<BYTE>(0x4C85AD, 0x56);
		Patch<WORD>(0x4C7619, 0xC483);
		Patch<WORD>(0x4C77F3, 0xC483);
		Patch<BYTE>(0x4C761B, 0x04);
		Patch<BYTE>(0x4C77F5, 0x04);
		Patch<DWORD>(0x4C85B3, 0xEB04C483);
		Patch<BYTE>(0x4C85B7, 0x07); 
		InjectHook(0x4C7614, CRenderer::AddVisibleEntity, PATCH_CALL);
		InjectHook(0x4C77EE, CRenderer::AddVisibleEntity, PATCH_CALL);
		InjectHook(0x4C85AE, CRenderer::AddVisibleEntity, PATCH_CALL);
		InjectHook(0x4C761C, 0x4C7714, PATCH_JUMP);
		InjectHook(0x4C77F6, 0x4C78F8, PATCH_JUMP);

		// Alpha entity list
		//Patch<DWORD>(0x5828DC, 500);
	}
	return TRUE;
};