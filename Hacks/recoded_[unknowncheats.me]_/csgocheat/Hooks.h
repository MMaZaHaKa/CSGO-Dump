
#pragma once
#include "IniReader.h"
#include "IniWriter.h"
#include <vector>
#include <algorithm>
#include "stdafx.h"
#include "resource.h"
#include "SDK/SDK Headers/CGameEvent.h"
#include <d3d9.h>
#include "Globals.h"
#include "stdint.h"
#include "Config.h"
#include "Chams.h"

//#include "Config.h

#include "Options.h"


//cOptions Options; 
//class CL_FullUpdate_t;
struct INetMessage {
	virtual ~INetMessage();
};

template<typename T>
class CNetMessagePB : public INetMessage, public T {};

class CCLCMsg_Move {
private:
	char __PAD0[0x8];
public:
	int numBackupCommands;
	int numNewCommands;
};

class IBaseClientDll
{
public:
	void CL_SendMove()
	{
		typedef void(__fastcall* CL_SendMoveFn)(void);
		static CL_SendMoveFn CL_SendMove = reinterpret_cast<CL_SendMoveFn>(Utils.FindSig("engine.dll", "55 8B EC A1 ? ? ? ? 81 EC ? ? ? ? B9 ? ? ? ? 53 8B 98"));

		CL_SendMove();
	}

	void WriteUsercmd(void* buf, CInput::CUserCmd* in, CInput::CUserCmd* out)
	{
		typedef void(__fastcall* WriteUsercmdFn)(void*, CInput::CUserCmd*, CInput::CUserCmd*);
		static WriteUsercmdFn WriteUsercmd = reinterpret_cast<WriteUsercmdFn>(Utils.FindSig("client.dll", "55 8B EC 83 E4 F8 51 53 56 8B D9 8B 0D"));

		WriteUsercmd(buf, in, out);
	}
};
using CCLCMsg_Move_t = CNetMessagePB<CCLCMsg_Move>;

void __fastcall Hooked_Createmove( CInput* thisptr, void* _EAX, int sequence_number, float input_sample_frametime, bool active );
void __stdcall Hooked_PaintTraverse( unsigned int vguiPanel, bool forceRepaint, bool allowForce);
void __stdcall Hooked_Frame_Stage_Notify( ClientFrameStage_t curStage );
void __stdcall Hooked_OverrideView( CViewSetup* pSetup );
float __stdcall Hooked_ViewmodelFOV();
HRESULT __stdcall Hooked_Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresParam);
HRESULT __stdcall Hooked_EndScene(IDirect3DDevice9* pDevice);
LRESULT __stdcall Hooked_WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
void __fastcall Hooked_DrawModelExecute( void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld );
void __stdcall Hooked_ClientCmdUnrestricted( const char* szCmdString, char flag );
int  __stdcall Hooked_DoPostScreenEffects(int a1);
void __stdcall Hooked_PlaySound( const char* pSample );
void __fastcall  hkSceneEnd(void *pEcx, void *pEdx);
bool __fastcall WriteUsercmdDeltaToBuffer_h(IBaseClientDll *ECX, void *EDX, int nSlot, bf_write *buf, int from, int to, bool isNewCmd);
void Init();
void DoEnemyCircle2(CBaseEntity* pLocalPlayer, const Vector &vecDelta, float *flRotation);
//void __stdcall Hooked_OverrideMouseInput(float *x, float* y);

extern VTHookManager VMTClientMode;
extern VTHookManager VMTGameEvent;
struct lbyRecords
{
	int tick_count;
	float lby;
	Vector headPosition;
};
struct backtrackData
{
	float simtime;
	Vector hitboxPos;
};

class BackTrack
{
	int latest_tick;
	bool IsTickValid(int tick);
	bool Lowerbody(int tick);
	void UpdateRecord(int i);
	void pejacofabacksadahdghasda();
public:
	lbyRecords records[64];
	bool RunLBYBackTrack(int i, CInput::CUserCmd* cmd, Vector& aimPoint);
	void Update(int tick_count);
	void legitBackTrack();
	void Rageback();

};
struct RageBackTrackData
{
	float simtime;
	float flcycle[13];
	float flprevcycle[13];
	float flweight[13];
	float flweightdatarate[13];
	float yaw;
	uint32_t norder[13];
	Vector hitboxPos;
	int tick;
	int balanceadjusttick;
	float balanceadjustsimtime;
	int activity[13];
	float damage;
};
extern backtrackData headPositions[64][12];
extern BackTrack* backtracking;
extern RageBackTrackData RageBackData[64][12];


class CHackManager
{
private:
	/* Typedefs */
	typedef float(__stdcall *GetViewModelFOVFn)();
	typedef long(__stdcall* tReset)(IDirect3DDevice9* dev, D3DPRESENT_PARAMETERS* pp);
	typedef void (__thiscall *iPaintTraverse)( void*, unsigned int, bool, bool );
	typedef long(__stdcall *EndSceneFn)(IDirect3DDevice9* device);
	typedef void (__stdcall *iCreateMoveFn)( CInput*, void*, int, float, bool );
	typedef void (__stdcall *iFrameStageNotifyFn)( ClientFrameStage_t );
	typedef int(__thiscall* DoPostScreenEffects)(IClientModeShared*, int);
	typedef void (__stdcall *iOverRideViewFn)( CViewSetup* );
	typedef void (__thiscall* IDrawModelExecute)( void*, void*, void*, const ModelRenderInfo_t&, matrix3x4_t* );
	typedef void (__stdcall* ClientCmdUnresticted)( const char* szCmdString, char flag );
	typedef bool ( __cdecl* ServerRankRevealAllFn)( float* );
	typedef void ( __stdcall* PlaySoundFn )( const char* pSample );
	typedef void ( __stdcall* IsReadyFn )();
	typedef void(__thiscall *SceneEnd_t)(void *pEcx);
	typedef int(__fastcall *ooSendDatagram)(INetChannel* netchan, bf_write* datagram);
	typedef bool(__fastcall *ooWriteUsercmdDeltaToBuffer_h)(IBaseClientDll *ECX, void *EDX, int nSlot, bf_write *buf, int from, int to, bool isNewCmd);



	int Hurt(IGameEvent *event);
	//typedef void(__stdcall* iOverrideMouseInput)(float *x, float *y);

public:
	std::vector<IGameEventListener2*> listeners = {};

	/* Original Functions */
	DWORD oCreateMove;
	SceneEnd_t pSceneEnd;
	EndSceneFn oEndScene;
	ooSendDatagram oSendDatagram;
	ooWriteUsercmdDeltaToBuffer_h oWriteUsercmdDeltaToBuffer_h;

	tReset oResetScene;
	iPaintTraverse oPaintTraverse;
	iFrameStageNotifyFn oFrameStageNotify;
	IDrawModelExecute oDrawModelExecute;
	iOverRideViewFn oOverrideView;
	ClientCmdUnresticted oClientCmdUnresticted;
	PlaySoundFn oPlaySound;
	GetViewModelFOVFn GetViewModelFOV;
	ServerRankRevealAllFn ServerRankRevealEx;
	IsReadyFn IsReadyEx;
	DoPostScreenEffects oDoPostScreenEffects;
	//iOverrideMouseInput oOverrideMouseInput;

	/* Holding Varialbles */
	CInput::CUserCmd* CurrentCmd;
	CBaseEntity* LocalPlayer;
	CBaseCombatWeapon* LocalWeapon;
	Vector LastAngles;
	bool SendPacket = true;
	float FOV;

	DWORD Font_Watermark;
	DWORD Font_ESP;
	DWORD Font_Controls;
	DWORD Font_Icon;
	DWORD Font_Weapons;
	DWORD Font_Health;
	DWORD Font_Bomb;
	DWORD Font_LBY;
	DWORD Font_Test;

	DWORD Font_ESP1;
	DWORD Font_ESP2;

	/* Hooking */
	void Hook()
	{
		Font_Watermark = Interfaces.pSurface->Create_Font();
		Font_ESP = Interfaces.pSurface->Create_Font();
		Font_ESP1 = Interfaces.pSurface->Create_Font();
		Font_ESP2 = Interfaces.pSurface->Create_Font();

		Font_Controls = Interfaces.pSurface->Create_Font();
		Font_Icon = Interfaces.pSurface->Create_Font();
		Font_Weapons = Interfaces.pSurface->Create_Font();
		Font_Health = Interfaces.pSurface->Create_Font();
		Font_Bomb = Interfaces.pSurface->Create_Font();
		Font_LBY = Interfaces.pSurface->Create_Font();
		Font_Test = Interfaces.pSurface->Create_Font();

		/*
		FONTFLAG_NONE,
		FONTFLAG_ITALIC = 0x001,
		FONTFLAG_UNDERLINE = 0x002,
		FONTFLAG_STRIKEOUT = 0x004,
		FONTFLAG_SYMBOL = 0x008,
		FONTFLAG_ANTIALIAS = 0x010,
		FONTFLAG_GAUSSIANBLUR = 0x020,
		FONTFLAG_ROTARY = 0x040,
		FONTFLAG_DROPSHADOW = 0x080,
		FONTFLAG_ADDITIVE = 0x100,
		FONTFLAG_OUTLINE = 0x200,
		FONTFLAG_CUSTOM = 0x400,
		FONTFLAG_BITMAP = 0x800,
		*/

		Interfaces.pSurface->SetFontGlyphSet( Font_Controls, "Verdana", 13, 400, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW | FONTFLAG_ADDITIVE | FONTFLAG_OUTLINE);
		Interfaces.pSurface->SetFontGlyphSet( Font_Watermark, "MS Sans Serif", 16, 600, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW | FONTFLAG_OUTLINE);
		Interfaces.pSurface->SetFontGlyphSet( Font_ESP, "Verdana Bold", 12, 50, 0, 0,  FONTFLAG_DROPSHADOW);
	    Interfaces.pSurface->SetFontGlyphSet( Font_ESP2, "Verdana Bold", 12, 50, 0, 0,  FONTFLAG_DROPSHADOW);
		Interfaces.pSurface->SetFontGlyphSet(Font_ESP1, "icomoon", 15, 400, 0, 0, FONTFLAG_DROPSHADOW);
		Interfaces.pSurface->SetFontGlyphSet( Font_Icon, "AstriumWep", 42, 400, 0, 0, FONTFLAG_ANTIALIAS );
		Interfaces.pSurface->SetFontGlyphSet( Font_Weapons, "AstriumWep", 12, 500, 0, 0, FONTFLAG_ANTIALIAS);
		Interfaces.pSurface->SetFontGlyphSet(Font_Health, "Verdana", 13, 400, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW | FONTFLAG_ADDITIVE | FONTFLAG_OUTLINE);
		Interfaces.pSurface->SetFontGlyphSet(Font_Bomb, "Verdana", 15, 600, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
		Interfaces.pSurface->SetFontGlyphSet(Font_LBY, "Verdana Bold", 30, 800, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
		Interfaces.pSurface->SetFontGlyphSet(Font_Test, "Counter-Strike", 30, 400, 0, 0, FONTFLAG_ANTIALIAS);

		DWORD dwDevice = **(DWORD**)(Utils.FindPatternIDA("shaderapidx9.dll", "A1 ?? ?? ?? ?? 50 8B 08 FF 51 0C") + 0x1);
		HRSRC hResInst = FindResource( nullptr, MAKEINTRESOURCE( IDR_FONT1 ), RT_FONT );
		HGLOBAL hGlobal = LoadResource( nullptr, hResInst );
		size_t __LEN = SizeofResource( nullptr, hResInst );
		DWORD dwFonts;
		Interfaces.__FNTHANDLE = AddFontMemResourceEx( nullptr, __LEN, nullptr, &dwFonts );
		auto __FONT = CreateFont( 16, 0, 0, 0, 500, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "undefeated" );
		
		
		CreateDirectory("C:\\floxware", NULL);
		//Global::configpath = "C:\\Astrium\\config.ini";
		// Hooks
		if( Interfaces.pClient )
		{
			VMTClient.bInitialize( reinterpret_cast< DWORD** >( Interfaces.pClient ) );
			oCreateMove = VMTClient.dwHookMethod( reinterpret_cast< DWORD >( &Hooked_Createmove ), 21 );
			cout << ired << "Create Move Hooked (21)." << iyellow << " oCreateMove = 0x" << oCreateMove << white << endl;
			oFrameStageNotify = reinterpret_cast< iFrameStageNotifyFn >( VMTClient.dwHookMethod( reinterpret_cast< DWORD >( &Hooked_Frame_Stage_Notify ), 36 ) );
			cout << ired << "Frame Stage Notfiy Hooked (36)." << iyellow << " oFrameStageNotify = 0x" << oFrameStageNotify << white << endl;
		}

		if( Interfaces.pEngine )
		{
			VMTEngine.bInitialize( reinterpret_cast< DWORD** >( Interfaces.pEngine ) );
			oClientCmdUnresticted = reinterpret_cast< ClientCmdUnresticted >( VMTEngine.dwHookMethod( reinterpret_cast< DWORD >( &Hooked_ClientCmdUnrestricted ), 114 ) );
		}

		if( Interfaces.pSurface )
		{
			VMTPanel.bInitialize( reinterpret_cast< unsigned long** >( Interfaces.pPanel ) );
			oPaintTraverse = reinterpret_cast< iPaintTraverse >( VMTPanel.dwHookMethod( reinterpret_cast< DWORD >( &Hooked_PaintTraverse ), 41 ) );
			cout << ired << "Paint Traverse Hooked (41)." << iyellow << " oPaintTraverse = 0x" << oPaintTraverse << white << endl;
		}

		if (dwDevice)
		{
			VMTD3D.bInitialize(reinterpret_cast<DWORD**>(dwDevice));
			oEndScene = reinterpret_cast<EndSceneFn>(VMTD3D.dwHookMethod(reinterpret_cast<DWORD>(&Hooked_EndScene), 42));
			cout << ired << "DirectX Hooked (42)." << iyellow << "oEndScene = 0x" << oEndScene << white << endl;
			oResetScene = reinterpret_cast<tReset>(VMTD3D.dwHookMethod(reinterpret_cast<DWORD>(&Hooked_Reset), 16));
			cout << ired << "DirectReset Hooked (16)." << iyellow << "oResetScene = 0x" << oResetScene << white << endl;
		}

		if( Interfaces.g_pModelRender )
		{
			VMTModelRender.bInitialize( reinterpret_cast< DWORD** >( Interfaces.g_pModelRender ) );
			oDrawModelExecute = reinterpret_cast< IDrawModelExecute >( VMTModelRender.dwHookMethod( reinterpret_cast< DWORD >( &Hooked_DrawModelExecute ), 21 ) );
			cout << ired << "Draw Model Execute Hooked (21)." << iyellow << " oDrawModelExecute = 0x" << oDrawModelExecute << white << endl;
		}
		if (Interfaces.g_pRenderView)
		{
			VMTRenderView.bInitialize(reinterpret_cast< DWORD** >(Interfaces.g_pRenderView));
			pSceneEnd = reinterpret_cast< SceneEnd_t >(VMTRenderView.dwHookMethod(reinterpret_cast< DWORD >(&hkSceneEnd), 9));

		}
		if (Interfaces.g_clientstate)
		{
			VMTSendFake.bInitialize(reinterpret_cast< DWORD** >(Interfaces.g_clientstate));

			oWriteUsercmdDeltaToBuffer_h = reinterpret_cast< ooWriteUsercmdDeltaToBuffer_h >(VMTSendFake.dwHookMethod(reinterpret_cast< DWORD >(&WriteUsercmdDeltaToBuffer_h), 48));

		}
		if( Interfaces.pClientMode )
		{
			VMTClientMode.bInitialize( reinterpret_cast< DWORD** >( Interfaces.pClientMode ) );
			GetViewModelFOV = reinterpret_cast< GetViewModelFOVFn >(VMTClientMode.dwHookMethod(reinterpret_cast< DWORD >(&Hooked_ViewmodelFOV), 35));
			oOverrideView = reinterpret_cast< iOverRideViewFn >( VMTClientMode.dwHookMethod( reinterpret_cast< DWORD >( &Hooked_OverrideView ), 18 ) );
			oDoPostScreenEffects = reinterpret_cast<DoPostScreenEffects>(VMTClientMode.dwHookMethod(reinterpret_cast<DWORD>(&Hooked_DoPostScreenEffects), 44));
		}

		if( Interfaces.pSurface )
		{
			VMTSurface.bInitialize( ( DWORD** )Interfaces.pSurface );
			//oPlaySound = ( PlaySoundFn )VMTSurface.dwHookMethod( ( DWORD )&Hooked_PlaySound, 82 );
		}

		//while( !( INIT::Window = FindWindowA( "Valve001", nullptr ) ) )
		//	Sleep( 100 );
		//if( INIT::Window )
		//	INIT::OldWindow = ( WNDPROC )SetWindowLongPtr( INIT::Window, GWL_WNDPROC, ( LONG_PTR )Hooked_WndProc );

		Init();
	}

	void UnHook()
	{
		VMTClient.UnHook();
		Sleep( 150 );
		VMTClientMode.UnHook();
		Sleep( 150 );
		VMTEngine.UnHook();
		Sleep( 150 );
		VMTModelRender.UnHook();
		Sleep( 150 );
		VMTPanel.UnHook();
		Sleep( 150 );
		VMTGameEvent.UnHook();
		Sleep( 150 );
		VMTSurface.UnHook();
		Sleep( 150 );
		VMTD3D.UnHook();
		Sleep( 150 );
		for (auto listener : listeners)
			Interfaces.g_GameEventMgr->RemoveListener(listener);
	}
};
