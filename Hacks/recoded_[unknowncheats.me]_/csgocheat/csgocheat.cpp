#include "stdafx.h"
#include "Tools/Hacks/Misc.h"
#include "Hooks.h"

bool Unhook_Cheat = false;
auto ExitCheat() -> bool;

void Unhook()
{
	Unhook_Cheat = true;
}
typedef void(__cdecl* MsgFn)(const char* msg, va_list);

void Msg3(const char* msg, ...)
{

	if (msg == nullptr)
		return; //If no string was passed, or it was null then don't do anything
	static MsgFn fn = (MsgFn)GetProcAddress(GetModuleHandle("tier0.dll"), "Msg"); //This gets the address of export "Msg" in the dll "tier0.dll". The static keyword means it's only called once and then isn't called again (but the variable is still there)
	char buffer[989];
	va_list list; //Normal varargs stuff http://stackoverflow.com/questions/10482960/varargs-to-printf-all-arguments
	va_start(list, msg);

	vsprintf(buffer, msg, list);
	va_end(list);

	fn(buffer, list); //Calls the function, we got the address above.
}

#define USERS 1823381277
#define USERS2 1213810804
#define USERS3 814905203
#define USERS4 1372684328
#define USERS5 1733124959
#define SEQUENCE_DEFAULT_DRAW 0
#define SEQUENCE_DEFAULT_IDLE1 1
#define SEQUENCE_DEFAULT_IDLE2 2
#define SEQUENCE_DEFAULT_LIGHT_MISS1 3
#define SEQUENCE_DEFAULT_LIGHT_MISS2 4
#define SEQUENCE_DEFAULT_HEAVY_MISS1 9
#define SEQUENCE_DEFAULT_HEAVY_HIT1 10
#define SEQUENCE_DEFAULT_HEAVY_BACKSTAB 11
#define SEQUENCE_DEFAULT_LOOKAT01 12

#define SEQUENCE_BUTTERFLY_DRAW 0
#define SEQUENCE_BUTTERFLY_DRAW2 1
#define SEQUENCE_BUTTERFLY_LOOKAT01 13
#define SEQUENCE_BUTTERFLY_LOOKAT03 15

#define SEQUENCE_FALCHION_IDLE1 1
#define SEQUENCE_FALCHION_HEAVY_MISS1 8
#define SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP 9
#define SEQUENCE_FALCHION_LOOKAT01 12
#define SEQUENCE_FALCHION_LOOKAT02 13

#define SEQUENCE_DAGGERS_IDLE1 1
#define SEQUENCE_DAGGERS_LIGHT_MISS1 2
#define SEQUENCE_DAGGERS_LIGHT_MISS5 6
#define SEQUENCE_DAGGERS_HEAVY_MISS2 11
#define SEQUENCE_DAGGERS_HEAVY_MISS1 12

#define SEQUENCE_BOWIE_IDLE1 1
using namespace std;
// Helper function to generate a random sequence number.
inline int RandomSequence(int low, int high) {
	return (rand() % (high - low + 1) + low);
}

#define	LIFE_ALIVE 0

#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);



RecvVarProxyFn fnSequenceProxyFn = nullptr;

RecvVarProxyFn oRecvnModelIndex;
void Hooked_RecvProxy_Viewmodel(CRecvProxyData *pData, void *pStruct, void *pOut)
{
	// Get the knife view model id's
	int default_t = Interfaces.g_pModelInfo->GetModelIndex("models/weapons/v_knife_default_t.mdl");
	int default_ct = Interfaces.g_pModelInfo->GetModelIndex("models/weapons/v_knife_default_ct.mdl");
	int iBayonet = Interfaces.g_pModelInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
	int iButterfly = Interfaces.g_pModelInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
	int iFlip = Interfaces.g_pModelInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
	int iGut = Interfaces.g_pModelInfo->GetModelIndex("models/weapons/v_knife_gut.mdl");
	int iKarambit = Interfaces.g_pModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
	int iM9Bayonet = Interfaces.g_pModelInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
	int iHuntsman = Interfaces.g_pModelInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
	int iFalchion = Interfaces.g_pModelInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
	int iDagger = Interfaces.g_pModelInfo->GetModelIndex("models/weapons/v_knife_push.mdl");
	int iBowie = Interfaces.g_pModelInfo->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");

	int iGunGame = Interfaces.g_pModelInfo->GetModelIndex("models/weapons/v_knife_gg.mdl");

	// Get local player (just to stop replacing spectators knifes)
	CBaseEntity* pLocal = Interfaces.pEntList->GetClientEntity(Interfaces.pEngine->GetLocalPlayer());
	if (Options.Skins.knife && pLocal)
	{
		int KnifeModel = Options.Skins.Knifemodels;

		// If we are alive and holding a default knife(if we already have a knife don't worry about changing)
		if (pLocal->isAlive() && (
			pData->m_Value.m_Int == default_t ||
			pData->m_Value.m_Int == default_ct ||
			pData->m_Value.m_Int == iBayonet ||
			pData->m_Value.m_Int == iFlip ||
			pData->m_Value.m_Int == iGunGame ||
			pData->m_Value.m_Int == iGut ||
			pData->m_Value.m_Int == iKarambit ||
			pData->m_Value.m_Int == iM9Bayonet ||
			pData->m_Value.m_Int == iHuntsman ||
			pData->m_Value.m_Int == iBowie ||
			pData->m_Value.m_Int == iButterfly ||
			pData->m_Value.m_Int == iFalchion ||
			pData->m_Value.m_Int == iDagger))
		{
			// Set whatever knife we want
			if (KnifeModel == 0)
				pData->m_Value.m_Int = iBayonet;
			else if (KnifeModel == 1)
				pData->m_Value.m_Int = iBowie;
			else if (KnifeModel == 2)
				pData->m_Value.m_Int = iButterfly;
			else if (KnifeModel == 3)
				pData->m_Value.m_Int = iFalchion;
			else if (KnifeModel == 4)
				pData->m_Value.m_Int = iFlip;
			else if (KnifeModel == 5)
				pData->m_Value.m_Int = iGut;
			else if (KnifeModel == 6)
				pData->m_Value.m_Int = iHuntsman;
			else if (KnifeModel == 7)
				pData->m_Value.m_Int = iKarambit;
			else if (KnifeModel == 8)
				pData->m_Value.m_Int = iM9Bayonet;
			else if (KnifeModel == 9)
				pData->m_Value.m_Int = iDagger;

		}
	}
	// Carry on the to original proxy
	oRecvnModelIndex(pData, pStruct, pOut);
}
void NetvarHook()
{
	ClientClass *pClass = Interfaces.pClient->GetAllClasses();
	while (pClass)
	{
		const char *pszName = pClass->m_pRecvTable->m_pNetTableName;

		 if (!strcmp(pszName, "DT_BaseViewModel"))
		{
			for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
			{
				RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);

				const char *name = pProp->m_pVarName;
				if (!strcmp(name, "m_nModelIndex"))
				{
					oRecvnModelIndex = (RecvVarProxyFn)pProp->m_ProxyFn;
					pProp->m_ProxyFn = Hooked_RecvProxy_Viewmodel;
				}
			}
		}
		pClass = pClass->m_pNext;
	}
}

extern HINSTANCE hAppInstance;

UCHAR szFileSys[255], szVolNameBuff[255];
DWORD dwMFL, dwSysFlags;
DWORD dwSerial;
LPCTSTR szHD = "C:\\";

HINSTANCE HThisModule;

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		GetVolumeInformation(szHD, (LPTSTR)szVolNameBuff, 255, &dwSerial, &dwMFL, &dwSysFlags, (LPTSTR)szFileSys, 255);
		
		//CfgManager::SetupPath();

		while (!(INIT::Window = FindWindowA("Valve001", NULL)))
			Sleep(200);

		INIT::OldWindow = (WNDPROC)SetWindowLongPtr(INIT::Window, GWL_WNDPROC, (LONG_PTR)Hooked_WndProc);

		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)(CheatMain), nullptr, 0, nullptr);

		INIT::Dll = hModule;

		return TRUE;



		{
			{
				DisableThreadLibraryCalls(hModule);

				CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CheatMain, NULL, NULL, NULL);

				return TRUE;
			}
		}
		return FALSE;
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
	}
	return TRUE;
}

bool ExitCheat()
{
	if( !Unhook_Cheat )
		return false;

	Interfaces.pEngine->ClientCmd_Unrestricted( "clear", 0 );
	Interfaces.pEngine->ClientCmd_Unrestricted( "echo Astrium has Exited", 0 );

	Interfaces.pEngine->ClientCmd_Unrestricted( "cl_mouseenable 1", 0 );
	Sleep( 100 );
	Interfaces.pEngine->ClientCmd_Unrestricted( "crosshair 1", 0 );
	Sleep( 100 );
	Interfaces.pEngine->ClientCmd_Unrestricted( "firstperson", 0 );
	Sleep( 100 );
	Interfaces.pEngine->ClientCmd_Unrestricted( "cl_drawhud 1", 0 );
	Sleep( 100 );

	if( Interfaces.pEngine->IsInGame() && Interfaces.pEngine->IsConnected() )
	{
		Misc::SetClanTag( "", Hacks.LocalPlayer->GetName().c_str() );
		Sleep( 100 );
	}

	Hacks.UnHook();
	Sleep( 200 );

	SetWindowLongPtr( INIT::Window, GWL_WNDPROC, reinterpret_cast< LONG_PTR >( INIT::OldWindow ) );
	Sleep( 200 );

	FreeLibraryAndExitThread( INIT::Dll, 0 );
}

void WINAPI CheatMain()
{
	//AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	SetConsoleTitle("floxware console");
	Interfaces.InitialiseSDK();

	auto BuildNumber = Interfaces.pEngine->GetEngineBuildNumber();

	CNetvarManager::Instance()->CreateDatabase();
	Hacks.Hook();

	if (BuildNumber != 13624)
	{
		Interfaces.pEngine->ClientCmd_Unrestricted("cl_mouseenable 1", 0);
		Interfaces.pEngine->ClientCmd_Unrestricted("crosshair 1", 0);
		Interfaces.pEngine->ClientCmd_Unrestricted("clear", 0);		

		
		
		Hacks.UnHook();
		Sleep(200);
		MessageBox(0, "Cheat is outdate right now", "floxware", MB_OK);
		SetWindowLongPtr(INIT::Window, GWL_WNDPROC, reinterpret_cast< LONG_PTR >(INIT::OldWindow));
		Sleep(200);

		FreeLibraryAndExitThread(INIT::Dll, 0);
	}
	else
	{
		Sleep(100);
		Interfaces.pEngine->ClientCmd_Unrestricted("crosshair 1", 0);

		Interfaces.pEngine->ClientCmd_Unrestricted("developer 1 ", 0);
		Sleep(0);

		Interfaces.pEngine->ClientCmd_Unrestricted("con_filter_enable 2 ", 0);
		Sleep(0);


		Interfaces.pEngine->ClientCmd_Unrestricted("con_filter_text (floxware) ", 0);
		Sleep(0);
		std::string before = ("(floxware) Welcome. Dev build");
		std::string newline = "\n";

		Msg3((before + newline).c_str());

		NetvarHook();
		while (!Unhook_Cheat)
		{
			Misc::ServerRankReveal();
			Sleep(15);
		}

		ExitCheat();
	}
	return;


}

