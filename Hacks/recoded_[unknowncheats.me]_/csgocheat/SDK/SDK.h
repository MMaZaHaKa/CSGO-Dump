#pragma once
#include "../stdafx.h"

class bf_read;
class bf_write;
typedef unsigned char uint8;
typedef unsigned char BYTE;
typedef unsigned char byte;
#if !defined( INETCHANNELINFO_H )
#define INETCHANNELINFO_H
#ifdef _WIN32
#pragma once
#endif

#define FLOW_OUTGOING	0
#define FLOW_INCOMING	1
#define MAX_FLOWS		2		// in & out

class INetChannelInfo
{
public:

	enum
	{
		GENERIC = 0, // must be first and is default group
		LOCALPLAYER, // bytes for local player entity update
		OTHERPLAYERS, // bytes for other players update
		ENTITIES, // all other entity bytes
		SOUNDS, // game sounds
		EVENTS, // event messages
		USERMESSAGES, // user messages
		ENTMESSAGES, // entity messages
		VOICE, // voice data
		STRINGTABLE, // a stringtable update
		MOVE, // client move cmds
		STRINGCMD, // string command
		SIGNON, // various signondata
		TOTAL, // must be last and is not a real group
	};

	virtual const char* GetName( void ) const = 0; // get channel name
	virtual const char* GetAddress( void ) const = 0; // get channel IP address as string
	virtual float GetTime( void ) const = 0; // current net time
	virtual float GetTimeConnected( void ) const = 0; // get connection time in seconds
	virtual int GetBufferSize( void ) const = 0; // netchannel packet history size
	virtual int GetDataRate( void ) const = 0; // send data rate in byte/sec

	virtual bool IsLoopback( void ) const = 0; // true if loopback channel
	virtual bool IsTimingOut( void ) const = 0; // true if timing out
	virtual bool IsPlayback( void ) const = 0; // true if demo playback

	virtual float GetLatency( int flow ) const = 0; // current latency (RTT), more accurate but jittering
	virtual float GetAvgLatency( int flow ) const = 0; // average packet latency in seconds
	virtual float GetAvgLoss( int flow ) const = 0; // avg packet loss[0..1]
	virtual float GetAvgChoke( int flow ) const = 0; // avg packet choke[0..1]
	virtual float GetAvgData( int flow ) const = 0; // data flow in bytes/sec
	virtual float GetAvgPackets( int flow ) const = 0; // avg packets/sec
	virtual int GetTotalData( int flow ) const = 0; // total flow in/out in bytes
	virtual int GetSequenceNr( int flow ) const = 0; // last send seq number
	virtual bool IsValidPacket( int flow, int frame_number ) const = 0; // true if packet was not lost/dropped/chocked/flushed
	virtual float GetPacketTime( int flow, int frame_number ) const = 0; // time when packet was send
	virtual int GetPacketBytes( int flow, int frame_number, int group ) const = 0; // group size of this packet
	virtual bool GetStreamProgress( int flow, int* received, int* total ) const = 0; // TCP progress if transmitting
	virtual float GetTimeSinceLastReceived( void ) const = 0; // get time since last recieved packet in seconds
	virtual float GetCommandInterpolationAmount( int flow, int frame_number ) const = 0;

	virtual void GetPacketResponseLatency( int flow, int frame_number, int* pnLatencyMsecs, int* pnChoke ) const = 0;

	virtual void GetRemoteFramerate( float* pflFrameTime, float* pflFrameTimeStdDeviation ) const = 0;

	virtual float GetTimeoutSeconds() const = 0;
};

#endif // INETCHANNELINFO_H
class Netmsgbinder;
class INetChannel;

class INetChannelHandler
{
public:
	virtual	~INetChannelHandler(void) {};

	virtual void ConnectionStart(INetChannel *chan) = 0;	// called first time network channel is established

	virtual void ConnectionClosing(const char *reason) = 0; // network channel is being closed by remote site

	virtual void ConnectionCrashed(const char *reason) = 0; // network error occured

	virtual void PacketStart(int incoming_sequence, int outgoing_acknowledged) = 0;	// called each time a new packet arrived

	virtual void PacketEnd(void) = 0; // all messages has been parsed

	virtual void FileRequested(const char *fileName, unsigned int transferID) = 0; // other side request a file for download

	virtual void FileReceived(const char *fileName, unsigned int transferID) = 0; // we received a file

	virtual void FileDenied(const char *fileName, unsigned int transferID) = 0;	// a file request was denied by other side

	virtual void FileSent(const char *fileName, unsigned int transferID) = 0;	// we sent a file
};
inline void**& getvtable(void* inst, size_t offset = 0)
{
	return *reinterpret_cast< void*** >((size_t)inst + offset);
}

inline const void** getvtable(const void* inst, size_t offset = 0)
{
	if (!inst && !offset)
		return nullptr;
	return *reinterpret_cast< const void*** >((size_t)inst + offset);
}

template< typename Fn >
inline Fn getvfunc(const void* inst, size_t index, size_t offset = 0)
{
	if (!inst && offset == 0)
		return NULL;

	return reinterpret_cast< Fn >(getvtable(inst, offset)[index]);
}
class INetChannel
{
public:
	bool Transmit(bool onlyReliable)
	{
		typedef bool(__thiscall* TransmitFn)(void*, bool);
		return getvfunc<TransmitFn>(this, 49)(this, onlyReliable);
	}
	__int32 vtable; //0x0000 
	Netmsgbinder* msgbinder1; //0x0004 
	Netmsgbinder* msgbinder2; //0x0008 
	Netmsgbinder* msgbinder3; //0x000C 
	Netmsgbinder* msgbinder4; //0x0010 
	unsigned char m_bProcessingMessages; //0x0014 
	unsigned char m_bShouldDelete; //0x0015 
	char pad_0x0016[0x2]; //0x0016
	__int32 m_nOutSequenceNr; //0x0018 
	__int32 m_nInSequenceNr; //0x001C 
	__int32 m_nOutSequenceNrAck; //0x0020 
	__int32 m_nOutReliableState; //0x0024 
	__int32 m_nInReliableState; //0x0028 
	__int32 m_nChokedPackets; //0x002C 

	/*CUtlMemory*/ char m_ReliableDataBuffer[12]; //0x0048 
	/*CUtlMemory*/ char m_UnreliableDataBuffer[12]; //0x006C 
	/*CUtlMemory*/char m_VoiceDataBuffer[12]; //0x0090 
	__int32 m_Socket; //0x009C 
	__int32 m_StreamSocket; //0x00A0 
	__int32 m_MaxReliablePayloadSize; //0x00A4 
	char pad_0x00A8[0x4]; //0x00A8
	char dylanpadding[84]; //padding added by dylan
	float last_received; //0x00B8  //dylan found 0x10c
						 //char pad_0x00BC[0x4]; //0x00BC
	double /*float*/ connect_time; //0x00C0 //dylan found 0x110
								   //char pad_0x00C4[0x4]; //0x00C4
	__int32 m_Rate; //0x00C8  //dylan found 0x118
	/*float*/double m_fClearTime; //0x00CC  //dylan found 0x120
	char pad_0x00D0[0x8]; //0x00D0
	char m_WaitingList[48];
	//CUtlVector m_WaitingList[0]; //0x00D8 
	//CUtlVector m_WaitingList[1]; //0x00EC 
	//char pad_0x0100[0x4120]; //0x0100
	char pad_0x0100[0x40F0]; //dylan changed
	__int32 m_PacketDrop; //0x4220  //dylan found 0x4250
	char m_Name[32]; //0x4224 
	__int32 m_ChallengeNr; //0x4244 
	float m_Timeout; //0x4248 //dylan found 0x4278
	INetChannelHandler* m_MessageHandler; //0x424C 
	/*CUtlVector*/char m_NetMessages[14]; //0x4250 
	__int32 dylanUnknown;
	void* m_pDemoRecorder; //0x4264 
	__int32 m_nQueuedPackets; //0x4268  //dylan found 0x4298
	float m_flInterpolationAmount; //0x426C //dylan found 0x429c
	float m_flRemoteFrameTime; //0x4270 //dylan found 0x42a0
	float m_flRemoteFrameTimeStdDeviation; //0x4274  //dylan found 0x42a4
	float m_flRemoteFrameTimeUnknown; //dylan found 0x42a8
	__int32 m_nMaxRoutablePayloadSize; //0x4278  //dylan found 0x42ac
	__int32 m_nSplitPacketSequence; //0x427C  //dylan found 0x42b0
	char pad_0x4280[0x14]; //0x4280

};//Size=0x4294
class KeyValues
{
public:
	char _pad[0x20];//csgo, for css its a diff size
};



typedef float matrix3x4[3][4];
typedef float matrix4x4[4][4];

typedef void* (__cdecl* CreateInterface_t)( const char*, int* );

typedef void* (*CreateInterfaceFn)( const char* pName, int* pReturnCode );

typedef __int16 int16;
typedef unsigned __int16 uint16;
typedef __int32 int32;
typedef unsigned __int32 uint32;
typedef __int64 int64;
typedef unsigned __int64 uint64;
typedef float vec_t;

class VMatrix;

//USERCMD OFFSETS
#define USERCMDOFFSET 0xEC
#define VERIFIEDCMDOFFSET 0xF0
#define MULTIPLAYER_BACKUP 150
#define CURRENTCOMMANDOFFSET 0x16E8
#define CURRENTPLAYERCOMMANDOFFSET 0x1640
#define PREIDCTIONSEEDOFFSET 0x30
#define PREDICTIONPLAYEROFFSET 0x54
#define GLOBALSOFFSET 0x53
#define WEAPONDATA_MAXRANGEOFFSET 0x77C
#define WEAPONDATA_DAMAGEOFFSET 0x778
#define WEAPONDATA_RANGEMODIFIEROFFSET 0x780
#define WEAPONDATA_PENETRATIONPOWEROFFSET 0x774
#define INPUTOFFSET 0x5F
#define GETSPREADOFFSET 0x740
#define GETCONEOFFSET 0x744
#define UPDATEACCURACYPENALTYOFFSET 0x748
#define WEAPONIDOFFSET 0x6D8
#define WEAPONDATAOFFSET 0x708
#define GETNAMEOFFSET 0x5CC
#define APPSYSTEMFACTORYOFFSET 0x3D
#define CLIENTFACTORYOFFSET 0x75
#define GLOWINDEXOFFSET 0x1DB8

//LIFESTATE
#define	LIFE_ALIVE				0
#define	LIFE_DYING				1
#define	LIFE_DEAD				2
#define LIFE_RESPAWNABLE		3
#define LIFE_DISCARDBODY		4

//Player flags
#define	FL_ONGROUND				(1<<0)	// At rest / on the ground
#define FL_DUCKING				(1<<1)	// Player flag -- Player is fully crouched
#define	FL_WATERJUMP			(1<<3)	// player jumping out of water
#define FL_ONTRAIN				(1<<4) // Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
#define FL_INRAIN				(1<<5)	// Indicates the entity is standing in rain
#define FL_FROZEN				(1<<6) // Player is frozen for 3rd person camera
#define FL_ATCONTROLS			(1<<7) // Player can't move, but keeps key inputs for controlling another entity
#define	FL_CLIENT				(1<<8)	// Is a player
#define FL_FAKECLIENT			(1<<9)	// Fake client, simulated server side; don't send network messages to them
#define	FL_INWATER				(1<<10)	// In water

/* MOVE TYPES */
enum MoveType_t
{
	MOVETYPE_NONE = 0,
	MOVETYPE_ISOMETRIC,
	MOVETYPE_WALK,
	MOVETYPE_STEP,
	MOVETYPE_FLY,
	MOVETYPE_FLYGRAVITY,
	MOVETYPE_VPHYSICS,
	MOVETYPE_PUSH,
	MOVETYPE_NOCLIP,
	MOVETYPE_LADDER,
	MOVETYPE_OBSERVER,
	MOVETYPE_CUSTOM,
	MOVETYPE_LAST = MOVETYPE_CUSTOM,
	MOVETYPE_MAX_BITS = 4
};

//USERCMD BUTTONS
#define IN_ATTACK		(1 << 0)
#define IN_JUMP			(1 << 1)
#define IN_DUCK			(1 << 2)
#define IN_FORWARD		(1 << 3)
#define IN_BACK			(1 << 4)
#define IN_USE			(1 << 5)
#define IN_CANCEL		(1 << 6)
#define IN_LEFT			(1 << 7)
#define IN_RIGHT		(1 << 8)
#define IN_MOVELEFT		(1 << 9)
#define IN_MOVERIGHT	(1 << 10)
#define IN_ATTACK2		(1 << 11)
#define IN_RUN			(1 << 12)
#define IN_RELOAD		(1 << 13)
#define IN_ALT1			(1 << 14)
#define IN_ALT2			(1 << 15)
#define IN_SCORE		(1 << 16)   // Used by client.dll for when scoreboard is held down
#define IN_SPEED		(1 << 17)	// Player is holding the speed key
#define IN_WALK			(1 << 18)	// Player holding walk key
#define IN_ZOOM			(1 << 19)	// Zoom key for HUD zoom
#define IN_WEAPON1		(1 << 20)	// weapon defines these bits
#define IN_WEAPON2		(1 << 21)	// weapon defines these bits
#define IN_BULLRUSH		(1 << 22)
#define IN_GRENADE1		(1 << 23)	// grenade 1
#define IN_GRENADE2		(1 << 24)	// grenade 2

#include "SDK Headers/SDK_Others.h"


class ISurface;
class IPanel;
class HLCLient;
class CEntityList;
class CEngineClient;
class CInput;
class CGlowObjectManager;
class IVModelInfo;
class IVModelRender;
class CPrediction;
class CGameMovement;
class IMoveHelper;
class CGlobalVars;
class IViewRenderBeams;
class IEngineTrace;


class CClockDriftMgr
{
public:
	float m_ClockOffsets[17];   //0x0000
	uint32_t m_iCurClockOffset; //0x0044
	uint32_t m_nServerTick;     //0x0048
	uint32_t m_nClientTick;     //0x004C
}; //Size: 0x0050
class clientstate
{
public:
	void ForceFullUpdate()
	{
		*(uint32_t*)((uintptr_t)this + 0x174) = -1;
	}

	char pad_0000[156];             //0x0000
	uint32_t m_NetChannel;          //0x009C
	uint32_t m_nChallengeNr;        //0x00A0
	char pad_00A4[100];             //0x00A4
	uint32_t m_nSignonState;        //0x0108
	char pad_010C[8];               //0x010C
	float m_flNextCmdTime;          //0x0114
	uint32_t m_nServerCount;        //0x0118
	uint32_t m_nCurrentSequence;    //0x011C
	char pad_0120[8];               //0x0120
	CClockDriftMgr m_ClockDriftMgr; //0x0128
	uint32_t m_nDeltaTick;          //0x0178
	bool m_bPaused;                 //0x017C
	char pad_017D[3];               //0x017D
	uint32_t m_nViewEntity;         //0x0180
	uint32_t m_nPlayerSlot;         //0x0184
	char m_szLevelName[260];        //0x0188
	char m_szLevelNameShort[40];    //0x028C
	char m_szGroupName[40];         //0x02B4
	char pad_02DC[52];              //0x02DC
	uint32_t m_nMaxClients;         //0x0310
	char pad_0314[18820];           //0x0314
	float m_flLastServerTickTime;   //0x4C98
	bool insimulation;              //0x4C9C
	char pad_4C9D[3];               //0x4C9D
	uint32_t oldtickcount;          //0x4CA0
	float m_tickRemainder;          //0x4CA4
	float m_frameTime;              //0x4CA8
	uint32_t lastoutgoingcommand;   //0x4CAC
	uint32_t chokedcommands;        //0x4CB0
	uint32_t last_command_ack;      //0x4CB4
	uint32_t command_ack;           //0x4CB8
	uint32_t m_nSoundSequence;      //0x4CBC
	char pad_4CC0[80];              //0x4CC0
	Vector viewangles;              //0x4D10
}; //Size: 0x4D1C

class IPhysicsSurfaceProps;
class CDebugOverlay;
class IMaterialSystem;;
class IVRenderView;
class ICVar;
class ICvar2;
class IEffects;
class IGameEventManager2;

template<typename T>
inline T ReadPtr(void* base, unsigned long offset)
{
	return *(T*)((char*)base + offset);
}

class CInterfaces
{
private:
	void* GetPointer( const char* Module, const char* InterfaceName )
	{
		void* Interface = nullptr;
		char PossibleInterfaceName[1024];

		CreateInterfaceFn CreateInterface = ( CreateInterfaceFn )GetProcAddress( GetModuleHandleA( Module ), "CreateInterface" );
		for( int i = 1; i < 100; i++ )
		{
			Interface = ( void* )CreateInterface( PossibleInterfaceName, nullptr );
			if( Interface != nullptr )
			{
				cout << PossibleInterfaceName << " Found: 0x " << igreen << Interface << endl;
				break;
			}

			Interface = ( void* )CreateInterface( PossibleInterfaceName, nullptr );
			if( Interface != nullptr )
			{
				cout << PossibleInterfaceName << " Found: 0x " << igreen << Interface << endl;
				break;
			}
		}

		return Interface;
	}

	void* GetInterfacePtr( const char* interfaceName, const char* ptrName, CreateInterface_t pInterface )
	{
		char szDebugString[1024];

		std::string sinterface = "";
		std::string interfaceVersion = "0";

		for( int i = 0; i <= 99; i++ )
		{
			sinterface = interfaceName;
			sinterface += interfaceVersion;
			sinterface += std::to_string( i );

			void* funcPtr = pInterface( sinterface.c_str(), nullptr );

			if( ( DWORD )funcPtr != 0x0 )
			{
				sprintf_s( szDebugString, "%s: 0x%x (%s%s%i)", ptrName, ( DWORD )funcPtr, interfaceName, interfaceVersion.c_str(), i );
				cout << iblue << ptrName << igreen << ": 0x" << funcPtr << "(" << iyellow << interfaceName << interfaceVersion << i << igreen << ")" << white << endl;
				return funcPtr;
			}
			if( i >= 99 && interfaceVersion == "0" )
			{
				interfaceVersion = "00";
				i = 0;
			}
			else if( i >= 99 && interfaceVersion == "00" )
			{
				sprintf_s( szDebugString, "%s: 0x%x (error)", ptrName, ( DWORD )funcPtr );
				cout << ired << ptrName << ": 0x" << funcPtr << " (ERROR)" << white << endl;
			}
		}
		return nullptr;
	}
public:
	void InitialiseSDK()
	{
		auto client = GetModuleHandleW(L"client.dll");
		auto engine = GetModuleHandleW(L"engine.dll");

		auto p = Utils.PatternSearch("client.dll", (BYTE*)"\x8B\x0D\x00\x00\x00\x00\xFF\x75\x08\x8B\x01\xFF\x50\x64", "xx????xxxxxxxx", NULL, NULL);
		if (p)
		{
			pClientMode = **(IClientModeShared***)(p + 2);
			pClientMode = pClientMode;
		}
		cout << iblue << "pClientMode" << igreen << ": 0x" << pClientMode << white << endl;
		
		CreateInterface_t ClientFactory = (CreateInterface_t)GetProcAddress((HMODULE)GetModuleHandle("client.dll"), "CreateInterface");
		CreateInterface_t EngineFactory = (CreateInterface_t)GetProcAddress((HMODULE)GetModuleHandle("engine.dll"), "CreateInterface");
		CreateInterface_t VGUI2Factory = (CreateInterface_t)GetProcAddress((HMODULE)GetModuleHandle("vgui2.dll"), "CreateInterface");
		CreateInterface_t VGUISurfaceFactory = (CreateInterface_t)GetProcAddress((HMODULE)GetModuleHandle("vguimatsurface.dll"), "CreateInterface");
		CreateInterface_t MatFactory = (CreateInterface_t)GetProcAddress((HMODULE)GetModuleHandle("materialsystem.dll"), "CreateInterface");
		CreateInterface_t PhysFactory = (CreateInterface_t)GetProcAddress((HMODULE)GetModuleHandle("vphysics.dll"), "CreateInterface");
		CreateInterface_t StdFactory = (CreateInterface_t)GetProcAddress((HMODULE)GetModuleHandle("vstdlib.dll"), "CreateInterface");

		pSurface = ( ISurface* )GetInterfacePtr( "VGUI_Surface", "g_pSurface", VGUISurfaceFactory);
		pPanel = ( IPanel* )GetInterfacePtr( "VGUI_Panel", "g_pPanel", VGUI2Factory);
		pClient = ( HLCLient* )GetInterfacePtr( "VClient", "g_pClient", ClientFactory );
		pEngine = ( CEngineClient* )GetInterfacePtr( "VEngineClient", "g_pEngine", EngineFactory );
		pEntList = ( CEntityList* )GetInterfacePtr( "VClientEntityList", "g_pEntList", ClientFactory );
		g_pDebugOverlay = ( CDebugOverlay* )GetInterfacePtr( "VDebugOverlay", "g_pDebugOverlay", EngineFactory );
		pTrace = ( IEngineTrace* )GetInterfacePtr( "EngineTraceClient", "g_pEngineTraceClient", EngineFactory );
		g_pModelInfo = ( IVModelInfo* )GetInterfacePtr( "VModelInfoClient", "g_pModelInfo", EngineFactory );
		g_pModelRender = ( IVModelRender* )GetInterfacePtr( "VEngineModel", "g_ModelRender", EngineFactory );
		g_pPred = ( CPrediction* )GetInterfacePtr( "VClientPrediction", "g_pPred", ClientFactory );
		g_pGameMovement = ( CGameMovement* )GetInterfacePtr( "GameMovement", "g_pGameMovement", ClientFactory );
		pPhysProps = ( IPhysicsSurfaceProps* )GetInterfacePtr( "VPhysicsSurfaceProps", "g_pPhysprops", PhysFactory );
		pMaterialSystem = ( IMaterialSystem* )GetInterfacePtr( "VMaterialSystem", "pMaterialSystem", MatFactory );
		g_pRenderView = ( IVRenderView* )GetInterfacePtr( "VEngineRenderView", "g_pRenderView", EngineFactory );
		pGlobalVars = *( CGlobalVars** )( ( ( *( PDWORD* )pClient )[ 0 ] ) + 0x1B );
		pGlobalVars = ( CGlobalVars* )*( PDWORD )pGlobalVars;
		g_GlowObjManager = *(CGlowObjectManager**)(Utils.PatternScan(client, "0F 11 05 ? ? ? ? 83 C8 01") + 3);
		cout << iblue << "pGlobalVars " << igreen << ": 0x" << pGlobalVars << white << endl;
		pInput = *( CInput** )( ( *( DWORD** )pClient )[ 15 ] + 0x1 );
		g_ICVars = ( ICVar* )GetInterfacePtr( "VEngineCvar", "g_pCVars", StdFactory );
		//GameEventManager = ( IGameEventManager2* )GetInterfacePtr ( "GAMEEVENTSMANAGER", "g_pGameEventManager", MaterialSystemFactory );
		GameResources = ( DWORD )Utils.PatternSearch( "client.dll", ( PBYTE )"\x8B\x3D\x00\x00\x00\x00\x85\xFF\x0F\x84\x00\x00\x00\x00\x81\xC7", "xx????xxxx????xx", NULL, NULL ) - ( DWORD )pClient;
		g_pViewRenderBeams = *reinterpret_cast< IViewRenderBeams** > (Utils.FindSig(("client.dll"), ("A1 ? ? ? ? 56 8B F1 B9 ? ? ? ? FF 50 08")) + 0x1);
		// testing some shit //
	//	g_clientstate = *reinterpret_cast< clientstate** > (Utils.PatternScan((engine), ("8B 3D ? ? ? ? 8A F9")) + 2);
		g_clientstate = **(clientstate ***)(Utils.PatternScan(engine, "A1 ? ? ? ? 8B 80 ? ? ? ? C3") + 1);


		cout << iblue << "GameResources " << igreen << ": 0x" << GameResources << white << endl;
		//g_pEffects = (IEffects*)GetInterfacePtr("VEngineEffects", "g_pEffects", AppSystemFactory);	
		g_GameEventMgr = ( IGameEventManager2* )( EngineFactory ( "GAMEEVENTSMANAGER002", nullptr ) );
		cout << iblue << "g_GameEventMgr " << igreen << ": 0x" << g_GameEventMgr << white << endl;
	}

private:
	bool InitialisedSuccessfuly = false;
public:
	//IGameEventManager2* GameEventManager;
	ISurface* pSurface;
	IClientModeShared* pClientMode;
	IPanel* pPanel;
	HLCLient* pClient;
	CEntityList* pEntList;
	CEngineClient* pEngine;
	CInput* pInput;
	CGlowObjectManager* g_pGlowObjectManager;
	IVModelInfo* g_pModelInfo;
	IVModelRender* g_pModelRender;
	CPrediction* g_pPred;
	CGameMovement* g_pGameMovement;
	IMoveHelper* g_pMoveHelper;
	CGlobalVars* pGlobalVars;
	IEngineTrace* pTrace;
	IViewRenderBeams* g_pViewRenderBeams;
	clientstate* g_clientstate;

	//DWORD* pGameEventManager;
	IPhysicsSurfaceProps* pPhysProps;
	CDebugOverlay* g_pDebugOverlay;
	IMaterialSystem* pMaterialSystem;
	IVRenderView* g_pRenderView;
	CGlowObjectManager* g_GlowObjManager;
	IEffects* g_pEffects;
	ICVar* g_ICVars;
	IGameEventManager2* pGameEventManager;
	IGameEventManager2* g_GameEventMgr;
	//IGameEventListener2* GameEventListener = nullptr;
	DWORD GameResources = 0;

	HANDLE __FNTHANDLE;
};

extern CInterfaces Interfaces;

#include "Math.h"

#include "SDK Headers/Valve/checksum_crc.h"
#include "SDK Headers/Valve/dt_recv2.h"
#include "SDK Headers/NetVars.h"

#include "SDK Headers/ISurface.h"
#include "SDK Headers/CClient.h"
#include "SDK Headers/EngineClient.h"
#include "SDK Headers/Entitys.h"
#include "SDK Headers/EntList.h"
#include "SDK Headers/DebugOverlay.h"
#include "SDK Headers/CTrace.h"
#include "SDK Headers/IVRenderView.h"
#include "SDK Headers/CModelInfo.h"
#include "SDK Headers/CInput.h"
#include "SDK Headers/ICVars.h"
#include "SDK Headers/CGlobleVars.h"
#include "SDK Headers/CGameMovement.h"
#include "SDK Headers/CPred.h"

void UTIL_TraceLine( Vector& vecAbsStart, Vector& vecAbsEnd, unsigned int mask, const IHandleEntity* ignore, int collisionGroup, trace_t* ptr );

void UTIL_ClipTraceToPlayers( Vector& vecAbsStart, Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr );

bool TraceToExit( Vector& end, trace_t& tr, Vector start, Vector vEnd, trace_t* trace );

/* FIRE BULLET DATA */
struct FireBulletData
{
	FireBulletData( const Vector& eye_pos ) : src( eye_pos )
	{
	}

	Vector src;
	trace_t enter_trace;
	Vector direction;
	CTraceFilter filter;
	float trace_length;
	float trace_length_remaining;
	float current_damage;
	int penetrate_count;
};

/* HITGROUP DEFINITIONS */
#define		HITGROUP_GENERIC    0
#define		HITGROUP_HEAD       1
#define		HITGROUP_CHEST      2
#define		HITGROUP_STOMACH    3
#define		HITGROUP_LEFTARM    4
#define		HITGROUP_RIGHTARM   5
#define		HITGROUP_LEFTLEG    6
#define		HITGROUP_RIGHTLEG   7
#define		HITGROUP_GEAR       10

typedef bool (*ShouldHitFunc_t)( IHandleEntity* pHandleEntity, int contentsMask );

enum class CSGOClassID
{
	CAK47 = 1,
	CBaseAnimating = 2,
	CBaseAnimatingOverlay = 3,
	CBaseAttributableItem = 4,
	CBaseButton = 5,
	CBaseCombatCharacter = 6,
	CBaseCombatWeapon = 7,
	CBaseCSGrenade = 8,
	CBaseCSGrenadeProjectile = 9,
	CBaseDoor = 10,
	CBaseEntity = 11,
	CBaseFlex = 12,
	CBaseGrenade = 13,
	CBaseParticleEntity = 14,
	CBasePlayer = 15,
	CBasePropDoor = 16,
	CBaseTeamObjectiveResource = 17,
	CBaseTempEntity = 18,
	CBaseToggle = 19,
	CBaseTrigger = 20,
	CBaseViewModel = 21,
	CBaseVPhysicsTrigger = 22,
	CBaseWeaponWorldModel = 23,
	CBeam = 24,
	CBeamSpotlight = 25,
	CBoneFollower = 26,
	CBreakableProp = 27,
	CBreakableSurface = 28,
	CC4 = 29,
	CCascadeLight = 30,
	CChicken = 31,
	CColorCorrection = 32,
	CColorCorrectionVolume = 33,
	CCSGameRulesProxy = 34,
	CCSPlayer = 35,
	CCSPlayerResource = 36,
	CCSRagdoll = 37,
	CCSTeam = 38,
	CDEagle = 39,
	CDecoyGrenade = 40,
	CDecoyProjectile = 41,
	CDynamicLight = 42,
	CDynamicProp = 43,
	CEconEntity = 44,
	CEconWearable = 45,
	CEmbers = 46,
	CEntityDissolve = 47,
	CEntityFlame = 48,
	CEntityFreezing = 49,
	CEntityParticleTrail = 50,
	CEnvAmbientLight = 51,
	CEnvDetailController = 52,
	CEnvDOFController = 53,
	CEnvParticleScript = 54,
	CEnvProjectedTexture = 55,
	CEnvQuadraticBeam = 56,
	CEnvScreenEffect = 57,
	CEnvScreenOverlay = 58,
	CEnvTonemapController = 59,
	CEnvWind = 60,
	CFEPlayerDecal = 61,
	CFireCrackerBlast = 62,
	CFireSmoke = 63,
	CFireTrail = 64,
	CFish = 65,
	CFlashbang = 66,
	CFogController = 67,
	CFootstepControl = 68,
	CFunc_Dust = 69,
	CFunc_LOD = 70,
	CFuncAreaPortalWindow = 71,
	CFuncBrush = 72,
	CFuncConveyor = 73,
	CFuncLadder = 74,
	CFuncMonitor = 75,
	CFuncMoveLinear = 76,
	CFuncOccluder = 77,
	CFuncReflectiveGlass = 78,
	CFuncRotating = 79,
	CFuncSmokeVolume = 80,
	CFuncTrackTrain = 81,
	CGameRulesProxy = 82,
	CHandleTest = 83,
	CHEGrenade = 84,
	CHostage = 85,
	CHostageCarriableProp = 86,
	CIncendiaryGrenade = 87,
	CInferno = 88,
	CInfoLadderDismount = 89,
	CInfoOverlayAccessor = 90,
	CItem_Healthshot = 91,
	CItemDogtags = 92,
	CKnife = 93,
	CKnifeGG = 94,
	CLightGlow = 95,
	CMaterialModifyControl = 96,
	CMolotovGrenade = 97,
	CMolotovProjectile = 98,
	CMovieDisplay = 99,
	CParticleFire = 100,
	CParticlePerformanceMonitor = 101,
	CParticleSystem = 102,
	CPhysBox = 103,
	CPhysBoxMultiplayer = 104,
	CPhysicsProp = 105,
	CPhysicsPropMultiplayer = 106,
	CPhysMagnet = 107,
	CPlantedC4 = 108,
	CPlasma = 109,
	CPlayerResource = 110,
	CPointCamera = 111,
	CPointCommentaryNode = 112,
	CPointWorldText = 113,
	CPoseController = 114,
	CPostProcessController = 115,
	CPrecipitation = 116,
	CPrecipitationBlocker = 117,
	CPredictedViewModel = 118,
	CProp_Hallucination = 119,
	CPropDoorRotating = 120,
	CPropJeep = 121,
	CPropVehicleDriveable = 122,
	CRagdollManager = 123,
	CRagdollProp = 124,
	CRagdollPropAttached = 125,
	CRopeKeyframe = 126,
	CSCAR17 = 127,
	CSceneEntity = 128,
	CSensorGrenade = 129,
	CSensorGrenadeProjectile = 130,
	CShadowControl = 131,
	CSlideshowDisplay = 132,
	CSmokeGrenade = 133,
	CSmokeGrenadeProjectile = 134,
	CSmokeStack = 135,
	CSpatialEntity = 136,
	CSpotlightEnd = 137,
	CSprite = 138,
	CSpriteOriented = 139,
	CSpriteTrail = 140,
	CStatueProp = 141,
	CSteamJet = 142,
	CSun = 143,
	CSunlightShadowControl = 144,
	CTeam = 145,
	CTeamplayRoundBasedRulesProxy = 146,
	CTEArmorRicochet = 147,
	CTEBaseBeam = 148,
	CTEBeamEntPoint = 149,
	CTEBeamEnts = 150,
	CTEBeamFollow = 151,
	CTEBeamLaser = 152,
	CTEBeamPoints = 153,
	CTEBeamRing = 154,
	CTEBeamRingPoint = 155,
	CTEBeamSpline = 156,
	CTEBloodSprite = 157,
	CTEBloodStream = 158,
	CTEBreakModel = 159,
	CTEBSPDecal = 160,
	CTEBubbles = 161,
	CTEBubbleTrail = 162,
	CTEClientProjectile = 163,
	CTEDecal = 164,
	CTEDust = 165,
	CTEDynamicLight = 166,
	CTEEffectDispatch = 167,
	CTEEnergySplash = 168,
	CTEExplosion = 169,
	CTEFireBullets = 170,
	CTEFizz = 171,
	CTEFootprintDecal = 172,
	CTEFoundryHelpers = 173,
	CTEGaussExplosion = 174,
	CTEGlowSprite = 175,
	CTEImpact = 176,
	CTEKillPlayerAttachments = 177,
	CTELargeFunnel = 178,
	CTEMetalSparks = 179,
	CTEMuzzleFlash = 180,
	CTEParticleSystem = 181,
	CTEPhysicsProp = 182,
	CTEPlantBomb = 183,
	CTEPlayerAnimEvent = 184,
	CTEPlayerDecal = 185,
	CTEProjectedDecal = 186,
	CTERadioIcon = 187,
	CTEShatterSurface = 188,
	CTEShowLine = 189,
	CTesla = 190,
	CTESmoke = 191,
	CTESparks = 192,
	CTESprite = 193,
	CTESpriteSpray = 194,
	CTest_ProxyToggle_Networkable = 194,
	CTestTraceline = 196,
	CTEWorldDecal = 197,
	CTriggerPlayerMovement = 198,
	CTriggerSoundOperator = 199,
	CVGuiScreen = 200,
	CVoteController = 201,
	CWaterBullet = 202,
	CWaterLODControl = 203,
	CWeaponAug = 204,
	CWeaponAWP = 205,
	CWeaponBaseItem = 206,
	CWeaponBizon = 207,
	CWeaponCSBase = 208,
	CWeaponCSBaseGun = 209,
	CWeaponCycler = 210,
	CWeaponElite = 211,
	CWeaponFamas = 212,
	CWeaponFiveSeven = 213,
	CWeaponG3SG1 = 214,
	CWeaponGalil = 215,
	CWeaponGalilAR = 216,
	CWeaponGlock = 217,
	CWeaponHKP2000 = 218,
	CWeaponM249 = 219,
	CWeaponM3 = 220,
	CWeaponM4A1 = 221,
	CWeaponMAC10 = 222,
	CWeaponMag7 = 223,
	CWeaponMP5Navy = 224,
	CWeaponMP7 = 225,
	CWeaponMP9 = 226,
	CWeaponNegev = 227,
	CWeaponNOVA = 228,
	CWeaponP228 = 229,
	CWeaponP250 = 230,
	CWeaponP90 = 231,
	CWeaponSawedoff = 232,
	CWeaponSCAR20 = 233,
	CWeaponScout = 234,
	CWeaponSG550 = 235,
	CWeaponSG552 = 236,
	CWeaponSG556 = 237,
	CWeaponSSG08 = 238,
	CWeaponTaser = 239,
	CWeaponTec9 = 240,
	CWeaponTMP = 241,
	CWeaponUMP45 = 242,
	CWeaponUSP = 243,
	CWeaponXM1014 = 244,
	CWorld = 245,
	DustTrail = 246,
	MovieExplosion = 247,
	ParticleSmokeGrenade = 248,
	RocketTrail = 249,
	SmokeTrail = 250,
	SporeExplosion = 251,
	SporeTrail = 252
};

struct dlight_t
{
	int flags;
	Vector origin;
	float radius;
	CColor color;
	float die;
	float decay;
	float minlight;
	int key;
	int style;
	Vector m_Direction;
	float m_InnerAngle;
	float m_OuterAngle;

	float GetRadius() const
	{
		return radius;
	}

	float GetRadiusSquared() const
	{
		return radius * radius;
	}

	float IsRadiusGreaterThanZero() const
	{
		return radius > 0.0f;
	}
};

class IEffects
{
public:
	dlight_t* CL_AllocDlight( int key )
	{
		typedef dlight_t*( __thiscall* OriginalFn )( PVOID, int );
		return getvfunc< OriginalFn >( this, 4 )( this, key );
	}

	dlight_t* CL_AllocElight( int key )
	{
		typedef dlight_t*( __thiscall* OriginalFn )( PVOID, int );
		return getvfunc< OriginalFn >( this, 5 )( this, key );
	}

	dlight_t* GetElightByKey( int key )
	{
		typedef dlight_t*( __thiscall* OriginalFn )( PVOID, int );
		return getvfunc< OriginalFn >( this, 8 )( this, key );
	}
};

class ICollideable
{
public:
	virtual void pad0();

	virtual const Vector& OBBMins() const;

	virtual const Vector& OBBMaxs() const;
};
