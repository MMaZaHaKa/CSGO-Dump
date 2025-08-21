#include "stdafx.h"
//#include "Tools/Menu/Menu.h"
#include "Tools/Hacks/Misc.h"
#include "Tools/Utils/Playerlist.h"
//#include "CircleStrafer.h"
#include "Tools/Hacks/Esp.h"
#include "Tools/Hacks/Antiaim.h"
#include "Tools/Hacks/Aimbot.h"
#include "Tools/Hacks/CreateMoveETC.h"
//#include "Tools/Menu/SettingsManager.h"
#include "Tools/Hacks/KnifeBot.h"
#include "Tools/Hacks/Nightmode.h"
#include "Tools/Hacks/Asuswalls.h"
#include "Tools/Hacks/Legitbot/Legit.h"
//#include "Tools/Hacks/Resolver.h"
#include "BlockBot.h"
#include <d3d9types.h>
#include "Sounds.h"
#include "playsoundapi.h"
#include "Pred.h"
#include <deque>
#pragma comment(lib, "Winmm.lib")
// deque standard header
#pragma once

/*
* Copyright (c) by P.J. Plauger. All rights reserved.
* Consult your license regarding permissions and restrictions.
V6.50:0009 */

#define DEV

cOptions Options;

#define PI_2 PI/2
#define PI_4 PI/4

#define KEY_DOWN(VK_NNM) ((GetAsyncKeyState(VK_NNM) & 0x8000) ? 1:0)
void Gui_Init(IDirect3DDevice9* pDevice);
extern IMGUI_API LRESULT ImGui_ImplDX9_WndProcHandler(HWND, UINT msg, WPARAM wParam, LPARAM lParam);
bool vecPressedKeys[256] = {};
CHackManager Hacks;

VTHookManager VMTPanel;
VTHookManager VMTD3D;
VTHookManager VMTClient;
VTHookManager VMTEngine;
VTHookManager VMTModelRender;
VTHookManager VMTClientMode;
VTHookManager VMTGameEvent;
VTHookManager VMTSurface;
VTHookManager VMTRenderView;
VTHookManager VMTSendFake;
VTHookManager RenderVMT;
bool isastronaut = false;
int nasaindex = 0;
struct CIncomingSequence
{
	CIncomingSequence::CIncomingSequence(int instate, int outstate, int seqnr, float time)
	{
		inreliablestate = instate;
		outreliablestate = outstate;
		sequencenr = seqnr;
		curtime = time;
	}
	int inreliablestate;
	int outreliablestate;
	int sequencenr;
	float curtime;
};

struct CSimulationData {
	CSimulationData() {}
	CSimulationData(CBaseEntity* ent)
	{
		m_pEntity = ent;
		velocity = ent->GetVecVelocity();
		networkorigin = ent->GetNetworkOrigin();
		m_nFlags = ent->GetFlags();
	}

	CBaseEntity* m_pEntity;
	Vector velocity;
	Vector networkorigin;
	int m_nFlags;
};
static int lastincomingsequencenumber;
static std::deque<CIncomingSequence>sequences;





 inline void CrossProduct(const Vector& a, const Vector& b, Vector& result)
{
	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
}
 void inline sincos(float radians, float *sine, float *cosine)
{
	*sine = sin(radians);
	*cosine = cos(radians);
}
 void angle_vectors(const Vector &angles, Vector *forward, Vector *right, Vector *up)
{
	float sr, sp, sy, cr, cp, cy;

	sincos(DEG2RAD(angles[1]), &sy, &cy);
	sincos(DEG2RAD(angles[0]), &sp, &cp);
	sincos(DEG2RAD(angles[2]), &sr, &cr);

	if (forward)
	{
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right)
	{
		right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
		right->y = (-1 * sr*sp*sy + -1 * cr*cy);
		right->z = -1 * sr*cp;
	}

	if (up)
	{
		up->x = (cr*sp*cy + -sr * -sy);
		up->y = (cr*sp*sy + -sr * cy);
		up->z = cr * cp;
	}
}
void DoEnemyCircle(Vector& origin)
{
	if (Options.Visuals.Esp.EspBox.Out) {
		constexpr float radius = 360.0f;
		int width, height;
		Interfaces.pEngine->GetScreenSize(width, height);

		Vector vRealAngles;
		Interfaces.pEngine->GetViewAngles(vRealAngles);

		Vector vForward, vRight, vUp(0.0f, 0.0f, 1.0f);

		angle_vectors(vRealAngles, &vForward, NULL, NULL);

		vForward.z = 0.0f;
		Misc::NormalizeVector(vForward);
		CrossProduct(vUp, vForward, vRight);
		float flFront = DotProduct(origin, vForward);
		float flSide = DotProduct(origin, vRight);
		float flXPosition = radius * -flSide;
		float flYPosition = radius * -flFront;

		float rotation = Hacks.CurrentCmd->viewangles.y;

		rotation = atan2(flXPosition, flYPosition) + M_PI;
		rotation *= 180.0f / M_PI;

		float flYawRadians = -(rotation)* M_PI / 180.0f;
		float flCosYaw = cos(flYawRadians);
		float flSinYaw = sin(flYawRadians);

		flXPosition = (int)((width / 2.0f) + (radius * flSinYaw));
		flYPosition = (int)((height / 2.0f) - (radius * flCosYaw));

		DrawRect(flXPosition, flYPosition, 10, 10, CColor(0, 255, 0, 255));

	}
}
class Hitbox;
class cbullet_tracer_info
{
public:
	cbullet_tracer_info(Vector src, Vector dst, float time, Color color)
	{
		this->src = src;
		this->dst = dst;
		this->time = time;
		this->color = color;

	}

	
	Vector src, dst;
	float time;
	Color color;
};
bool IsVisiblePoint(CBaseEntity*pLocal, Vector Point)
{

	Vector start = pLocal->GetAbsOrigin() + pLocal->GetVecViewOffset();
	trace_t Trace;

	UTIL_TraceLine(start, Point, MASK_SOLID, pLocal, 0, &Trace);

	if (Trace.fraction > 0.9f)  //i know this is bad af....
	{
		return true;
	}

	return false;
}
Vector GetHitboxPosition(CBaseEntity* pEntity, int Hitbox)
{
	matrix3x4 matrix[128];

	if (pEntity->SetupBones(matrix, 128, 0x100, 0.f))
	{
		studiohdr_t* hdr = Interfaces.g_pModelInfo->GetStudioModel(pEntity->GetModel());
		mstudiohitboxset_t* set = hdr->GetHitboxSet(0);

		mstudiobbox_t* hitbox = set->pHitbox(Hitbox);

		if (hitbox)
		{
			Vector vMin, vMax, vCenter, sCenter;
			g_Math.VectorTransform2(hitbox->bbmin, matrix[hitbox->bone], vMin);
			g_Math.VectorTransform2(hitbox->bbmax, matrix[hitbox->bone], vMax);

			vCenter = ((vMin + vMax) *0.5f);

			return vCenter;
		}
	}

	return Vector(0, 0, 0);
}
inline Vector AngleVector(Vector meme)
{
	auto sy = sin(meme.y / 180.f * static_cast<float>(PI));
	auto cy = cos(meme.y / 180.f * static_cast<float>(PI));

	auto sp = sin(meme.x / 180.f * static_cast<float>(PI));
	auto cp = cos(meme.x / 180.f* static_cast<float>(PI));

	return Vector(cp*cy, cp*sy, -sp);
}

inline float DistPointToLine(Vector Point, Vector LineOrigin, Vector Dir)
{
	auto PointDir = Point - LineOrigin;

	auto TempOffset = PointDir.Dot(Dir) / (Dir.x*Dir.x + Dir.y*Dir.y + Dir.z*Dir.z);
	if (TempOffset < 0.000001f)
		return FLT_MAX;

	auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);

	return (Point - PerpendicularPoint).Length();
}


#define TICK_INTERVAL			(Interfaces.pGlobalVars->interval_per_tick)

#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )



void BackTrack::Update(int tick_count)
{
	if (!Options.Ragebot.Aimbot.Backtrack)
		return;

	latest_tick = tick_count;
	for (int i = 0; i < 64; i++)
	{

		UpdateRecord(i);
	}
}

bool BackTrack::IsTickValid(int tick)
{

	int delta = latest_tick - tick;
	float deltaTime = delta * Interfaces.pGlobalVars->interval_per_tick;
	return (fabs(deltaTime) <= 0.2f);
}

void BackTrack::UpdateRecord(int i)
{
	CBaseEntity* pEntity = Interfaces.pEntList->GetClientEntity(i);
	if (pEntity && pEntity->isAlive() && !pEntity->IsDormant())
	{
		float lby = pEntity->pelvisangs();
		if (lby != records[i].lby)
		{

			records[i].tick_count = latest_tick;
			records[i].lby = lby;
			records[i].headPosition = GetHitboxPosition(pEntity, 0);
		}
	}
	else
	{

		records[i].tick_count = 0;
	}
}

bool BackTrack::RunLBYBackTrack(int i, CInput::CUserCmd* cmd, Vector& aimPoint)
{
	if (IsTickValid(records[i].tick_count))
	{
		aimPoint = records[i].headPosition;
		cmd->tick_count = records[i].tick_count;
		return true;
	}
	return false;
}

void BackTrack::Rageback()
{
	if (Options.Ragebot.Aimbot.RageBacktrack)
	{

		int bestTargetIndex = -1;
		int tickxd = 12;
		float bestFov = FLT_MAX;
		player_info_t info;
		if (!Hacks.LocalPlayer->isAlive())
			return;

		for (int i = 0; i < Interfaces.pEngine->GetMaxClients(); i++)
		{
			auto entity = (CBaseEntity*)Interfaces.pEntList->GetClientEntity(i);

			if (!entity || !Hacks.LocalPlayer)
				continue;

			if (entity == Hacks.LocalPlayer)
				continue;

			if (!Interfaces.pEngine->GetPlayerInfo(i, &info))
				continue;

			if (entity->IsDormant())
				continue;

			if (entity->GetTeam() == Hacks.LocalPlayer->GetTeam())
				continue;

			if (entity->isAlive())
			{

				float simtime = entity->GetSimulationTime();
				Vector hitboxPos = GetHitboxPosition(entity, 0);

				//headPositions[i][cmd->command_number % 13] = backtrackData{ simtime, hitboxPos };
				headPositions[i][Hacks.CurrentCmd->command_number % tickxd] = backtrackData{ simtime, hitboxPos };
				Vector ViewDir = AngleVector(Hacks.CurrentCmd->viewangles + (Hacks.LocalPlayer->GetPunchAngleVel() * 2.f));
				float FOVDistance = DistPointToLine(hitboxPos, Hacks.LocalPlayer->GetEyePosition(), ViewDir);

				if (bestFov > FOVDistance)
				{
					bestFov = FOVDistance;
					bestTargetIndex = i;
				}
			}

		}

		float bestTargetSimTime;
		if (bestTargetIndex != -1)
		{
			Vector AimBackrackPoint;
			Vector aimpoit;
			float tempFloat = FLT_MAX;
			Vector ViewDir = AngleVector(Hacks.CurrentCmd->viewangles + (Hacks.LocalPlayer->GetPunchAngleVel() * 2.f));
			for (int t = 0; t < 13; ++t)
			{
				CBaseEntity * entity;

				AimBackrackPoint = headPositions[bestTargetIndex][t].hitboxPos;

				float tempFOVDistance = DistPointToLine(headPositions[bestTargetIndex][t].hitboxPos, Hacks.LocalPlayer->GetEyePosition(), ViewDir);
				if (tempFloat > tempFOVDistance && headPositions[bestTargetIndex][t].simtime > Hacks.LocalPlayer->GetSimulationTime() - 1)
				{

					tempFloat = tempFOVDistance;
					bestTargetSimTime = headPositions[bestTargetIndex][t].simtime;
				}
			}
			aimpoit = headPositions[bestTargetIndex][12].hitboxPos;

				aimpoit = headPositions[bestTargetIndex][12].hitboxPos;

				Hacks.CurrentCmd->tick_count = TIME_TO_TICKS(bestTargetSimTime);

			

		}



	}

}
void BackTrack::legitBackTrack()
{
	if (Options.Ragebot.Aimbot.LegitBacktrack)
	{

		int bestTargetIndex = -1;
		int tickxd = 5;
		float bestFov = FLT_MAX;
		player_info_t info;
		if (!Hacks.LocalPlayer->isAlive())
			return;

		for (int i = 0; i < Interfaces.pEngine->GetMaxClients(); i++)
		{
			auto entity = (CBaseEntity*)Interfaces.pEntList->GetClientEntity(i);

			if (!entity || !Hacks.LocalPlayer)
				continue;

			if (entity == Hacks.LocalPlayer)
				continue;

			if (!Interfaces.pEngine->GetPlayerInfo(i, &info))
				continue;

			if (entity->IsDormant())
				continue;

			if (entity->GetTeam() == Hacks.LocalPlayer->GetTeam())
				continue;

			if (entity->isAlive())
			{

				float simtime = entity->GetSimulationTime();
				Vector hitboxPos = GetHitboxPosition(entity, 0);

				//headPositions[i][cmd->command_number % 13] = backtrackData{ simtime, hitboxPos };
				headPositions[i][Hacks.CurrentCmd->command_number % tickxd] = backtrackData{ simtime, hitboxPos };
				Vector ViewDir = AngleVector(Hacks.CurrentCmd->viewangles + (Hacks.LocalPlayer->GetPunchAngleVel() * 2.f));
				float FOVDistance = DistPointToLine(hitboxPos, Hacks.LocalPlayer->GetEyePosition(), ViewDir);

				if (bestFov > FOVDistance)
				{
					bestFov = FOVDistance;
					bestTargetIndex = i;
				}
			}

		}

		float bestTargetSimTime;
		if (bestTargetIndex != -1)
		{
			float tempFloat = FLT_MAX;
			Vector ViewDir = AngleVector(Hacks.CurrentCmd->viewangles + (Hacks.LocalPlayer ->GetPunchAngleVel() * 2.f));
			for (int t = 0; t < 12; ++t)
			{
				float tempFOVDistance = DistPointToLine(headPositions[bestTargetIndex][t].hitboxPos, Hacks.LocalPlayer->GetEyePosition(), ViewDir);
				if (tempFloat > tempFOVDistance && headPositions[bestTargetIndex][t].simtime > Hacks.LocalPlayer->GetSimulationTime() - 1)
				{
					tempFloat = tempFOVDistance;
					bestTargetSimTime = headPositions[bestTargetIndex][t].simtime;
				}
			}
			if (Hacks.CurrentCmd->buttons & IN_ATTACK)
			{

				Hacks.CurrentCmd->tick_count = TIME_TO_TICKS(bestTargetSimTime);
			}
		}

	}

}

BackTrack* backtracking = new BackTrack();
backtrackData headPositions[64][12];
struct DamageIndicator_t
{
	int iDamage;
	bool bInitialized;
	float flEraseTime;
	float flLastUpdate;
	CBaseEntity * Player;
	Vector Position;
};
std::vector<DamageIndicator_t> DamageIndicator;
std::vector<cbullet_tracer_info> logs;
std::vector< int > AutoStraferConstants = {};
std::vector< const char* > smoke_materials =
{
	"particle/vistasmokev1/vistasmokev1_smokegrenade",
	"particle/vistasmokev1/vistasmokev1_emods",
	"particle/vistasmokev1/vistasmokev1_emods_impactdust",
	"particle/vistasmokev1/vistasmokev1_fire",
};
enum
{
	TE_BEAMPOINTS = 0x00,		// beam effect between two points
	TE_SPRITE = 0x01,	// additive sprite, plays 1 cycle
	TE_BEAMDISK = 0x02,	// disk that expands to max radius over lifetime
	TE_BEAMCYLINDER = 0x03,		// cylinder that expands to max radius over lifetime
	TE_BEAMFOLLOW = 0x04,		// create a line of decaying beam segments until entity stops moving
	TE_BEAMRING = 0x05,		// connect a beam ring to two entities
	TE_BEAMSPLINE = 0x06,
	TE_BEAMRINGPOINT = 0x07,
	TE_BEAMLASER = 0x08,		// Fades according to viewpoint
	TE_BEAMTESLA = 0x09,
};
enum
{
	FBEAM_STARTENTITY = 0x00000001,
	FBEAM_ENDENTITY = 0x00000002,
	FBEAM_FADEIN = 0x00000004,
	FBEAM_FADEOUT = 0x00000008,
	FBEAM_SINENOISE = 0x00000010,
	FBEAM_SOLID = 0x00000020,
	FBEAM_SHADEIN = 0x00000040,
	FBEAM_SHADEOUT = 0x00000080,
	FBEAM_ONLYNOISEONCE = 0x00000100,		// Only calculate our noise once
	FBEAM_NOTILE = 0x00000200,
	FBEAM_USE_HITBOXES = 0x00000400,		// Attachment indices represent hitbox indices instead when this is set.
	FBEAM_STARTVISIBLE = 0x00000800,		// Has this client actually seen this beam's start entity yet?
	FBEAM_ENDVISIBLE = 0x00001000,		// Has this client actually seen this beam's end entity yet?
	FBEAM_ISACTIVE = 0x00002000,
	FBEAM_FOREVER = 0x00004000,
	FBEAM_HALOBEAM = 0x00008000,		// When drawing a beam with a halo, don't ignore the segments and endwidth
	FBEAM_REVERSED = 0x00010000,
	NUM_BEAM_FLAGS = 17	// KEEP THIS UPDATED!
};

struct BeamTrail_t
{
	// NOTE:  Don't add user defined fields except after these four fields.
	BeamTrail_t*	next;
	float			die;
	Vector			org;
	Vector			vel;
};

#define NOISE_DIVISIONS		128
#define MAX_BEAM_ENTS		10


typedef int pixelvis_handle_t;

class Beam_t
{
public:
	Beam_t();

	// Methods of IClientRenderable
	virtual const Vector&	GetRenderOrigin(void) = 0;
	virtual const QAngle&	GetRenderAngles(void) = 0;
	virtual const matrix3x4_t &RenderableToWorldTransform() = 0;
	virtual void			GetRenderBounds(Vector& mins, Vector& maxs) = 0;
	virtual bool			ShouldDraw(void) = 0;
	virtual bool			IsTransparent(void) = 0;
	virtual int				DrawModel(int flags) = 0;
	virtual void			ComputeFxBlend() = 0;
	virtual int				GetFxBlend() = 0;

	// Resets the beam state
	void			Reset();

	// Method to computing the bounding box
	void			ComputeBounds();

	// Bounding box...
	Vector			m_Mins;
	Vector			m_Maxs;
	pixelvis_handle_t *m_queryHandleHalo;
	float			m_haloProxySize;

	// Data is below..

	// Next beam in list
	Beam_t*			next;

	// Type of beam
	int				type;
	int				flags;

	// Control points for the beam
	int				numAttachments;
	Vector			attachment[MAX_BEAM_ENTS];
	Vector			delta;

	// 0 .. 1 over lifetime of beam
	float			t;
	float			freq;

	// Time when beam should die
	float			die;
	float			width;
	float			endWidth;
	float			fadeLength;
	float			amplitude;
	float			life;

	// Color
	float			r, g, b;
	float			brightness;

	// Speed
	float			speed;

	// Animation
	float			frameRate;
	float			frame;
	int				segments;

	// Attachment entities for the beam
	HANDLE			entity[MAX_BEAM_ENTS];
	int				attachmentIndex[MAX_BEAM_ENTS];

	// Model info
	int				modelIndex;
	int				haloIndex;

	float			haloScale;
	int				frameCount;

	float			rgNoise[NOISE_DIVISIONS + 1];

	// Popcorn trail for beam follows to use
	BeamTrail_t*	trail;

	// for TE_BEAMRINGPOINT
	float			start_radius;
	float			end_radius;

	// for FBEAM_ONLYNOISEONCE
	bool			m_bCalculatedNoise;

	float			m_flHDRColorScale;

};

struct BeamInfo_t
{
	//Beam
	int				m_nType;
	CBaseEntity*	m_pStartEnt;
	int				m_nStartAttachment;
	CBaseEntity*	m_pEndEnt;
	int				m_nEndAttachment;
	Vector			m_vecStart;
	Vector			m_vecEnd;
	int				m_nModelIndex;
	const char*		m_pszModelName;
	int				m_nHaloIndex;
	const char*		m_pszHaloName;
	float			m_flHaloScale;
	float			m_flLife;
	float			m_flWidth;
	float			m_flEndWidth;
	float			m_flFadeLength;
	float			m_flAmplitude;
	float			m_flBrightness;
	float			m_flSpeed;
	int				m_nStartFrame;
	float			m_flFrameRate;
	float			m_flRed;
	float			m_flGreen;
	float			m_flBlue;
	bool			m_bRenderable;
	int				m_nSegments;
	int				m_nFlags;
	// Rings
	Vector			m_vecCenter;
	float			m_flStartRadius;
	float			m_flEndRadius;

	BeamInfo_t()
	{
		m_nType = TE_BEAMPOINTS;
		m_nSegments = -1;
		m_pszModelName = NULL;
		m_pszHaloName = NULL;
		m_nModelIndex = -1;
		m_nHaloIndex = -1;
		m_bRenderable = true;
		m_nFlags = 0;
	}
};

class C_Beam;
class IViewRenderBeams
{
public:
	virtual void	InitBeams(void) = 0;
	virtual void	ShutdownBeams(void) = 0;
	virtual void	ClearBeams(void) = 0;

	// Updates the state of the temp ent beams
	virtual void	UpdateTempEntBeams() = 0;

	virtual void	DrawBeam(C_Beam* pbeam, ITraceFilter *pEntityBeamTraceFilter = NULL) = 0;
	virtual void	DrawBeam(Beam_t *pbeam) = 0;

	virtual void	KillDeadBeams(CBaseEntity *pEnt) = 0;

	// New interfaces!
	virtual Beam_t	*CreateBeamEnts(BeamInfo_t &beamInfo) = 0;
	virtual Beam_t	*CreateBeamEntPoint(BeamInfo_t &beamInfo) = 0;
	virtual	Beam_t	*CreateBeamPoints(BeamInfo_t &beamInfo) = 0;
	virtual Beam_t	*CreateBeamRing(BeamInfo_t &beamInfo) = 0;
	virtual Beam_t	*CreateBeamRingPoint(BeamInfo_t &beamInfo) = 0;
	virtual Beam_t	*CreateBeamCirclePoints(BeamInfo_t &beamInfo) = 0;
	virtual Beam_t	*CreateBeamFollow(BeamInfo_t &beamInfo) = 0;

	virtual void	FreeBeam(Beam_t *pBeam) = 0;
	virtual void	UpdateBeamInfo(Beam_t *pBeam, BeamInfo_t &beamInfo) = 0;

	// These will go away!
	virtual void	CreateBeamEnts(int startEnt, int endEnt, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float m_nEndWidth, float m_nFadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b, int type = -1) = 0;
	virtual void	CreateBeamEntPoint(int	nStartEntity, const Vector *pStart, int nEndEntity, const Vector* pEnd,
		int modelIndex, int haloIndex, float haloScale,
		float life, float width, float m_nEndWidth, float m_nFadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b) = 0;
	virtual void	CreateBeamPoints(Vector& start, Vector& end, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float m_nEndWidth, float m_nFadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b) = 0;
	virtual void	CreateBeamRing(int startEnt, int endEnt, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float m_nEndWidth, float m_nFadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b, int flags = 0) = 0;
	virtual void	CreateBeamRingPoint(const Vector& center, float start_radius, float end_radius, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float m_nEndWidth, float m_nFadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b, int flags = 0) = 0;
	virtual void	CreateBeamCirclePoints(int type, Vector& start, Vector& end,
		int modelIndex, int haloIndex, float haloScale, float life, float width,
		float m_nEndWidth, float m_nFadeLength, float amplitude, float brightness, float speed,
		int startFrame, float framerate, float r, float g, float b) = 0;
	virtual void	CreateBeamFollow(int startEnt, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float m_nEndWidth, float m_nFadeLength, float r, float g, float b,
		float brightness) = 0;
};
typedef void(*Fucntion)(IGameEvent* event);
typedef void(__cdecl* MsgFn)(const char* msg, va_list);
void Msg(const char* msg, ...)
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

player_info_t GetInfo(int Index) {
	player_info_t Info;
	Interfaces.pEngine->GetPlayerInfo(Index, &Info);
	return Info;
}

void log(IGameEvent* event)
{
	auto index = Interfaces.pEngine->GetPlayerForUserID(event->GetInt("userid"));

	CBaseEntity *pPlayer = Interfaces.pEntList->GetClientEntity(index);
	if (Hacks.LocalPlayer)
	{
		//if we receive bullet_impact event
			//get the user who fired the bullet

			//get the user who fired the bullet
		auto index = Interfaces.pEngine->GetPlayerForUserID(event->GetInt("userid"));

		//return if the userid is not valid or we werent the entity who was fireing
		if (index != Interfaces.pEngine->GetLocalPlayer())
			return;

		//get local player
		auto local = static_cast<CBaseEntity*>(Interfaces.pEntList->GetClientEntity(index));
		if (!local)
			return;

		//get the bullet impact's position
		Vector position(event->GetFloat("x"), event->GetFloat("y"), event->GetFloat("z"));

		Ray_t ray;
		ray.Init(local->GetEyePosition(), position);

		//skip local player
		CTraceFilter filter;
		filter.pSkip = local;

		//trace a ray
		trace_t tr;
		Interfaces.pTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

		//use different color when we hit a player
		auto color = (tr.m_pEnt && reinterpret_cast<CBaseEntity*>(tr.m_pEnt)->IsPlayer()) ? Color(140, 29, 29, 220) : Color(255, 255, 0, 220);

		//push info to our vector
		logs.push_back(cbullet_tracer_info(local->GetEyePosition(), position, Interfaces.pGlobalVars->curtime, color)); \

			if (!local)
				return;

		//loop through our vector
		for (size_t i = 0; i < logs.size(); i++)
		{//Beam Tracer
			//get the current item
			auto current = logs.at(i);
			BeamInfo_t beamInfo;
			beamInfo.m_nType = TE_BEAMPOINTS;
			beamInfo.m_pszModelName = "sprites/physbeam.vmt";
			beamInfo.m_nModelIndex = -1;
			beamInfo.m_flHaloScale = 0.0f;
			beamInfo.m_flLife = 1.0; //duration of tracers
			beamInfo.m_flWidth = 10.0; //start width
			beamInfo.m_flEndWidth = 10.0; //end width
			beamInfo.m_flFadeLength = 0.0f;
			beamInfo.m_flAmplitude = 2.0f;
			beamInfo.m_flBrightness = 255.f;
			beamInfo.m_flSpeed = 0.2f;
			beamInfo.m_nStartFrame = 0;
			beamInfo.m_flFrameRate = 0.f;
			beamInfo.m_flRed = 0;
			beamInfo.m_flGreen = 255;
			beamInfo.m_flBlue = 0;
			beamInfo.m_nSegments = 2;
			beamInfo.m_bRenderable = true;
			beamInfo.m_nFlags = FBEAM_ONLYNOISEONCE | FBEAM_NOTILE | FBEAM_HALOBEAM;

			beamInfo.m_vecStart = Hacks.LocalPlayer->GetEyePosition();
			beamInfo.m_vecEnd = current.dst;
			if (Options.Visuals.Esp.EspBox.Bullet)
			{
				auto beam = Interfaces.g_pViewRenderBeams->CreateBeamPoints(beamInfo);
				if (beam)
					Interfaces.g_pViewRenderBeams->DrawBeam(beam);
			}
			//draw a line from local player's head position to the hit point

			// Line BulletTracer
			/*if (Options.Visuals.Esp.EspBox.Bullet)
				Interfaces.g_pDebugOverlay->AddLineOverlay(current.src, current.dst, 255, 255, 255, true, 1.f);*/
				//	if (Options.Visuals.Esp.EspBox.Bullet)
					// * add box overlay crash* 
					//Interfaces.g_pDebugOverlay->AddBoxOverlay(current.dst, Vector(2, 2, 2), Vector(2, 2, 2), Vector(0, 0, 0), 255, 255, 255, 127, -1.f);

					//draw a box at the hit point

					//if the item is older than 5 seconds, delete it
			logs.erase(logs.begin() + i);
		}
	}
	if (pPlayer->GetTeam() == 2)
	{
		//if we receive bullet_impact event
		//get the user who fired the bullet

		//get the user who fired the bullet
		auto index = Interfaces.pEngine->GetPlayerForUserID(event->GetInt("userid"));

		//return if the userid is not valid or we werent the entity who was fireing

		//get local player
	if (!pPlayer)
			return;

		//get the bullet impact's position
		Vector position(event->GetFloat("x"), event->GetFloat("y"), event->GetFloat("z"));

		Ray_t ray;
		ray.Init(pPlayer->GetEyePosition(), position);

		//skip local player
		CTraceFilter filter;
		filter.pSkip = pPlayer;

		//trace a ray
		trace_t tr;
		Interfaces.pTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

		//use different color when we hit a player
		auto color = (tr.m_pEnt && reinterpret_cast<CBaseEntity*>(tr.m_pEnt)->IsPlayer()) ? Color(140, 29, 29, 220) : Color(255, 255, 0, 220);

		//push info to our vector
		logs.push_back(cbullet_tracer_info(pPlayer->GetEyePosition(), position, Interfaces.pGlobalVars->curtime, color));

			if (!pPlayer)
				return;

		//loop through our vector
		for (size_t i = 0; i < logs.size(); i++)
		{//Beam Tracer
		 //get the current item
			auto current = logs.at(i);
			BeamInfo_t beamInfo;
			beamInfo.m_nType = TE_BEAMPOINTS;
			beamInfo.m_pszModelName = "sprites/physbeam.vmt";
			beamInfo.m_nModelIndex = -1;
			beamInfo.m_flHaloScale = 0.0f;
			beamInfo.m_flLife = 1.0; //duration of tracers
			beamInfo.m_flWidth = 10.0; //start width
			beamInfo.m_flEndWidth = 10.0; //end width
			beamInfo.m_flFadeLength = 0.0f;
			beamInfo.m_flAmplitude = 2.0f;
			beamInfo.m_flBrightness = 255.f;
			beamInfo.m_flSpeed = 0.2f;
			beamInfo.m_nStartFrame = 0;
			beamInfo.m_flFrameRate = 0.f;
			beamInfo.m_flRed = 255;
			beamInfo.m_flGreen = 0;
			beamInfo.m_flBlue = 0;
			beamInfo.m_nSegments = 2;
			beamInfo.m_bRenderable = true;
			beamInfo.m_nFlags = FBEAM_ONLYNOISEONCE | FBEAM_NOTILE | FBEAM_HALOBEAM;

			beamInfo.m_vecStart = pPlayer->GetEyePosition();
			beamInfo.m_vecEnd = current.dst;
			if (Options.Visuals.Esp.EspBox.Bullet)
			{
				auto beam = Interfaces.g_pViewRenderBeams->CreateBeamPoints(beamInfo);
				if (beam)
					Interfaces.g_pViewRenderBeams->DrawBeam(beam);
			}
			//draw a line from local player's head position to the hit point

			// Line BulletTracer
			/*if (Options.Visuals.Esp.EspBox.Bullet)
			Interfaces.g_pDebugOverlay->AddLineOverlay(current.src, current.dst, 255, 255, 255, true, 1.f);*/
			//	if (Options.Visuals.Esp.EspBox.Bullet)
			// * add box overlay crash* 
			//Interfaces.g_pDebugOverlay->AddBoxOverlay(current.dst, Vector(2, 2, 2), Vector(2, 2, 2), Vector(0, 0, 0), 255, 255, 255, 127, -1.f);

			//draw a box at the hit point

			//if the item is older than 5 seconds, delete it
			logs.erase(logs.begin() + i);
		}
	}
	else
	{
		//if we receive bullet_impact event
		//get the user who fired the bullet

		//get the user who fired the bullet
		auto index = Interfaces.pEngine->GetPlayerForUserID(event->GetInt("userid"));


		//get local player
		if (!pPlayer)
			return;

		//get the bullet impact's position
		Vector position(event->GetFloat("x"), event->GetFloat("y"), event->GetFloat("z"));

		Ray_t ray;
		ray.Init(pPlayer->GetEyePosition(), position);

		//skip local player
		CTraceFilter filter;
		filter.pSkip = pPlayer;

		//trace a ray
		trace_t tr;
		Interfaces.pTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

		//use different color when we hit a player
		auto color = (tr.m_pEnt && reinterpret_cast<CBaseEntity*>(tr.m_pEnt)->IsPlayer()) ? Color(140, 29, 29, 220) : Color(255, 255, 0, 220);

		//push info to our vector
		logs.push_back(cbullet_tracer_info(pPlayer->GetEyePosition(), position, Interfaces.pGlobalVars->curtime, color));

		if (!pPlayer)
			return;

		//loop through our vector
		for (size_t i = 0; i < logs.size(); i++)
		{//Beam Tracer
		 //get the current item
			auto current = logs.at(i);
			BeamInfo_t beamInfo;
			beamInfo.m_nType = TE_BEAMPOINTS;
			beamInfo.m_pszModelName = "sprites/physbeam.vmt";
			beamInfo.m_nModelIndex = -1;
			beamInfo.m_flHaloScale = 0.0f;
			beamInfo.m_flLife = 1.0; //duration of tracers
			beamInfo.m_flWidth = 10.0; //start width
			beamInfo.m_flEndWidth = 10.0; //end width
			beamInfo.m_flFadeLength = 0.0f;
			beamInfo.m_flAmplitude = 2.0f;
			beamInfo.m_flBrightness = 255.f;
			beamInfo.m_flSpeed = 0.2f;
			beamInfo.m_nStartFrame = 0;
			beamInfo.m_flFrameRate = 0.f;
			beamInfo.m_flRed = 0;
			beamInfo.m_flGreen = 0;
			beamInfo.m_flBlue = 255;
			beamInfo.m_nSegments = 2;
			beamInfo.m_bRenderable = true;
			beamInfo.m_nFlags = FBEAM_ONLYNOISEONCE | FBEAM_NOTILE | FBEAM_HALOBEAM;

			beamInfo.m_vecStart = pPlayer->GetEyePosition();
			beamInfo.m_vecEnd = current.dst;
			if (Options.Visuals.Esp.EspBox.Bullet)
			{
				auto beam = Interfaces.g_pViewRenderBeams->CreateBeamPoints(beamInfo);
				if (beam)
					Interfaces.g_pViewRenderBeams->DrawBeam(beam);
			}
			//draw a line from local player's head position to the hit point

			// Line BulletTracer
			/*if (Options.Visuals.Esp.EspBox.Bullet)
			Interfaces.g_pDebugOverlay->AddLineOverlay(current.src, current.dst, 255, 255, 255, true, 1.f);*/
			//	if (Options.Visuals.Esp.EspBox.Bullet)
			// * add box overlay crash* 
			//Interfaces.g_pDebugOverlay->AddBoxOverlay(current.dst, Vector(2, 2, 2), Vector(2, 2, 2), Vector(0, 0, 0), 255, 255, 255, 127, -1.f);

			//draw a box at the hit point

			//if the item is older than 5 seconds, delete it
			logs.erase(logs.begin() + i);
		}
	}

}
void log2(IGameEvent* event)
{
	if (Options.Visuals.Esp.EspBox.DmgIndector)
	{
		CBaseEntity* hurt = (CBaseEntity*)Interfaces.pEntList->GetClientEntity(Interfaces.pEngine->GetPlayerForUserID(event->GetInt("userid")));
		CBaseEntity* attacker = (CBaseEntity*)Interfaces.pEntList->GetClientEntity(Interfaces.pEngine->GetPlayerForUserID(event->GetInt("attacker")));

		if (hurt != Hacks.LocalPlayer && attacker == Hacks.LocalPlayer)
		{
			DamageIndicator_t DmgIndicator;
			DmgIndicator.iDamage = event->GetInt("dmg_health");
			DmgIndicator.Player = hurt;
			DmgIndicator.flEraseTime = Hacks.LocalPlayer->GetTickBase() * Interfaces.pGlobalVars->interval_per_tick + 3.f;
			DmgIndicator.bInitialized = false;

			DamageIndicator.push_back(DmgIndicator);
		}

		float CurrentTime = Hacks.LocalPlayer->GetTickBase() * Interfaces.pGlobalVars->interval_per_tick;

		for (int i = 0; i < DamageIndicator.size(); i++)
		{
			if (DamageIndicator[i].flEraseTime < CurrentTime)
			{
				DamageIndicator.erase(DamageIndicator.begin() + i);
				continue;
			}

			if (!DamageIndicator[i].bInitialized)
			{
				DamageIndicator[i].Position = DamageIndicator[i].Player->GetBonePos(6);
				DamageIndicator[i].bInitialized = true;
			}

			if (CurrentTime - DamageIndicator[i].flLastUpdate > 0.0001f)
			{
				DamageIndicator[i].Position.z -= (0.1f * (CurrentTime - DamageIndicator[i].flEraseTime));
				DamageIndicator[i].flLastUpdate = CurrentTime;
			}

			Vector ScreenPosition;

			if (g_Math.WorldToScreen(DamageIndicator[i].Position, ScreenPosition))
			{
				Interfaces.pSurface->DrawString(Hacks.Font_ESP, true, ScreenPosition.x, ScreenPosition.y, CColor(255, 0, 0, 255), std::to_wstring(DamageIndicator[i].iDamage).c_str());
			}
		}
	}
}
void render()
{
	if (Options.Visuals.Esp.EspBox.Bullet)
		return;

	//get local player
	auto local = static_cast<CBaseEntity*>(Interfaces.pEntList->GetClientEntity(Interfaces.pEngine->GetLocalPlayer()));
	if (!local)
		return;

	//loop through our vector
	for (size_t i = 0; i < logs.size(); i++)
	{
		//get the current item
		auto current = logs.at(i);

		//draw a line from local player's head position to the hit point
		if(Options.Visuals.Esp.EspBox.Bullet)
		Interfaces.g_pDebugOverlay->AddLineOverlay(current.src, current.dst,255, 255, 255, true, -1.f);

		//draw a box at the hit point
		Interfaces.g_pDebugOverlay->AddBoxOverlay(current.dst, Vector(-2, -2, -2), Vector(2, 2, 2), Vector(0, 0, 0), 255, 255, 255, 127, -1.f);

		//if the item is older than 5 seconds, delete it
		if (fabs(Interfaces.pGlobalVars->curtime - current.time) > 5.f)
			logs.erase(logs.begin() + i);
	}
}
void Hurt(IGameEvent* event)
{

	if (Options.Visuals.VisualsMisc.bHitMarker)
	{


		if (Interfaces.pEngine->GetPlayerForUserID(event->GetInt("attacker")) == Interfaces.pEngine->GetLocalPlayer())
		{
			Global::hitmarkerAlpha = 1.f;
			PlaySoundA(rawData, NULL, SND_ASYNC | SND_MEMORY); // Default

		}
	}

	if (Options.Visuals.Infobox.InfoECHO)
	{
		int W, H, cW, cH;
		Interfaces.pEngine->GetScreenSize(W, H);

		long currentTime_ms = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		static long timeStamp = currentTime_ms;

		timeStamp = currentTime_ms;

		std::string change;

		cW = W / 2;
		cH = H / 2;
			if (Interfaces.pEngine->IsInGame() && Interfaces.pEngine->IsConnected() && Hacks.LocalPlayer->isAlive())
			{
				if (Interfaces.pEngine->GetPlayerForUserID(event->GetInt("attacker")) == Interfaces.pEngine->GetLocalPlayer())
				{

					Interfaces.pEngine->ClientCmd_Unrestricted("echo (floxware) player got hit", 0);
					Sleep(0);

				}
				else if (Interfaces.pEngine->IsInGame() && Interfaces.pEngine->IsConnected() && Hacks.LocalPlayer->isAlive() &Hacks.LocalWeapon->GetActiveBaseCombatWeapon()->GetSpread() > 5 & IN_ATTACK)
				{
					Interfaces.pEngine->ClientCmd_Unrestricted("echo (floxware) Missed due to spread", 0);
					Sleep(0);
				}
				else
				{


					Interfaces.pEngine->ClientCmd_Unrestricted("echo (floxware) other player hit other player", 0);
					Sleep(0);
				}
				int attackerid = event->GetInt("attacker");
				int entityid = Interfaces.pEngine->GetPlayerForUserID(attackerid);
				if (entityid == Interfaces.pEngine->GetLocalPlayer())
				{
					Interfaces.pEngine->ClientCmd_Unrestricted("developer 1 ", 0);
					Sleep(0);

					Interfaces.pEngine->ClientCmd_Unrestricted("con_filter_enable 2 ", 0);
					Sleep(0);


					Interfaces.pEngine->ClientCmd_Unrestricted("con_filter_text (floxware) ", 0);
					Sleep(0);

					int nUserID = event->GetInt("attacker");
					int nDead = event->GetInt("userid");
					if (nUserID || nDead)
					{

						player_info_t killed_info = GetInfo(Interfaces.pEngine->GetPlayerForUserID(nDead));
						player_info_t killer_info = GetInfo(Interfaces.pEngine->GetPlayerForUserID(nUserID));
						std::string before = ("(floxware) ");
						std::string two = ("Hit ");
						std::string three = killed_info.name;
						std::string foura = " for ";
						std::string fivea = event->GetString("dmg_health");
						std::string damage = " damage";
						std::string fourb = " (";
						std::string fiveb = event->GetString("health");
						std::string six = " health remaining)";
						std::string newline = "\n";
							Msg((before + two + three + foura + fivea + damage + fourb + fiveb + six + newline).c_str());

					}

				}
			}
		}
	}


void BuyList(IGameEvent* event)
{

}


void AutoBuy(IGameEvent* event)
{

		if (Options.Visuals.Infobox.InfoECHO)
		{
			Interfaces.pEngine->ClientCmd_Unrestricted("developer 1 ", 0);
			Sleep(0);

			Interfaces.pEngine->ClientCmd_Unrestricted("con_filter_enable 2 ", 0);
			Sleep(0);


			Interfaces.pEngine->ClientCmd_Unrestricted("con_filter_text (floxware) ", 0);
			Sleep(0);
			int nUserID = event->GetInt("attacker");
			int nDead = event->GetInt("userid");
			if (nUserID || nDead)
			{
				player_info_t killed_info = GetInfo(Interfaces.pEngine->GetPlayerForUserID(nDead));
				player_info_t killer_info = GetInfo(Interfaces.pEngine->GetPlayerForUserID(nUserID));
				std::string before = ("(floxware) ");
				std::string one = killed_info.name;
				std::string two = (" bought ");
				std::string three = event->GetString("weapon");
				std::string six = "\n";
				Msg((before + one + two + three + six).c_str());
			}

	}
}

class CGameEventListener : public IGameEventListener2
{
private:
	std::string eventName;
	Fucntion Call;
	bool server;

public:
	CGameEventListener() : eventName(""), Call(nullptr), server(false)
	{
	}

	CGameEventListener(const std::string& gameEventName, const Fucntion Func, bool Server) : eventName(gameEventName), Call(Func), server(Server)
	{
		this->RegListener();
	}

	virtual void FireGameEvent(IGameEvent* event)
	{
		this->Call(event);
	}

	virtual int IndicateEventHandling(void)
	{
		return 0x2A;
	}
	 
	void RegListener(void)
	{
		static bool first = true;
		if (!first)
			return;
		if (Interfaces.g_GameEventMgr->AddListener(this, this->eventName.c_str(), server))
			Hacks.listeners.push_back(this);
	}
};

#define REG_EVENT_LISTENER(p, e, n, s) p = new CGameEventListener(n, e, s)
CGameEventListener* Hurts;
CGameEventListener* Buy;
CGameEventListener* HitSounds;
CGameEventListener* Test;
CGameEventListener* Test2;

void Init()
{
	REG_EVENT_LISTENER(Hurts, &Hurt, "player_hurt", false);
	REG_EVENT_LISTENER(Buy, &AutoBuy, "item_purchase", false);
	REG_EVENT_LISTENER(Test, &log, "bullet_impact", false);
	REG_EVENT_LISTENER(Test2, &log2, "player_hurt", false);

}
void __fastcall  hkSceneEnd(void *pEcx, void *pEdx) {

	if (Options.Visuals.Chams.bFakeAngleChams)
	{
		if (Hacks.LocalPlayer)
		{
			IMaterial* material = nullptr;
			IMaterial * visible = Interfaces.pMaterialSystem->FindMaterial("cstm_visible", TEXTURE_GROUP_MODEL);


				Vector OrigAng;
				OrigAng = Hacks.LocalPlayer->GetEyeAngles();
				Hacks.LocalPlayer->SetAngle2(Vector(0, Hacks.LocalPlayer->pelvisangs(), 0));

				bool LbyColor = false;
				float NormalColor[3] = { 1, 1, 1 };
				float lbyUpdateColor[3] = { 0, 1, 0 };
				Interfaces.g_pRenderView->SetColorModulation(NormalColor);
				Interfaces.g_pModelRender->ForcedMaterialOverride(visible);
				Hacks.LocalPlayer->DrawModel(STUDIO_RENDER, 200);
				Interfaces.g_pModelRender->ForcedMaterialOverride(nullptr);
				Hacks.LocalPlayer->SetAngle2(OrigAng);
			
		}
	}
	Hacks.pSceneEnd(pEcx);
}
std::unique_ptr<VTHookManager>ModelRenderHook;

LRESULT __stdcall Hooked_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//if (Menu::Menu.active) return true;
	switch (uMsg) {
	case WM_LBUTTONDOWN:
		vecPressedKeys[VK_LBUTTON] = true;
		break;
	case WM_LBUTTONUP:
		vecPressedKeys[VK_LBUTTON] = false;
		break;
	case WM_RBUTTONDOWN:
		vecPressedKeys[VK_RBUTTON] = true;
		break;
	case WM_RBUTTONUP:
		vecPressedKeys[VK_RBUTTON] = false;
		break;
	case WM_KEYDOWN:
		vecPressedKeys[wParam] = true;
		break;
	case WM_KEYUP:
		vecPressedKeys[wParam] = false;
		break;
	default: break;
	}

	{
		static bool isDown = false;
		static bool isClicked = false;
		if (vecPressedKeys[VK_INSERT]) {
			isClicked = false;
			isDown = true;
		}
		else if (!vecPressedKeys[VK_INSERT] && isDown) {
			isClicked = true;
			isDown = false;
		}
		else {
			isClicked = false;
			isDown = false;
		}

		if (isClicked)
		{
			Global::MenuOpen = !Global::MenuOpen;
			
			static auto cl_mouseenable = Interfaces.g_ICVars->FindVar("cl_mouseenable");
			cl_mouseenable->SetValue(!Global::MenuOpen);
		}
	}

	if (Global::MenuInit && Global::MenuOpen && ImGui_ImplDX9_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(INIT::OldWindow, hWnd, uMsg, wParam, lParam);
}
 void DoEnemyCircle2(CBaseEntity* pLocalPlayer, const Vector &vecDelta, float *flRotation)
{
	float flRadius = 360.0f;
	int iScreenWidth, iScreenHeight;
	Interfaces.pEngine->GetScreenSize(iScreenWidth, iScreenHeight);

	Vector vRealAngles;
	Interfaces.pEngine->GetViewAngles(vRealAngles);

	Vector vForward, vRight, vUp(0.0f, 0.0f, 1.0f);

	Misc::AngleVectors(vRealAngles, &vForward);

	vForward.z = 0.0f;
	Misc::NormalizeVector(vForward);
	Misc::CrossProduct(vUp, vForward, vRight);

	float flFront = DotProduct(vecDelta, vForward);
	float flSide = DotProduct(vecDelta, vRight);
	float flXPosition = flRadius * -flSide;
	float flYPosition = flRadius * -flFront;

	*flRotation = atan2(flXPosition, flYPosition) + M_PI;
	*flRotation *= 180.0f / M_PI;

	float flYawRadians = -(*flRotation) * M_PI / 180.0f;
	float flCosYaw = cos(flYawRadians);
	float flSinYaw = sin(flYawRadians);

	flXPosition = (int)((iScreenWidth / 2.0f) + (flRadius * flSinYaw));
	flYPosition = (int)((iScreenHeight / 2.0f) - (flRadius * flCosYaw));

	DrawRect(flXPosition, flYPosition, 10, 10, CColor(255, 255, 255, 255));
}
void __stdcall Hooked_PaintTraverse(unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	CBaseEntity * Enityty;
	float MEMES = 234523452345;
	MEMES = 1212341234523452345;
	if (MEMES = 234523412341234)
		MEMES = 123412353245;
	MEMES = 235623562345;
	MEMES = 1234123541325;
	if (MEMES = 23461342514325);
	MEMES = 1234123514235;
	MEMES = 14613451234;
	if (MEMES = 12345124514361245)
		MEMES = 235623451245;
	MEMES = 123412341234;
	MEMES = 234123412345;
	if (MEMES = 1235623562356);
	MEMES = 32145143531246;
	MEMES = 34523452345;
	if (MEMES = 2346234623451234)
		MEMES = 1245124523462345;
	MEMES = 213454523452364;
	MEMES = 2345234623453245;
	if (MEMES = 2346234512342345);
	MEMES = 234623451234512435;
	MEMES = 3245234562341234;

	if (Options.Visuals.VisualsMisc.bScopeRemover && !strcmp("HudZoom", Interfaces.pPanel->GetName(vguiPanel)))
		return;

	Hacks.oPaintTraverse(Interfaces.pPanel, vguiPanel, forceRepaint, allowForce);

	if (strcmp("FocusOverlayPanel", Interfaces.pPanel->GetName(vguiPanel)))
	{
		return;
	}
	static bool bSpoofed = false;

	if (!bSpoofed)
	{
        ConVar* sv_cheats = Interfaces.g_ICVars->FindVar("sv_cheats");
        SpoofedConvar* sv_cheats_spoofed = new SpoofedConvar(sv_cheats);
        sv_cheats_spoofed->SetInt(1);
		bSpoofed = true;
	}

	if (Interfaces.pEngine->IsConnected() && Interfaces.pEngine->IsInGame())
	{
		for (int i = 0; i < Interfaces.pEntList->GetHighestEntityIndex(); i++) {
			auto m_entity = static_cast<CBaseEntity*>(Interfaces.pEntList->GetClientEntity(i));
			if (!m_entity) continue;

			DoEnemyCircle(m_entity->GetAbsOrigin());
		}
		Hacks.LocalPlayer = Interfaces.pEntList->GetClientEntity(Interfaces.pEngine->GetLocalPlayer());
		Hacks.LocalWeapon = Hacks.LocalPlayer->GetActiveBaseCombatWeapon();
		
		DoAsusWalls();
		DoNightMode();
		if (Hacks.LocalPlayer)
		{
			Misc::DrawScope();
			Esp::DoEsp();
		}
	}


	render();
	Esp::Watermark();
}
typedef bool(__thiscall *FireEventClientSideFn)(PVOID, IGameEvent*);
FireEventClientSideFn oFireEventClientSide;
bool __fastcall hkFireEventClientSide(PVOID ECX, PVOID EDX, IGameEvent *Event);

int Kills2 = 0;
int Kills = 0;
bool RoundInfo = false;
size_t Delay = 0;



bool __fastcall hkFireEventClientSide(PVOID ECX, PVOID EDX, IGameEvent *Event) {
	if (!Event)
		return oFireEventClientSide(ECX, Event);
	int W, H, cW, cH;
	Interfaces.pEngine->GetScreenSize(W, H);

	long currentTime_ms = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	static long timeStamp = currentTime_ms;

	timeStamp = currentTime_ms;

	std::string change;

	cW = W / 2;
	cH = H / 2;
	const char* szEventName = Event->GetName();

	if (!strcmp(szEventName, "player_hurt"))
	{
		int nUserID = Event->GetInt("attacker");

		if (nUserID && (Interfaces.pEngine->GetPlayerForUserID(nUserID) == Interfaces.pEngine->GetLocalPlayer()))
		{
			int Damge = *Event->GetString("dmg_health");
			Interfaces.pSurface->DrawT(cW - 955, cH - 60, CColor(255, 255, 255, (Global::Infoalpha * 255)), Hacks.Font_ESP2, false, "(floxware) Damage : %i",Damge);

		}
	}

	return oFireEventClientSide(ECX, Event);
}

void New_CreateMove(CInput* thisptr, void* _EAX, int sequence_number, float input_sample_frametime, bool active)
{
		float MEMES = 2356234523452345;
	MEMES = 24573245234562356;
	if (MEMES = 3467346734657)
		MEMES = 23456324521345;
	MEMES = 2345234523462346;
	MEMES = 435734563456;
	if (MEMES = 345634563457);
	MEMES = 3457345634256;
	MEMES = 345634563457;
	if (MEMES = 3456345623452345)
		MEMES = 23453246432563456;
	MEMES = 3456234632452345;
	MEMES = 34563457453623546;
	if (MEMES = 324523462346);
	MEMES = 234625362345;
	MEMES = 234523452345;
	if (MEMES = 23567235623453245)
		MEMES = 73454563456;
	MEMES = 34673465462345;
	MEMES = 112323234512341234;
	if (MEMES = 2341234235431265);
	MEMES = 123412543523452345;
	MEMES = 23421312341234;
	CInput::CVerifiedUserCmd* VerifiedCmd;
	AA.ShouldAA = false;
	CreateMoveETC::GetCmds(sequence_number, Hacks.CurrentCmd, VerifiedCmd);
	Aimbot.Target = Vector(0, 0, 0);
	Misc::ServerRankReveal();
	
	if (Hacks.CurrentCmd && VerifiedCmd && Interfaces.pEngine->IsConnected() && Interfaces.pEngine->IsInGame() && Hacks.LocalPlayer)
	{
		backtracking->legitBackTrack();
		backtracking->Rageback();
		static bool enabled = false;
		static bool check = false;
		Misc::Chatspam();

		//plist.Update();
		MEMCHECK;
		Misc::ClantagChanger();
		Misc::NameSpam();
		Hacks.LocalWeapon = Hacks.LocalPlayer->GetActiveBaseCombatWeapon();
		Vector OrigAng = Hacks.CurrentCmd->viewangles;
		Misc::Normalize(OrigAng);
		//CircleStrafer(OrigAng);
		int airstuckmode = Options.Misc.Movement.iAirStuck;
		switch (airstuckmode)
		{
		case 0:
			break;
		case 1:
			if (GetAsyncKeyState(Options.Misc.Movement.iAirStuckKey))
			{
				if (!check) {
					enabled = !enabled;
					check = true;
				}
			}
			else {
				check = false;
			}
			if (enabled) {
				float ServerTime = Hacks.LocalPlayer->GetTickBase() * Interfaces.pGlobalVars->interval_per_tick;
				float NextPrimaryAttack = Hacks.LocalWeapon->NextPrimaryAttack();

				bool Shooting = false;
				if (NextPrimaryAttack > ServerTime)
					Shooting = true;


				if (!Shooting) {
					Hacks.CurrentCmd->tick_count = 9999999;
				}
			}
			break;
		case 2:
			if (KEY_DOWN(Options.Misc.Movement.iAirStuckKey))
			{
				Hacks.CurrentCmd->tick_count = 9999999;
			}
			break;
		}
	

		if (KEY_DOWN(Options.Misc.Misc1.iFreeCamKey) && Options.Misc.Misc1.bFreeCam)
		{
			Hacks.CurrentCmd->forwardmove = 0;
			Hacks.CurrentCmd->sidemove = 0;
		}

		if (KEY_DOWN(Options.Misc.Movement.iSlowMotionKey) && Options.Misc.Movement.bSlowmotion)
		{
			static bool xd = true;

			if (Hacks.CurrentCmd->buttons & IN_ATTACK || Hacks.CurrentCmd->buttons & IN_ATTACK2)
				xd = false;
			
			if (xd)
			{
				Hacks.CurrentCmd->tick_count = INT_MAX;
				xd = false;
			}
			else
			{
				Hacks.CurrentCmd->tick_count = 0;
				xd = true;
			}
		}

		if (Options.Misc.Misc2.bBlockBot && GetAsyncKeyState(Options.Misc.Misc2.iBlockBotKey))
			AutoBlocker(Hacks.CurrentCmd);
		if (Options.Misc.Movement.bWalkBot)
			Misc::Walkbot(OrigAng);
		CreateMoveETC::BhopMovement(OrigAng);
		if (Hacks.LocalWeapon)
		{
			if (Hacks.LocalPlayer->isAlive())
			{
				//if (Options.Ragebot.HvH.bPred)
				//	Pred::StartPrediction();

				CreateMoveETC::LocalPrediction();
				LegitBot.Run();
				Trigger.Run();
				if (Options.Misc.Misc2.bKnifeBot)
					KnifeBot::Run();
				if (Hacks.LocalWeapon->HasAmmo())
				{
					Aimbot.Aim(Hacks.CurrentCmd);
					if (Misc::Shooting() && LocalInfo.Choked < 13)
					{
						if (Options.Skins.NoRecoil)
							Hacks.CurrentCmd->viewangles -= LocalInfo.PunchAns * (vec_t)1.5f;
						if (!Options.Ragebot.Aimbot.bSilentAim && Options.Ragebot.Aimbot.bAimbotEnable)
							Interfaces.pEngine->SetViewAngles(Hacks.CurrentCmd->viewangles);//	Hacks.SendPacket = false;

						if (Options.Ragebot.Aimbot.bAimbotEnable || Options.Ragebot.Accuracy.bNoRecoil)
							Hacks.CurrentCmd->viewangles -= LocalInfo.PunchAns * (vec_t)2.f;
					}
					else if (!Hacks.LocalWeapon->IsNade() && Options.Ragebot.Aimbot.bAutoPistol)
						Hacks.CurrentCmd->buttons &= ~IN_ATTACK;
					if (*Hacks.LocalWeapon->GetItemDefinitionIndex() == weapon_revolver && (Options.Ragebot.Aimbot.bAimbotEnable && Options.Ragebot.Aimbot.bAutoPistol))
					{
						//if (Hacks.CurrentCmd->buttons & IN_RELOAD)
						//	return;

						//if (!Hacks.LocalPlayer)
						//	return;

						//if (!Hacks.LocalWeapon->HasAmmo())
						//	return;

						//float postponeFireReady = Hacks.LocalWeapon->GetPostponeFireReadyTime();
						//float server_time = Hacks.LocalPlayer->GetTickBase() * Interfaces.pGlobalVars->interval_per_tick;

						//Hacks.CurrentCmd->buttons |= IN_ATTACK;

						//if (Hacks.CurrentCmd->buttons & IN_ATTACK2)
						//	Hacks.CurrentCmd->buttons |= IN_ATTACK;
						//else if (Hacks.LocalWeapon->GetPostponeFireReadyTime() - server_time <= 0.05f)
						//	Hacks.CurrentCmd->buttons &= ~IN_ATTACK;

						if (Hacks.LocalWeapon->GetPostponeFireReadyTime() - Misc::GetServerTime() > 0.05)
							Hacks.CurrentCmd->buttons |= IN_ATTACK;
					}
				}
				else
				{
					Hacks.CurrentCmd->buttons &= ~IN_ATTACK;
					Hacks.CurrentCmd->buttons |= IN_RELOAD;
				}
				if (!Misc::Shooting())
					AA.Run();
			}

		}



		CreateMoveETC::CleanUp(OrigAng);
		if (!Hacks.SendPacket && Options.Visuals.VisualsMisc.iThirdPerson == 1 && Hacks.LocalPlayer->isAlive())
			Global::LastAngle = Hacks.CurrentCmd->viewangles;
		else if (Hacks.SendPacket && Options.Visuals.VisualsMisc.iThirdPerson == 2 &&  Hacks.LocalPlayer->isAlive())
			Global::LastAngle = Hacks.CurrentCmd->viewangles;




		LocalInfo.Choked = Hacks.SendPacket ? 0 : LocalInfo.Choked + 1;
		CreateMoveETC::VerifyCmd(VerifiedCmd);
		//plist.UpdateSim();
	}
}
int tickBaseShift = 0;
bool inSendMove = false, firstSendMovePack = false;
void CL_SendMove()
{
	typedef void(__fastcall* CL_SendMoveFn)(void);
	static CL_SendMoveFn CL_SendMove = reinterpret_cast<CL_SendMoveFn>(Utils.FindSig("engine.dll", "55 8B EC A1 ? ? ? ? 81 EC ? ? ? ? B9 ? ? ? ? 53 8B 98"));

	CL_SendMove();
}
#include "intrin.h"

void WriteUsercmd(void* buf, CInput::CUserCmd* in, CInput::CUserCmd* out)
{
	typedef void(__fastcall* WriteUsercmdFn)(void*, CInput::CUserCmd*, CInput::CUserCmd*);
	static WriteUsercmdFn WriteUsercmd = reinterpret_cast<WriteUsercmdFn>(Utils.FindSig("client.dll", "55 8B EC 83 E4 F8 51 53 56 8B D9 8B 0D"));

	WriteUsercmd(buf, in, out);
}
bool __fastcall WriteUsercmdDeltaToBuffer_h(IBaseClientDll *ECX, void *EDX, int nSlot, bf_write *buf, int from, int to, bool isNewCmd)
{
	static DWORD WriteUsercmdDeltaToBufferReturn = (DWORD)Utils.PatternScan(GetModuleHandle("engine.dll"), "84 C0 74 04 B0 01 EB 02 32 C0 8B FE 46 3B F3 7E C9 84 C0 0F 84 ? ? ? ?");

	if (Options.Visuals.Esp.EspBox.NasaWalk || _ReturnAddress() != &WriteUsercmdDeltaToBufferReturn)
		return Hacks.oWriteUsercmdDeltaToBuffer_h(ECX, EDX, nSlot, buf, from, to, isNewCmd);

	if (from != -1)
		return true;
	int MAX_USERCMD_LOSS = 10  ;
	int  MAX_USERCMDS_SEND = 62  ;
	// CL_SendMove function
	auto CL_SendMove = []()
	{
		using CL_SendMove_t = void(__fastcall*)(void);
		static CL_SendMove_t CL_SendMoveF = (CL_SendMove_t)Utils.PatternScan(GetModuleHandle("engine.dll"), ("55 8B EC A1 ? ? ? ? 81 EC ? ? ? ? B9 ? ? ? ? 53 8B 98"));

		CL_SendMoveF();
	};

	// WriteUsercmd function
	auto WriteUsercmd = [](bf_write *buf, CInput::CUserCmd *in, CInput::CUserCmd *out)
	{
		using WriteUsercmd_t = void(__fastcall*)(bf_write*, CInput::CUserCmd*, CInput::CUserCmd*);
		static WriteUsercmd_t WriteUsercmdF = (WriteUsercmd_t)Utils.PatternScan(GetModuleHandle("client.dll"), ("55 8B EC 83 E4 F8 51 53 56 8B D9 8B 0D"));

		WriteUsercmdF(buf, in, out);
	};
	DWORD ClientState = (DWORD)*(DWORD*)Interfaces.g_clientstate;
		INetChannel *netchan = (INetChannel*)(uintptr_t)(ClientState + 0x9C);
	uintptr_t *framePtr;
	__asm mov framePtr, ebp;
	auto msg = reinterpret_cast<CCLCMsg_Move_t*>(*framePtr + 0xFCC);
	auto& state = Interfaces.g_clientstate;
	auto netChan = netchan;

	int newCommands = msg->numNewCommands;

	// Call CL_SendMove multiple times - split fake move commands between packets to bypass 62 limit
	if (!inSendMove) {
		if (newCommands <= 0) {
			return false;
		}

		inSendMove = true;
		firstSendMovePack = true;
		tickBaseShift += newCommands;

		while (tickBaseShift > 0) {
			CL_SendMove();
			netChan->Transmit(false);
			firstSendMovePack = false;
		}

		inSendMove = false;
		return false; // Stop current CL_SendMove from sending
	}

	// Pack 10 more ticks by faking packet loss
	if (!firstSendMovePack) {
		int loss = min(tickBaseShift, MAX_USERCMD_LOSS);
		tickBaseShift -= loss;
		netChan->m_nOutSequenceNr += loss;
	}

	// Manipulate CLC_Move

	int nextCmdNr = Interfaces.g_clientstate->lastoutgoingcommand + Interfaces.g_clientstate->chokedcommands + 1;
	int totalNewCommands = min(tickBaseShift, MAX_USERCMDS_SEND);
	tickBaseShift -= totalNewCommands;

	from = -1;
	msg->numNewCommands = totalNewCommands;
	msg->numBackupCommands = 0;

	// Write real commands

	for (to = nextCmdNr - newCommands + 1; to <= nextCmdNr; to++) {

		from = to;
	}

	// Write fake commands

	CInput::CUserCmd* lastRealCmd = Interfaces.pInput->GetUserCmd(nSlot, from);
	CInput::CUserCmd fromCmd;
	if (lastRealCmd) {
		fromCmd = *lastRealCmd;
	}

	CInput::CUserCmd toCmd = fromCmd;
	toCmd.command_number++;
	toCmd.tick_count += 200; // Prevent server from executing fake commands sometimes

	for (int i = newCommands; i <= totalNewCommands; i++) {
		WriteUsercmd(buf, &toCmd, &fromCmd);
		fromCmd = toCmd;
		toCmd.command_number++;
		toCmd.tick_count++;
	}
	if (Options.Visuals.Esp.EspBox.NasaWalk && (Hacks.CurrentCmd->buttons & IN_USE))
	{
		if (toCmd.tick_count++ < 3)
			toCmd.tick_count = TIME_TO_TICKS(11.f);
	}
	else
		toCmd.tick_count = 0;
	return true;
}
void __declspec(naked) __fastcall Hooked_Createmove(CInput* thisptr, void* _EAX, int sequence_number, float input_sample_frametime, bool active)
{
	Hacks.SendPacket = true;
	__asm
	{
		MOV Hacks.SendPacket, BL
		PUSH EBP
		MOV EBP, ESP
		SUB ESP, 8
		PUSHAD
		PUSH active
		PUSH input_sample_frametime
		PUSH sequence_number
		CALL Hacks.oCreateMove
	}
	Hacks.LocalPlayer = Interfaces.pEntList->GetClientEntity(Interfaces.pEngine->GetLocalPlayer());
	New_CreateMove(thisptr, _EAX, sequence_number, input_sample_frametime, active);
	__asm
	{
		POPAD
		MOV BL, Hacks.SendPacket
		MOV ESP, EBP
		POP EBP
		RETN 0x0C
	}
}

float __stdcall Hooked_ViewmodelFOV()
{
	if (Interfaces.pEngine->IsInGame() && Interfaces.pEngine->IsConnected())
	{
		if (Options.Misc.Misc2.bEnableFov)
		{
			float value = Options.Misc.Misc2.flViewModelValue;
			return value;
		}
		else
		{
			return 68.f;
		}
	}
	return 68.f;
}


void __stdcall Hooked_OverrideView(CViewSetup* pSetup)
{
	static Vector newOrigin;

	if (Interfaces.pEngine->IsInGame() && Interfaces.pEngine->IsConnected())
	{
		if (Options.Misc.Misc2.bEnableFov)
			pSetup->fov = Options.Misc.Misc2.flFovValue;

		if (Options.Misc.Misc1.bFreeCam)
		{
			int freecamkey = Options.Misc.Misc1.iFreeCamKey;
			
			float freecamspeed = Options.Misc.Misc1.flFreeCamSpeed;

			if (!KEY_DOWN(freecamkey))
			{
				newOrigin = pSetup->origin;
			}
			if (KEY_DOWN(freecamkey))
			{

				unsigned int fSpeed = freecamspeed; //5.f;
				if (KEY_DOWN(VK_CONTROL))
					fSpeed = fSpeed * 0.45;
				if (KEY_DOWN(VK_SHIFT))
					fSpeed = fSpeed * 1.65;
				if (KEY_DOWN(0x57)) // W		
				{
					newOrigin += pSetup->angles.Forward() * fSpeed;
				}
				if (KEY_DOWN(0x41)) // A		
				{
					newOrigin += pSetup->angles.Right() * fSpeed;
				}
				if (KEY_DOWN(0x44)) // D		
				{
					newOrigin -= pSetup->angles.Right() * fSpeed;
				}
				if (KEY_DOWN(0x53)) // S		
				{
					newOrigin -= pSetup->angles.Forward() * fSpeed;

				}
				if (KEY_DOWN(VK_SPACE)) // Space		
				{
					newOrigin += pSetup->angles.Up() * fSpeed;
				}
				pSetup->origin = newOrigin;
			}
		}

		CHECKMEM;
		//pSetup->angles = Aimbot.Angles;
	}
	Hacks.FOV = pSetup->fov;
	Hacks.oOverrideView(pSetup);
}
class CGlowObjectManager
{
public:

	class GlowObjectDefinition_t
	{
	public:
		void set(float r, float g, float b, float a)
		{
			m_vGlowColor = Vector(r, g, b);
			m_flGlowAlpha = a;
			m_bRenderWhenOccluded = true;
			m_bRenderWhenUnoccluded = false;
			m_flBloomAmount = 1.0f;
		}

		CBaseEntity* getEnt()
		{
			return m_hEntity;
		}

		bool IsUnused() const { return m_nNextFreeSlot != GlowObjectDefinition_t::ENTRY_IN_USE; }

	public:
		CBaseEntity * m_hEntity;
		Vector            m_vGlowColor;
		float            m_flGlowAlpha;

		char            unknown[4]; //pad 
		float            flUnk; //confirmed to be treated as a float while reversing glow functions 
		float            m_flBloomAmount;
		float            localplayeriszeropoint3;


		bool            m_bRenderWhenOccluded;
		bool            m_bRenderWhenUnoccluded;
		bool            m_bFullBloomRender;
		char            unknown1[1]; //pad 


		int                m_nFullBloomStencilTestValue; // 0x28 
		int                iUnk; //appears like it needs to be zero  
		int                m_nSplitScreenSlot; //Should be -1 

											   // Linked list of free slots 
		int                m_nNextFreeSlot;

		// Special values for GlowObjectDefinition_t::m_nNextFreeSlot 
		static const int END_OF_FREE_LIST = -1;
		static const int ENTRY_IN_USE = -2;
	};

	GlowObjectDefinition_t* m_GlowObjectDefinitions;
	int		max_size;
	int		pad;
	int		size;
	GlowObjectDefinition_t* m_GlowObjectDefinitions2;
	int		currentObjects;	int m_nFirstFreeSlot;
};
DWORD GlowManager = *(DWORD*)(Utils.FindSig("client.dll", "0F 11 05 ?? ?? ?? ?? 83 C8 01 C7 05 ?? ?? ?? ?? 00 00 00 00") + 3);

int __stdcall Hooked_DoPostScreenEffects(int a1)
{
	IMaterial *pMatGlowColor = Interfaces.pMaterialSystem->FindMaterial("dev/glow_color", TEXTURE_GROUP_OTHER, true);

	Interfaces.g_pModelRender->ForcedMaterialOverride(pMatGlowColor);
	CGlowObjectManager* GlowObjectManager = (CGlowObjectManager*)GlowManager;

	if (Options.Visuals.Esp.EspBox.Glowmeme && Interfaces.g_GlowObjManager && Interfaces.pEngine->IsConnected())
	{
		if (Hacks.LocalPlayer)
		{
			for (int i = 0; i <= GlowObjectManager->size; i++)
			{
				if (Interfaces.g_GlowObjManager->m_GlowObjectDefinitions[i].IsUnused() || !Interfaces.g_GlowObjManager->m_GlowObjectDefinitions[i].getEnt())
					continue;

				CGlowObjectManager::GlowObjectDefinition_t* glowEnt = &Interfaces.g_GlowObjManager->m_GlowObjectDefinitions[i];

				if (glowEnt != nullptr)
				{
					ClientClass* pClass = (ClientClass*)glowEnt->getEnt()->GetClientClass();

					switch (pClass->m_ClassID)
					{
					case 35:
					{
						if (Options.Visuals.Esp.EspFilter.bEnemyOnly && glowEnt->getEnt()->GetTeam() == Hacks.LocalPlayer->GetTeam())
							break;

						glowEnt->set(255, 0, 255, 0.7f);
						break;
					}
					case 1:
					{
						glowEnt->set(255, 0, 255, 0.7f);
						break;
					}
					case 9:
					{
						glowEnt->set(255, 0, 255, 0.7f);
						break;
					}					case 29:
					{
						glowEnt->set(255, 0, 255, 0.7f);
						break;
					}					case 39:
					{
						glowEnt->set(255, 0, 255, 0.7f);
						break;
					}					case 41:
					{
						glowEnt->set(255, 0, 255, 0.7f);
						break;
					}
					case 66:
					{
						glowEnt->set(255, 0, 255, 0.7f);
						break;
					}
					case 87:
					{
						glowEnt->set(255, 0, 255, 0.7f);
						break;
					}
					case 98:
					{
						glowEnt->set(255, 0, 255, 0.7f);
						break;
					}
					case 108:
					{
						glowEnt->set(255, 0, 255, 0.7f);
						break;
					}					case 130:
					{
						glowEnt->set(255, 0, 255, 0.7f);
						break;
					}
					case 134:
					{
						glowEnt->set(255, 0, 255, 0.7f);
						break;
					}
					}
				}
			}
		}
	}
	if (Options.Visuals.Esp.EspBox.Glowmeme2 && Interfaces.g_GlowObjManager && Interfaces.pEngine->IsConnected())
	{
		if (Hacks.LocalPlayer)
		{
			for (int i = 0; i <= GlowObjectManager->size; i++)
			{
				if (Interfaces.g_GlowObjManager->m_GlowObjectDefinitions[i].IsUnused() || !Interfaces.g_GlowObjManager->m_GlowObjectDefinitions[i].getEnt())
					continue;

				CGlowObjectManager::GlowObjectDefinition_t* glowEnt = &Interfaces.g_GlowObjManager->m_GlowObjectDefinitions[i];

				if (glowEnt != nullptr)
				{
					ClientClass* pClass = (ClientClass*)glowEnt->getEnt()->GetClientClass();

					switch (pClass->m_ClassID)
					{
					case 1:
					{
						glowEnt->set(255, 0, 255, 0.7f);
						
					}
					break;
					case 9:
					{
						glowEnt->set(255, 0, 255, 0.7f);
					}					case 29:
					{
						glowEnt->set(255, 0, 255, 0.7f);
					}					case 39:
					{
						glowEnt->set(255, 0, 255, 0.7f);
					}					case 41:
					{
						glowEnt->set(255, 0, 255, 0.7f);
					}
					case 66:
					{
						glowEnt->set(255, 0, 255, 0.7f);
					}
					case 87:
					{
						glowEnt->set(255, 0, 255, 0.7f);
					}
					case 98:
					{
						glowEnt->set(255, 0, 255, 0.7f);
					}
					case 108:
					{
						glowEnt->set(255, 0, 255, 0.7f);
					}				case 130:
					{
						glowEnt->set(255, 0, 255, 0.7f);
					}
					case 134:
					{
						glowEnt->set(255, 0, 255, 0.7f);
					}
					default:
						if (Options.Visuals.Esp.EspBox.Glowmeme2)
						{
							if (strstr(pClass->m_pNetworkName, "Weapon"))
								glowEnt->set(255, 0, 255, 0.7f);
						}

					}
				}
			}
		}
	}
	return Hacks.oDoPostScreenEffects(Interfaces.pClientMode, a1);

}

void AutoResolver(Vector* & Angle, PlayerList::CPlayer* Player)
{
	static int iLastUpdatedTick = 0;

	Player->reset.y = Angle->y;

	static Vector orginalview = Vector(0, 0, 0);
	if (orginalview.x != Angle->x)
		orginalview.x = Angle->x;
	if (Angle->y != Player->resolved)
	{
		orginalview.y = Angle->y;

		float flResolve = 0.f;
		float flLowerBodyYaw = Player->entity->pelvisangs();

		int difference = orginalview.y - flLowerBodyYaw;

		iLastUpdatedTick++;

		if (flLowerBodyYaw != Player->flLastPelvisAng)
		{
			if (Player->entity->GetVecVelocity().Length2D() == 0)
			{
				int temp = static_cast<int>(floor(Player->flEyeAng - Player->flLastPelvisAng));
				while (temp < 0)
					temp += 360;
				while (temp > 360)
					temp -= 360;
				Player->Backtrack[temp] = flLowerBodyYaw - Player->flEyeAng;
			}

			iLastUpdatedTick = 0;
			Player->flLastPelvisAng = flLowerBodyYaw;
			Player->flEyeAng = orginalview.y;
		}

		if (Player->entity->GetVecVelocity().Length2D() >= 1)
		{
			flResolve = flLowerBodyYaw;
		}
		else
		{
			int temp = static_cast<int>(floor(orginalview.y - flLowerBodyYaw));
			while (temp < 0)
				temp += 360;
			while (temp > 360)
				temp -= 360;
			flResolve = Player->Backtrack[temp] + orginalview.y;
		}
		Angle->y = flResolve;
		Player->resolved = Angle->y;
	}
}

static inline bool IsNearEqual(float v1, float v2, float Tolerance)
{
	return std::abs(v1 - v2) <= std::abs(Tolerance);
}

float GetLatency()
{
	INetChannelInfo *nci = Interfaces.pEngine->GetNetChannelInfo();
	if (nci)
	{
		float Latency = nci->GetAvgLatency(FLOW_OUTGOING) + nci->GetAvgLatency(FLOW_INCOMING);
		return Latency;
	}
	else
	{
		return 0.0f;
	}
}
float GetOutgoingLatency()
{
	INetChannelInfo *nci = Interfaces.pEngine->GetNetChannelInfo();
	if (nci)
	{
		float OutgoingLatency = nci->GetAvgLatency(FLOW_OUTGOING);
		return OutgoingLatency;
	}
	else
	{
		return 0.0f;
	}
}
float GetIncomingLatency()
{
	INetChannelInfo *nci = Interfaces.pEngine->GetNetChannelInfo();
	if (nci)
	{
		float IncomingLatency = nci->GetAvgLatency(FLOW_INCOMING);
		return IncomingLatency;
	}
	else
	{
		return 0.0f;
	}
}

/*void ApplyAAAHooks()
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

				// Knives
				if (!strcmp(name, "m_nModelIndex"))
				{
					oRecvnModelIndex = (RecvVarProxyFn)pProp->m_ProxyFn;
					pProp->m_ProxyFn = Hooked_RecvProxy_Viewmodel;
				}
			}
		}
		pClass = pClass->m_pNext;
	}
}*/
/*void KnifeApplyCallbk()
{

	static auto CL_FullUpdate = reinterpret_cast<CL_FullUpdate_t>(Utils.PatternSearch("client.dll", (PBYTE) "\xA1\x00\x00\x00\x00\xB9\x00\x00\x00\x00\x56\xFF\x50\x14\x8B\x34\x85", "x????x????xxxxxxx", 0, 0));
	CL_FullUpdate();
}*/
int RandomInt(int min, int max)
{
	return rand() % max + min;
}

bool bGlovesNeedUpdate;
void ApplyCustomGloves()
{
	if (Options.Skins.GloveOver)
	{
		if (!Interfaces.pEngine->IsConnected() || !Interfaces.pEngine->IsInGame())
			return;

		if (!Hacks.LocalPlayer->isAlive())
		{
			DWORD* hMyWearables = (DWORD*)((size_t)Hacks.LocalPlayer + 0x2EF4);

			if (!Interfaces.pEntList->GetClientEntity(hMyWearables[0] & 0xFFF))
			{
				for (ClientClass* pClass = Interfaces.pClient->GetAllClasses(); pClass; pClass = pClass->m_pNext)
				{
					if (pClass->m_ClassID != (int)CSGOClassID::CEconWearable)
						continue;

					int iEntry = (Interfaces.pEntList->GetHighestEntityIndex() + 1);
					int	iSerial = RandomInt(0x0, 0xFFF);

					pClass->m_pCreateFn(iEntry, iSerial);
					hMyWearables[0] = iEntry | (iSerial << 16);

					break;
				}
			}

			player_info_t LocalPlayerInfo;
			Interfaces.pEngine->GetPlayerInfo(Interfaces.pEngine->GetLocalPlayer(), &LocalPlayerInfo);

			CBaseCombatWeapon* glovestochange = (CBaseCombatWeapon*)Interfaces.pEntList->GetClientEntity(hMyWearables[0] & 0xFFF);

			if (!glovestochange)
				return;
			int Model = Interfaces.g_pModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl");
			int Model2 = Interfaces.g_pModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl");
			int Model3 = Interfaces.g_pModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl");
			int Model4 = Interfaces.g_pModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl");
			int Model5 = Interfaces.g_pModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl");
			int Model6 = Interfaces.g_pModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl");

			switch (Options.Skins.Glovemodel)
			{
			case 0:
			{
				*glovestochange->GetItemDefinitionIndex() = 5027;
				*glovestochange->GetModelIndex() = Model;
				break;
			}
			case 1:
			{
				*glovestochange->GetItemDefinitionIndex() = 5032;
				*glovestochange->GetModelIndex() = Model2;				
				break;
			}
			case 2:
			{
				*glovestochange->GetItemDefinitionIndex() = 5031;
				*glovestochange->GetModelIndex() = Model3;
				break;
			}
			case 3:
			{
				*glovestochange->GetItemDefinitionIndex() = 5030;
				*glovestochange->GetModelIndex() = Model4;
				break;
			}
			case 4:
			{
				*glovestochange->GetItemDefinitionIndex() = 5033;
				*glovestochange->GetModelIndex() = Model5;
				break;
			}
			case 5:
			{
				*glovestochange->GetItemDefinitionIndex() = 5034;
				*glovestochange->GetModelIndex() = Model6;
				break;
			}
			default:
				break;
			}

			switch (Options.Skins.Gloveskin)
			{
			case 0:
				break;
			case 1:
				*glovestochange->GetFallbackPaintKit() = 10007;
				break;
			case 2:
				*glovestochange->GetFallbackPaintKit() = 10006;
				break;
			case 3:
				*glovestochange->GetFallbackPaintKit() = 10039;
				break;
			case 4:
				*glovestochange->GetFallbackPaintKit() = 10008;
				break;
			case 5:
				*glovestochange->GetFallbackPaintKit() = 10021;
				break;
			case 6:
				*glovestochange->GetFallbackPaintKit() = 10036;
				break;
			case 7:
				*glovestochange->GetFallbackPaintKit() = 10009;
				break;
			case 8:
				*glovestochange->GetFallbackPaintKit() = 10010;
				break;
			case 9:
				*glovestochange->GetFallbackPaintKit() = 10016;
				break;
			case 10:
				*glovestochange->GetFallbackPaintKit() = 10013;
				break;
			case 11:
				*glovestochange->GetFallbackPaintKit() = 10040;
				break;
			case 12:
				*glovestochange->GetFallbackPaintKit() = 10015;
				break;
			case 13:
				*glovestochange->GetFallbackPaintKit() = 10037;
				break;
			case 14:
				*glovestochange->GetFallbackPaintKit() = 10038;
				break;
			case 15:
				*glovestochange->GetFallbackPaintKit() = 10018;
				break;
			case 16:
				*glovestochange->GetFallbackPaintKit() = 10019;
				break;
			case 17:
				*glovestochange->GetFallbackPaintKit() = 10026;
				break;
			case 18:
				*glovestochange->GetFallbackPaintKit() = 10028;
				break;
			case 19:
				*glovestochange->GetFallbackPaintKit() = 10027;
				break;
			case 20:
				*glovestochange->GetFallbackPaintKit() = 10024;
				break;
			case 21:
				*glovestochange->GetFallbackPaintKit() = 10033;
				break;
			case 22:
				*glovestochange->GetFallbackPaintKit() = 10034;
				break;
			case 23:
				*glovestochange->GetFallbackPaintKit() = 10035;
				break;
			case 24:
				*glovestochange->GetFallbackPaintKit() = 10030;
				break;
			}
				*glovestochange->GetOriginalOwnerXuidHigh() = 0;
				*glovestochange->GetOriginalOwnerXuidLow() = 0;
				*glovestochange->GetFallbackWear() = 0.001f;
				*glovestochange->GetItemIDHigh() = 1;
				*glovestochange->GetAccountID() = LocalPlayerInfo.xuidlow;
				glovestochange->PreDataUpdate(0);

		}
		return;

	}
}
struct ResolverData
{
	float simtime, flcycle[13], flprevcycle[13], flweight[13], flweightdatarate[13], fakewalkdetection[2], fakeanglesimtimedetection[2], fakewalkdetectionsimtime[2];
	float yaw, addyaw, lbycurtime;
	float shotsimtime, oldlby, lastmovinglby, balanceadjustsimtime, balanceadjustflcycle;
	int fakeanglesimtickdetectionaverage[4], amountgreaterthan2, amountequal1or2, amountequal0or1, amountequal1, amountequal0, resetmovetick, resetmovetick2;
	int tick, balanceadjusttick, missedshots, activity[13];
	bool bfakeangle, bfakewalk, playerhurtcalled, weaponfirecalled;
	Vector shotaimangles, hitboxPos, balanceadjustaimangles;
	uint32_t norder[13];
	char* resolvermode = "NONE", *fakewalk = "Not Moving";
} pResolverData[64];;
bool isPartOf(char *a, char *b) {
	if (std::strstr(b, a) != NULL) {    //Strstr says does b contain a
		return true;
	}
	return false;
}



enum
{
	ACT_RESET,
	ACT_IDLE,
	ACT_TRANSITION,
	ACT_COVER,
	ACT_COVER_MED,
	ACT_COVER_LOW,
	ACT_WALK,
	ACT_WALK_AIM,
	ACT_WALK_CROUCH,
	ACT_WALK_CROUCH_AIM,
	ACT_RUN,
	ACT_RUN_AIM,
	ACT_RUN_CROUCH,
	ACT_RUN_CROUCH_AIM,
	ACT_RUN_PROTECTED,
	ACT_SCRIPT_CUSTOM_MOVE,
	ACT_RANGE_ATTACK1,
	ACT_RANGE_ATTACK2,
	ACT_RANGE_ATTACK1_LOW,
	ACT_RANGE_ATTACK2_LOW,
	ACT_DIESIMPLE,
	ACT_DIEBACKWARD,
	ACT_DIEFORWARD,
	ACT_DIEVIOLENT,
	ACT_DIERAGDOLL,
	ACT_FLY,
	ACT_HOVER,
	ACT_GLIDE,
	ACT_SWIM,
	ACT_JUMP,
	ACT_HOP,
	ACT_LEAP,
	ACT_LAND,
	ACT_CLIMB_UP,
	ACT_CLIMB_DOWN,
	ACT_CLIMB_DISMOUNT,
	ACT_SHIPLADDER_UP,
	ACT_SHIPLADDER_DOWN,
	ACT_STRAFE_LEFT,
	ACT_STRAFE_RIGHT,
	ACT_ROLL_LEFT,
	ACT_ROLL_RIGHT,
	ACT_TURN_LEFT,
	ACT_TURN_RIGHT,
	ACT_CROUCH,
	ACT_CROUCHIDLE,
	ACT_STAND,
	ACT_USE,
	ACT_ALIEN_BURROW_IDLE,
	ACT_ALIEN_BURROW_OUT,
	ACT_SIGNAL1,
	ACT_SIGNAL2,
	ACT_SIGNAL3,
	ACT_SIGNAL_ADVANCE,
	ACT_SIGNAL_FORWARD,
	ACT_SIGNAL_GROUP,
	ACT_SIGNAL_HALT,
	ACT_SIGNAL_LEFT,
	ACT_SIGNAL_RIGHT,
	ACT_SIGNAL_TAKECOVER,
	ACT_LOOKBACK_RIGHT,
	ACT_LOOKBACK_LEFT,
	ACT_COWER,
	ACT_SMALL_FLINCH,
	ACT_BIG_FLINCH,
	ACT_MELEE_ATTACK1,
	ACT_MELEE_ATTACK2,
	ACT_RELOAD,
	ACT_RELOAD_START,
	ACT_RELOAD_FINISH,
	ACT_RELOAD_LOW,
	ACT_ARM,
	ACT_DISARM,
	ACT_DROP_WEAPON,
	ACT_DROP_WEAPON_SHOTGUN,
	ACT_PICKUP_GROUND,
	ACT_PICKUP_RACK,
	ACT_IDLE_ANGRY,
	ACT_IDLE_RELAXED,
	ACT_IDLE_STIMULATED,
	ACT_IDLE_AGITATED,
	ACT_IDLE_STEALTH,
	ACT_IDLE_HURT,
	ACT_WALK_RELAXED,
	ACT_WALK_STIMULATED,
	ACT_WALK_AGITATED,
	ACT_WALK_STEALTH,
	ACT_RUN_RELAXED,
	ACT_RUN_STIMULATED,
	ACT_RUN_AGITATED,
	ACT_RUN_STEALTH,
	ACT_IDLE_AIM_RELAXED,
	ACT_IDLE_AIM_STIMULATED,
	ACT_IDLE_AIM_AGITATED,
	ACT_IDLE_AIM_STEALTH,
	ACT_WALK_AIM_RELAXED,
	ACT_WALK_AIM_STIMULATED,
	ACT_WALK_AIM_AGITATED,
	ACT_WALK_AIM_STEALTH,
	ACT_RUN_AIM_RELAXED,
	ACT_RUN_AIM_STIMULATED,
	ACT_RUN_AIM_AGITATED,
	ACT_RUN_AIM_STEALTH,
	ACT_CROUCHIDLE_STIMULATED,
	ACT_CROUCHIDLE_AIM_STIMULATED,
	ACT_CROUCHIDLE_AGITATED,
	ACT_WALK_HURT,
	ACT_RUN_HURT,
	ACT_SPECIAL_ATTACK1,
	ACT_SPECIAL_ATTACK2,
	ACT_COMBAT_IDLE,
	ACT_WALK_SCARED,
	ACT_RUN_SCARED,
	ACT_VICTORY_DANCE,
	ACT_DIE_HEADSHOT,
	ACT_DIE_CHESTSHOT,
	ACT_DIE_GUTSHOT,
	ACT_DIE_BACKSHOT,
	ACT_FLINCH_HEAD,
	ACT_FLINCH_CHEST,
	ACT_FLINCH_STOMACH,
	ACT_FLINCH_LEFTARM,
	ACT_FLINCH_RIGHTARM,
	ACT_FLINCH_LEFTLEG,
	ACT_FLINCH_RIGHTLEG,
	ACT_FLINCH_PHYSICS,
	ACT_FLINCH_HEAD_BACK,
	ACT_FLINCH_HEAD_LEFT,
	ACT_FLINCH_HEAD_RIGHT,
	ACT_FLINCH_CHEST_BACK,
	ACT_FLINCH_STOMACH_BACK,
	ACT_FLINCH_CROUCH_FRONT,
	ACT_FLINCH_CROUCH_BACK,
	ACT_FLINCH_CROUCH_LEFT,
	ACT_FLINCH_CROUCH_RIGHT,
	ACT_IDLE_ON_FIRE,
	ACT_WALK_ON_FIRE,
	ACT_RUN_ON_FIRE,
	ACT_RAPPEL_LOOP,
	ACT_180_LEFT,
	ACT_180_RIGHT,
	ACT_90_LEFT,
	ACT_90_RIGHT,
	ACT_STEP_LEFT,
	ACT_STEP_RIGHT,
	ACT_STEP_BACK,
	ACT_STEP_FORE,
	ACT_GESTURE_RANGE_ATTACK1,
	ACT_GESTURE_RANGE_ATTACK2,
	ACT_GESTURE_MELEE_ATTACK1,
	ACT_GESTURE_MELEE_ATTACK2,
	ACT_GESTURE_RANGE_ATTACK1_LOW,
	ACT_GESTURE_RANGE_ATTACK2_LOW,
	ACT_MELEE_ATTACK_SWING_GESTURE,
	ACT_GESTURE_SMALL_FLINCH,
	ACT_GESTURE_BIG_FLINCH,
	ACT_GESTURE_FLINCH_BLAST,
	ACT_GESTURE_FLINCH_BLAST_SHOTGUN,
	ACT_GESTURE_FLINCH_BLAST_DAMAGED,
	ACT_GESTURE_FLINCH_BLAST_DAMAGED_SHOTGUN,
	ACT_GESTURE_FLINCH_HEAD,
	ACT_GESTURE_FLINCH_CHEST,
	ACT_GESTURE_FLINCH_STOMACH,
	ACT_GESTURE_FLINCH_LEFTARM,
	ACT_GESTURE_FLINCH_RIGHTARM,
	ACT_GESTURE_FLINCH_LEFTLEG,
	ACT_GESTURE_FLINCH_RIGHTLEG,
	ACT_GESTURE_TURN_LEFT,
	ACT_GESTURE_TURN_RIGHT,
	ACT_GESTURE_TURN_LEFT45,
	ACT_GESTURE_TURN_RIGHT45,
	ACT_GESTURE_TURN_LEFT90,
	ACT_GESTURE_TURN_RIGHT90,
	ACT_GESTURE_TURN_LEFT45_FLAT,
	ACT_GESTURE_TURN_RIGHT45_FLAT,
	ACT_GESTURE_TURN_LEFT90_FLAT,
	ACT_GESTURE_TURN_RIGHT90_FLAT,
	ACT_BARNACLE_HIT,
	ACT_BARNACLE_PULL,
	ACT_BARNACLE_CHOMP,
	ACT_BARNACLE_CHEW,
	ACT_DO_NOT_DISTURB,
	ACT_SPECIFIC_SEQUENCE,
	ACT_VM_DRAW,
	ACT_VM_HOLSTER,
	ACT_VM_IDLE,
	ACT_VM_FIDGET,
	ACT_VM_PULLBACK,
	ACT_VM_PULLBACK_HIGH,
	ACT_VM_PULLBACK_LOW,
	ACT_VM_THROW,
	ACT_VM_PULLPIN,
	ACT_VM_PRIMARYATTACK,
	ACT_VM_SECONDARYATTACK,
	ACT_VM_RELOAD,
	ACT_VM_DRYFIRE,
	ACT_VM_HITLEFT,
	ACT_VM_HITLEFT2,
	ACT_VM_HITRIGHT,
	ACT_VM_HITRIGHT2,
	ACT_VM_HITCENTER,
	ACT_VM_HITCENTER2,
	ACT_VM_MISSLEFT,
	ACT_VM_MISSLEFT2,
	ACT_VM_MISSRIGHT,
	ACT_VM_MISSRIGHT2,
	ACT_VM_MISSCENTER,
	ACT_VM_MISSCENTER2,
	ACT_VM_HAULBACK,
	ACT_VM_SWINGHARD,
	ACT_VM_SWINGMISS,
	ACT_VM_SWINGHIT,
	ACT_VM_IDLE_TO_LOWERED,
	ACT_VM_IDLE_LOWERED,
	ACT_VM_LOWERED_TO_IDLE,
	ACT_VM_RECOIL1,
	ACT_VM_RECOIL2,
	ACT_VM_RECOIL3,
	ACT_VM_PICKUP,
	ACT_VM_RELEASE,
	ACT_VM_ATTACH_SILENCER,
	ACT_VM_DETACH_SILENCER,
	ACT_VM_EMPTY_FIRE,
	ACT_VM_EMPTY_RELOAD,
	ACT_VM_EMPTY_DRAW,
	ACT_VM_EMPTY_IDLE,
	ACT_SLAM_STICKWALL_IDLE,
	ACT_SLAM_STICKWALL_ND_IDLE,
	ACT_SLAM_STICKWALL_ATTACH,
	ACT_SLAM_STICKWALL_ATTACH2,
	ACT_SLAM_STICKWALL_ND_ATTACH,
	ACT_SLAM_STICKWALL_ND_ATTACH2,
	ACT_SLAM_STICKWALL_DETONATE,
	ACT_SLAM_STICKWALL_DETONATOR_HOLSTER,
	ACT_SLAM_STICKWALL_DRAW,
	ACT_SLAM_STICKWALL_ND_DRAW,
	ACT_SLAM_STICKWALL_TO_THROW,
	ACT_SLAM_STICKWALL_TO_THROW_ND,
	ACT_SLAM_STICKWALL_TO_TRIPMINE_ND,
	ACT_SLAM_THROW_IDLE,
	ACT_SLAM_THROW_ND_IDLE,
	ACT_SLAM_THROW_THROW,
	ACT_SLAM_THROW_THROW2,
	ACT_SLAM_THROW_THROW_ND,
	ACT_SLAM_THROW_THROW_ND2,
	ACT_SLAM_THROW_DRAW,
	ACT_SLAM_THROW_ND_DRAW,
	ACT_SLAM_THROW_TO_STICKWALL,
	ACT_SLAM_THROW_TO_STICKWALL_ND,
	ACT_SLAM_THROW_DETONATE,
	ACT_SLAM_THROW_DETONATOR_HOLSTER,
	ACT_SLAM_THROW_TO_TRIPMINE_ND,
	ACT_SLAM_TRIPMINE_IDLE,
	ACT_SLAM_TRIPMINE_DRAW,
	ACT_SLAM_TRIPMINE_ATTACH,
	ACT_SLAM_TRIPMINE_ATTACH2,
	ACT_SLAM_TRIPMINE_TO_STICKWALL_ND,
	ACT_SLAM_TRIPMINE_TO_THROW_ND,
	ACT_SLAM_DETONATOR_IDLE,
	ACT_SLAM_DETONATOR_DRAW,
	ACT_SLAM_DETONATOR_DETONATE,
	ACT_SLAM_DETONATOR_HOLSTER,
	ACT_SLAM_DETONATOR_STICKWALL_DRAW,
	ACT_SLAM_DETONATOR_THROW_DRAW,
	ACT_SHOTGUN_RELOAD_START,
	ACT_SHOTGUN_RELOAD_FINISH,
	ACT_SHOTGUN_PUMP,
	ACT_SMG2_IDLE2,
	ACT_SMG2_FIRE2,
	ACT_SMG2_DRAW2,
	ACT_SMG2_RELOAD2,
	ACT_SMG2_DRYFIRE2,
	ACT_SMG2_TOAUTO,
	ACT_SMG2_TOBURST,
	ACT_PHYSCANNON_UPGRADE,
	ACT_RANGE_ATTACK_AR1,
	ACT_RANGE_ATTACK_AR2,
	ACT_RANGE_ATTACK_AR2_LOW,
	ACT_RANGE_ATTACK_AR2_GRENADE,
	ACT_RANGE_ATTACK_HMG1,
	ACT_RANGE_ATTACK_ML,
	ACT_RANGE_ATTACK_SMG1,
	ACT_RANGE_ATTACK_SMG1_LOW,
	ACT_RANGE_ATTACK_SMG2,
	ACT_RANGE_ATTACK_SHOTGUN,
	ACT_RANGE_ATTACK_SHOTGUN_LOW,
	ACT_RANGE_ATTACK_PISTOL,
	ACT_RANGE_ATTACK_PISTOL_LOW,
	ACT_RANGE_ATTACK_SLAM,
	ACT_RANGE_ATTACK_TRIPWIRE,
	ACT_RANGE_ATTACK_THROW,
	ACT_RANGE_ATTACK_SNIPER_RIFLE,
	ACT_RANGE_ATTACK_RPG,
	ACT_MELEE_ATTACK_SWING,
	ACT_RANGE_AIM_LOW,
	ACT_RANGE_AIM_SMG1_LOW,
	ACT_RANGE_AIM_PISTOL_LOW,
	ACT_RANGE_AIM_AR2_LOW,
	ACT_COVER_PISTOL_LOW,
	ACT_COVER_SMG1_LOW,
	ACT_GESTURE_RANGE_ATTACK_AR1,
	ACT_GESTURE_RANGE_ATTACK_AR2,
	ACT_GESTURE_RANGE_ATTACK_AR2_GRENADE,
	ACT_GESTURE_RANGE_ATTACK_HMG1,
	ACT_GESTURE_RANGE_ATTACK_ML,
	ACT_GESTURE_RANGE_ATTACK_SMG1,
	ACT_GESTURE_RANGE_ATTACK_SMG1_LOW,
	ACT_GESTURE_RANGE_ATTACK_SMG2,
	ACT_GESTURE_RANGE_ATTACK_SHOTGUN,
	ACT_GESTURE_RANGE_ATTACK_PISTOL,
	ACT_GESTURE_RANGE_ATTACK_PISTOL_LOW,
	ACT_GESTURE_RANGE_ATTACK_SLAM,
	ACT_GESTURE_RANGE_ATTACK_TRIPWIRE,
	ACT_GESTURE_RANGE_ATTACK_THROW,
	ACT_GESTURE_RANGE_ATTACK_SNIPER_RIFLE,
	ACT_GESTURE_MELEE_ATTACK_SWING,
	ACT_IDLE_RIFLE,
	ACT_IDLE_SMG1,
	ACT_IDLE_ANGRY_SMG1,
	ACT_IDLE_PISTOL,
	ACT_IDLE_ANGRY_PISTOL,
	ACT_IDLE_ANGRY_SHOTGUN,
	ACT_IDLE_STEALTH_PISTOL,
	ACT_IDLE_PACKAGE,
	ACT_WALK_PACKAGE,
	ACT_IDLE_SUITCASE,
	ACT_WALK_SUITCASE,
	ACT_IDLE_SMG1_RELAXED,
	ACT_IDLE_SMG1_STIMULATED,
	ACT_WALK_RIFLE_RELAXED,
	ACT_RUN_RIFLE_RELAXED,
	ACT_WALK_RIFLE_STIMULATED,
	ACT_RUN_RIFLE_STIMULATED,
	ACT_IDLE_AIM_RIFLE_STIMULATED,
	ACT_WALK_AIM_RIFLE_STIMULATED,
	ACT_RUN_AIM_RIFLE_STIMULATED,
	ACT_IDLE_SHOTGUN_RELAXED,
	ACT_IDLE_SHOTGUN_STIMULATED,
	ACT_IDLE_SHOTGUN_AGITATED,
	ACT_WALK_ANGRY,
	ACT_POLICE_HARASS1,
	ACT_POLICE_HARASS2,
	ACT_IDLE_MANNEDGUN,
	ACT_IDLE_MELEE,
	ACT_IDLE_ANGRY_MELEE,
	ACT_IDLE_RPG_RELAXED,
	ACT_IDLE_RPG,
	ACT_IDLE_ANGRY_RPG,
	ACT_COVER_LOW_RPG,
	ACT_WALK_RPG,
	ACT_RUN_RPG,
	ACT_WALK_CROUCH_RPG,
	ACT_RUN_CROUCH_RPG,
	ACT_WALK_RPG_RELAXED,
	ACT_RUN_RPG_RELAXED,
	ACT_WALK_RIFLE,
	ACT_WALK_AIM_RIFLE,
	ACT_WALK_CROUCH_RIFLE,
	ACT_WALK_CROUCH_AIM_RIFLE,
	ACT_RUN_RIFLE,
	ACT_RUN_AIM_RIFLE,
	ACT_RUN_CROUCH_RIFLE,
	ACT_RUN_CROUCH_AIM_RIFLE,
	ACT_RUN_STEALTH_PISTOL,
	ACT_WALK_AIM_SHOTGUN,
	ACT_RUN_AIM_SHOTGUN,
	ACT_WALK_PISTOL,
	ACT_RUN_PISTOL,
	ACT_WALK_AIM_PISTOL,
	ACT_RUN_AIM_PISTOL,
	ACT_WALK_STEALTH_PISTOL,
	ACT_WALK_AIM_STEALTH_PISTOL,
	ACT_RUN_AIM_STEALTH_PISTOL,
	ACT_RELOAD_PISTOL,
	ACT_RELOAD_PISTOL_LOW,
	ACT_RELOAD_SMG1,
	ACT_RELOAD_SMG1_LOW,
	ACT_RELOAD_SHOTGUN,
	ACT_RELOAD_SHOTGUN_LOW,
	ACT_GESTURE_RELOAD,
	ACT_GESTURE_RELOAD_PISTOL,
	ACT_GESTURE_RELOAD_SMG1,
	ACT_GESTURE_RELOAD_SHOTGUN,
	ACT_BUSY_LEAN_LEFT,
	ACT_BUSY_LEAN_LEFT_ENTRY,
	ACT_BUSY_LEAN_LEFT_EXIT,
	ACT_BUSY_LEAN_BACK,
	ACT_BUSY_LEAN_BACK_ENTRY,
	ACT_BUSY_LEAN_BACK_EXIT,
	ACT_BUSY_SIT_GROUND,
	ACT_BUSY_SIT_GROUND_ENTRY,
	ACT_BUSY_SIT_GROUND_EXIT,
	ACT_BUSY_SIT_CHAIR,
	ACT_BUSY_SIT_CHAIR_ENTRY,
	ACT_BUSY_SIT_CHAIR_EXIT,
	ACT_BUSY_STAND,
	ACT_BUSY_QUEUE,
	ACT_DUCK_DODGE,
	ACT_DIE_BARNACLE_SWALLOW,
	ACT_GESTURE_BARNACLE_STRANGLE,
	ACT_PHYSCANNON_DETACH,
	ACT_PHYSCANNON_ANIMATE,
	ACT_PHYSCANNON_ANIMATE_PRE,
	ACT_PHYSCANNON_ANIMATE_POST,
	ACT_DIE_FRONTSIDE,
	ACT_DIE_RIGHTSIDE,
	ACT_DIE_BACKSIDE,
	ACT_DIE_LEFTSIDE,
	ACT_DIE_CROUCH_FRONTSIDE,
	ACT_DIE_CROUCH_RIGHTSIDE,
	ACT_DIE_CROUCH_BACKSIDE,
	ACT_DIE_CROUCH_LEFTSIDE,
	ACT_OPEN_DOOR,
	ACT_DI_ALYX_ZOMBIE_MELEE,
	ACT_DI_ALYX_ZOMBIE_TORSO_MELEE,
	ACT_DI_ALYX_HEADCRAB_MELEE,
	ACT_DI_ALYX_ANTLION,
	ACT_DI_ALYX_ZOMBIE_SHOTGUN64,
	ACT_DI_ALYX_ZOMBIE_SHOTGUN26,
	ACT_READINESS_RELAXED_TO_STIMULATED,
	ACT_READINESS_RELAXED_TO_STIMULATED_WALK,
	ACT_READINESS_AGITATED_TO_STIMULATED,
	ACT_READINESS_STIMULATED_TO_RELAXED,
	ACT_READINESS_PISTOL_RELAXED_TO_STIMULATED,
	ACT_READINESS_PISTOL_RELAXED_TO_STIMULATED_WALK,
	ACT_READINESS_PISTOL_AGITATED_TO_STIMULATED,
	ACT_READINESS_PISTOL_STIMULATED_TO_RELAXED,
	ACT_IDLE_CARRY,
	ACT_WALK_CARRY,
	ACT_STARTDYING,
	ACT_DYINGLOOP,
	ACT_DYINGTODEAD,
	ACT_RIDE_MANNED_GUN,
	ACT_VM_SPRINT_ENTER,
	ACT_VM_SPRINT_IDLE,
	ACT_VM_SPRINT_LEAVE,
	ACT_FIRE_START,
	ACT_FIRE_LOOP,
	ACT_FIRE_END,
	ACT_CROUCHING_GRENADEIDLE,
	ACT_CROUCHING_GRENADEREADY,
	ACT_CROUCHING_PRIMARYATTACK,
	ACT_OVERLAY_GRENADEIDLE,
	ACT_OVERLAY_GRENADEREADY,
	ACT_OVERLAY_PRIMARYATTACK,
	ACT_OVERLAY_SHIELD_UP,
	ACT_OVERLAY_SHIELD_DOWN,
	ACT_OVERLAY_SHIELD_UP_IDLE,
	ACT_OVERLAY_SHIELD_ATTACK,
	ACT_OVERLAY_SHIELD_KNOCKBACK,
	ACT_SHIELD_UP,
	ACT_SHIELD_DOWN,
	ACT_SHIELD_UP_IDLE,
	ACT_SHIELD_ATTACK,
	ACT_SHIELD_KNOCKBACK,
	ACT_CROUCHING_SHIELD_UP,
	ACT_CROUCHING_SHIELD_DOWN,
	ACT_CROUCHING_SHIELD_UP_IDLE,
	ACT_CROUCHING_SHIELD_ATTACK,
	ACT_CROUCHING_SHIELD_KNOCKBACK,
	ACT_TURNRIGHT45,
	ACT_TURNLEFT45,
	ACT_TURN,
	ACT_OBJ_ASSEMBLING,
	ACT_OBJ_DISMANTLING,
	ACT_OBJ_STARTUP,
	ACT_OBJ_RUNNING,
	ACT_OBJ_IDLE,
	ACT_OBJ_PLACING,
	ACT_OBJ_DETERIORATING,
	ACT_OBJ_UPGRADING,
	ACT_DEPLOY,
	ACT_DEPLOY_IDLE,
	ACT_UNDEPLOY,
	ACT_CROSSBOW_DRAW_UNLOADED,
	ACT_GAUSS_SPINUP,
	ACT_GAUSS_SPINCYCLE,
	ACT_VM_PRIMARYATTACK_SILENCED,
	ACT_VM_RELOAD_SILENCED,
	ACT_VM_DRYFIRE_SILENCED,
	ACT_VM_IDLE_SILENCED,
	ACT_VM_DRAW_SILENCED,
	ACT_VM_IDLE_EMPTY_LEFT,
	ACT_VM_DRYFIRE_LEFT,
	ACT_VM_IS_DRAW,
	ACT_VM_IS_HOLSTER,
	ACT_VM_IS_IDLE,
	ACT_VM_IS_PRIMARYATTACK,
	ACT_PLAYER_IDLE_FIRE,
	ACT_PLAYER_CROUCH_FIRE,
	ACT_PLAYER_CROUCH_WALK_FIRE,
	ACT_PLAYER_WALK_FIRE,
	ACT_PLAYER_RUN_FIRE,
	ACT_IDLETORUN,
	ACT_RUNTOIDLE,
	ACT_VM_DRAW_DEPLOYED,
	ACT_HL2MP_IDLE_MELEE,
	ACT_HL2MP_RUN_MELEE,
	ACT_HL2MP_IDLE_CROUCH_MELEE,
	ACT_HL2MP_WALK_CROUCH_MELEE,
	ACT_HL2MP_GESTURE_RANGE_ATTACK_MELEE,
	ACT_HL2MP_GESTURE_RELOAD_MELEE,
	ACT_HL2MP_JUMP_MELEE,
	ACT_VM_FIZZLE,
	ACT_MP_STAND_IDLE,
	ACT_MP_CROUCH_IDLE,
	ACT_MP_CROUCH_DEPLOYED_IDLE,
	ACT_MP_CROUCH_DEPLOYED,
	ACT_MP_DEPLOYED_IDLE,
	ACT_MP_RUN,
	ACT_MP_WALK,
	ACT_MP_AIRWALK,
	ACT_MP_CROUCHWALK,
	ACT_MP_SPRINT,
	ACT_MP_JUMP,
	ACT_MP_JUMP_START,
	ACT_MP_JUMP_FLOAT,
	ACT_MP_JUMP_LAND,
	ACT_MP_JUMP_IMPACT_N,
	ACT_MP_JUMP_IMPACT_E,
	ACT_MP_JUMP_IMPACT_W,
	ACT_MP_JUMP_IMPACT_S,
	ACT_MP_JUMP_IMPACT_TOP,
	ACT_MP_DOUBLEJUMP,
	ACT_MP_SWIM,
	ACT_MP_DEPLOYED,
	ACT_MP_SWIM_DEPLOYED,
	ACT_MP_VCD,
	ACT_MP_ATTACK_STAND_PRIMARYFIRE,
	ACT_MP_ATTACK_STAND_PRIMARYFIRE_DEPLOYED,
	ACT_MP_ATTACK_STAND_SECONDARYFIRE,
	ACT_MP_ATTACK_STAND_GRENADE,
	ACT_MP_ATTACK_CROUCH_PRIMARYFIRE,
	ACT_MP_ATTACK_CROUCH_PRIMARYFIRE_DEPLOYED,
	ACT_MP_ATTACK_CROUCH_SECONDARYFIRE,
	ACT_MP_ATTACK_CROUCH_GRENADE,
	ACT_MP_ATTACK_SWIM_PRIMARYFIRE,
	ACT_MP_ATTACK_SWIM_SECONDARYFIRE,
	ACT_MP_ATTACK_SWIM_GRENADE,
	ACT_MP_ATTACK_AIRWALK_PRIMARYFIRE,
	ACT_MP_ATTACK_AIRWALK_SECONDARYFIRE,
	ACT_MP_ATTACK_AIRWALK_GRENADE,
	ACT_MP_RELOAD_STAND,
	ACT_MP_RELOAD_STAND_LOOP,
	ACT_MP_RELOAD_STAND_END,
	ACT_MP_RELOAD_CROUCH,
	ACT_MP_RELOAD_CROUCH_LOOP,
	ACT_MP_RELOAD_CROUCH_END,
	ACT_MP_RELOAD_SWIM,
	ACT_MP_RELOAD_SWIM_LOOP,
	ACT_MP_RELOAD_SWIM_END,
	ACT_MP_RELOAD_AIRWALK,
	ACT_MP_RELOAD_AIRWALK_LOOP,
	ACT_MP_RELOAD_AIRWALK_END,
	ACT_MP_ATTACK_STAND_PREFIRE,
	ACT_MP_ATTACK_STAND_POSTFIRE,
	ACT_MP_ATTACK_STAND_STARTFIRE,
	ACT_MP_ATTACK_CROUCH_PREFIRE,
	ACT_MP_ATTACK_CROUCH_POSTFIRE,
	ACT_MP_ATTACK_SWIM_PREFIRE,
	ACT_MP_ATTACK_SWIM_POSTFIRE,
	ACT_MP_STAND_PRIMARY,
	ACT_MP_CROUCH_PRIMARY,
	ACT_MP_RUN_PRIMARY,
	ACT_MP_WALK_PRIMARY,
	ACT_MP_AIRWALK_PRIMARY,
	ACT_MP_CROUCHWALK_PRIMARY,
	ACT_MP_JUMP_PRIMARY,
	ACT_MP_JUMP_START_PRIMARY,
	ACT_MP_JUMP_FLOAT_PRIMARY,
	ACT_MP_JUMP_LAND_PRIMARY,
	ACT_MP_SWIM_PRIMARY,
	ACT_MP_DEPLOYED_PRIMARY,
	ACT_MP_SWIM_DEPLOYED_PRIMARY,
	ACT_MP_ATTACK_STAND_PRIMARY,
	ACT_MP_ATTACK_STAND_PRIMARY_DEPLOYED,
	ACT_MP_ATTACK_CROUCH_PRIMARY,
	ACT_MP_ATTACK_CROUCH_PRIMARY_DEPLOYED,
	ACT_MP_ATTACK_SWIM_PRIMARY,
	ACT_MP_ATTACK_AIRWALK_PRIMARY,
	ACT_MP_RELOAD_STAND_PRIMARY,
	ACT_MP_RELOAD_STAND_PRIMARY_LOOP,
	ACT_MP_RELOAD_STAND_PRIMARY_END,
	ACT_MP_RELOAD_CROUCH_PRIMARY,
	ACT_MP_RELOAD_CROUCH_PRIMARY_LOOP,
	ACT_MP_RELOAD_CROUCH_PRIMARY_END,
	ACT_MP_RELOAD_SWIM_PRIMARY,
	ACT_MP_RELOAD_SWIM_PRIMARY_LOOP,
	ACT_MP_RELOAD_SWIM_PRIMARY_END,
	ACT_MP_RELOAD_AIRWALK_PRIMARY,
	ACT_MP_RELOAD_AIRWALK_PRIMARY_LOOP,
	ACT_MP_RELOAD_AIRWALK_PRIMARY_END,
	ACT_MP_ATTACK_STAND_GRENADE_PRIMARY,
	ACT_MP_ATTACK_CROUCH_GRENADE_PRIMARY,
	ACT_MP_ATTACK_SWIM_GRENADE_PRIMARY,
	ACT_MP_ATTACK_AIRWALK_GRENADE_PRIMARY,
	ACT_MP_STAND_SECONDARY,
	ACT_MP_CROUCH_SECONDARY,
	ACT_MP_RUN_SECONDARY,
	ACT_MP_WALK_SECONDARY,
	ACT_MP_AIRWALK_SECONDARY,
	ACT_MP_CROUCHWALK_SECONDARY,
	ACT_MP_JUMP_SECONDARY,
	ACT_MP_JUMP_START_SECONDARY,
	ACT_MP_JUMP_FLOAT_SECONDARY,
	ACT_MP_JUMP_LAND_SECONDARY,
	ACT_MP_SWIM_SECONDARY,
	ACT_MP_ATTACK_STAND_SECONDARY,
	ACT_MP_ATTACK_CROUCH_SECONDARY,
	ACT_MP_ATTACK_SWIM_SECONDARY,
	ACT_MP_ATTACK_AIRWALK_SECONDARY,
	ACT_MP_RELOAD_STAND_SECONDARY,
	ACT_MP_RELOAD_STAND_SECONDARY_LOOP,
	ACT_MP_RELOAD_STAND_SECONDARY_END,
	ACT_MP_RELOAD_CROUCH_SECONDARY,
	ACT_MP_RELOAD_CROUCH_SECONDARY_LOOP,
	ACT_MP_RELOAD_CROUCH_SECONDARY_END,
	ACT_MP_RELOAD_SWIM_SECONDARY,
	ACT_MP_RELOAD_SWIM_SECONDARY_LOOP,
	ACT_MP_RELOAD_SWIM_SECONDARY_END,
	ACT_MP_RELOAD_AIRWALK_SECONDARY,
	ACT_MP_RELOAD_AIRWALK_SECONDARY_LOOP,
	ACT_MP_RELOAD_AIRWALK_SECONDARY_END,
	ACT_MP_ATTACK_STAND_GRENADE_SECONDARY,
	ACT_MP_ATTACK_CROUCH_GRENADE_SECONDARY,
	ACT_MP_ATTACK_SWIM_GRENADE_SECONDARY,
	ACT_MP_ATTACK_AIRWALK_GRENADE_SECONDARY,
	ACT_MP_STAND_MELEE,
	ACT_MP_CROUCH_MELEE,
	ACT_MP_RUN_MELEE,
	ACT_MP_WALK_MELEE,
	ACT_MP_AIRWALK_MELEE,
	ACT_MP_CROUCHWALK_MELEE,
	ACT_MP_JUMP_MELEE,
	ACT_MP_JUMP_START_MELEE,
	ACT_MP_JUMP_FLOAT_MELEE,
	ACT_MP_JUMP_LAND_MELEE,
	ACT_MP_SWIM_MELEE,
	ACT_MP_ATTACK_STAND_MELEE,
	ACT_MP_ATTACK_STAND_MELEE_SECONDARY,
	ACT_MP_ATTACK_CROUCH_MELEE,
	ACT_MP_ATTACK_CROUCH_MELEE_SECONDARY,
	ACT_MP_ATTACK_SWIM_MELEE,
	ACT_MP_ATTACK_AIRWALK_MELEE,
	ACT_MP_ATTACK_STAND_GRENADE_MELEE,
	ACT_MP_ATTACK_CROUCH_GRENADE_MELEE,
	ACT_MP_ATTACK_SWIM_GRENADE_MELEE,
	ACT_MP_ATTACK_AIRWALK_GRENADE_MELEE,
	ACT_MP_STAND_ITEM1,
	ACT_MP_CROUCH_ITEM1,
	ACT_MP_RUN_ITEM1,
	ACT_MP_WALK_ITEM1,
	ACT_MP_AIRWALK_ITEM1,
	ACT_MP_CROUCHWALK_ITEM1,
	ACT_MP_JUMP_ITEM1,
	ACT_MP_JUMP_START_ITEM1,
	ACT_MP_JUMP_FLOAT_ITEM1,
	ACT_MP_JUMP_LAND_ITEM1,
	ACT_MP_SWIM_ITEM1,
	ACT_MP_ATTACK_STAND_ITEM1,
	ACT_MP_ATTACK_STAND_ITEM1_SECONDARY,
	ACT_MP_ATTACK_CROUCH_ITEM1,
	ACT_MP_ATTACK_CROUCH_ITEM1_SECONDARY,
	ACT_MP_ATTACK_SWIM_ITEM1,
	ACT_MP_ATTACK_AIRWALK_ITEM1,
	ACT_MP_STAND_ITEM2,
	ACT_MP_CROUCH_ITEM2,
	ACT_MP_RUN_ITEM2,
	ACT_MP_WALK_ITEM2,
	ACT_MP_AIRWALK_ITEM2,
	ACT_MP_CROUCHWALK_ITEM2,
	ACT_MP_JUMP_ITEM2,
	ACT_MP_JUMP_START_ITEM2,
	ACT_MP_JUMP_FLOAT_ITEM2,
	ACT_MP_JUMP_LAND_ITEM2,
	ACT_MP_SWIM_ITEM2,
	ACT_MP_ATTACK_STAND_ITEM2,
	ACT_MP_ATTACK_STAND_ITEM2_SECONDARY,
	ACT_MP_ATTACK_CROUCH_ITEM2,
	ACT_MP_ATTACK_CROUCH_ITEM2_SECONDARY,
	ACT_MP_ATTACK_SWIM_ITEM2,
	ACT_MP_ATTACK_AIRWALK_ITEM2,
	ACT_MP_GESTURE_FLINCH,
	ACT_MP_GESTURE_FLINCH_PRIMARY,
	ACT_MP_GESTURE_FLINCH_SECONDARY,
	ACT_MP_GESTURE_FLINCH_MELEE,
	ACT_MP_GESTURE_FLINCH_ITEM1,
	ACT_MP_GESTURE_FLINCH_ITEM2,
	ACT_MP_GESTURE_FLINCH_HEAD,
	ACT_MP_GESTURE_FLINCH_CHEST,
	ACT_MP_GESTURE_FLINCH_STOMACH,
	ACT_MP_GESTURE_FLINCH_LEFTARM,
	ACT_MP_GESTURE_FLINCH_RIGHTARM,
	ACT_MP_GESTURE_FLINCH_LEFTLEG,
	ACT_MP_GESTURE_FLINCH_RIGHTLEG,
	ACT_MP_GRENADE1_DRAW,
	ACT_MP_GRENADE1_IDLE,
	ACT_MP_GRENADE1_ATTACK,
	ACT_MP_GRENADE2_DRAW,
	ACT_MP_GRENADE2_IDLE,
	ACT_MP_GRENADE2_ATTACK,
	ACT_MP_PRIMARY_GRENADE1_DRAW,
	ACT_MP_PRIMARY_GRENADE1_IDLE,
	ACT_MP_PRIMARY_GRENADE1_ATTACK,
	ACT_MP_PRIMARY_GRENADE2_DRAW,
	ACT_MP_PRIMARY_GRENADE2_IDLE,
	ACT_MP_PRIMARY_GRENADE2_ATTACK,
	ACT_MP_SECONDARY_GRENADE1_DRAW,
	ACT_MP_SECONDARY_GRENADE1_IDLE,
	ACT_MP_SECONDARY_GRENADE1_ATTACK,
	ACT_MP_SECONDARY_GRENADE2_DRAW,
	ACT_MP_SECONDARY_GRENADE2_IDLE,
	ACT_MP_SECONDARY_GRENADE2_ATTACK,
	ACT_MP_MELEE_GRENADE1_DRAW,
	ACT_MP_MELEE_GRENADE1_IDLE,
	ACT_MP_MELEE_GRENADE1_ATTACK,
	ACT_MP_MELEE_GRENADE2_DRAW,
	ACT_MP_MELEE_GRENADE2_IDLE,
	ACT_MP_MELEE_GRENADE2_ATTACK,
	ACT_MP_ITEM1_GRENADE1_DRAW,
	ACT_MP_ITEM1_GRENADE1_IDLE,
	ACT_MP_ITEM1_GRENADE1_ATTACK,
	ACT_MP_ITEM1_GRENADE2_DRAW,
	ACT_MP_ITEM1_GRENADE2_IDLE,
	ACT_MP_ITEM1_GRENADE2_ATTACK,
	ACT_MP_ITEM2_GRENADE1_DRAW,
	ACT_MP_ITEM2_GRENADE1_IDLE,
	ACT_MP_ITEM2_GRENADE1_ATTACK,
	ACT_MP_ITEM2_GRENADE2_DRAW,
	ACT_MP_ITEM2_GRENADE2_IDLE,
	ACT_MP_ITEM2_GRENADE2_ATTACK,
	ACT_MP_STAND_BUILDING,
	ACT_MP_CROUCH_BUILDING,
	ACT_MP_RUN_BUILDING,
	ACT_MP_WALK_BUILDING,
	ACT_MP_AIRWALK_BUILDING,
	ACT_MP_CROUCHWALK_BUILDING,
	ACT_MP_JUMP_BUILDING,
	ACT_MP_JUMP_START_BUILDING,
	ACT_MP_JUMP_FLOAT_BUILDING,
	ACT_MP_JUMP_LAND_BUILDING,
	ACT_MP_SWIM_BUILDING,
	ACT_MP_ATTACK_STAND_BUILDING,
	ACT_MP_ATTACK_CROUCH_BUILDING,
	ACT_MP_ATTACK_SWIM_BUILDING,
	ACT_MP_ATTACK_AIRWALK_BUILDING,
	ACT_MP_ATTACK_STAND_GRENADE_BUILDING,
	ACT_MP_ATTACK_CROUCH_GRENADE_BUILDING,
	ACT_MP_ATTACK_SWIM_GRENADE_BUILDING,
	ACT_MP_ATTACK_AIRWALK_GRENADE_BUILDING,
	ACT_MP_STAND_PDA,
	ACT_MP_CROUCH_PDA,
	ACT_MP_RUN_PDA,
	ACT_MP_WALK_PDA,
	ACT_MP_AIRWALK_PDA,
	ACT_MP_CROUCHWALK_PDA,
	ACT_MP_JUMP_PDA,
	ACT_MP_JUMP_START_PDA,
	ACT_MP_JUMP_FLOAT_PDA,
	ACT_MP_JUMP_LAND_PDA,
	ACT_MP_SWIM_PDA,
	ACT_MP_ATTACK_STAND_PDA,
	ACT_MP_ATTACK_SWIM_PDA,
	ACT_MP_GESTURE_VC_HANDMOUTH,
	ACT_MP_GESTURE_VC_FINGERPOINT,
	ACT_MP_GESTURE_VC_FISTPUMP,
	ACT_MP_GESTURE_VC_THUMBSUP,
	ACT_MP_GESTURE_VC_NODYES,
	ACT_MP_GESTURE_VC_NODNO,
	ACT_MP_GESTURE_VC_HANDMOUTH_PRIMARY,
	ACT_MP_GESTURE_VC_FINGERPOINT_PRIMARY,
	ACT_MP_GESTURE_VC_FISTPUMP_PRIMARY,
	ACT_MP_GESTURE_VC_THUMBSUP_PRIMARY,
	ACT_MP_GESTURE_VC_NODYES_PRIMARY,
	ACT_MP_GESTURE_VC_NODNO_PRIMARY,
	ACT_MP_GESTURE_VC_HANDMOUTH_SECONDARY,
	ACT_MP_GESTURE_VC_FINGERPOINT_SECONDARY,
	ACT_MP_GESTURE_VC_FISTPUMP_SECONDARY,
	ACT_MP_GESTURE_VC_THUMBSUP_SECONDARY,
	ACT_MP_GESTURE_VC_NODYES_SECONDARY,
	ACT_MP_GESTURE_VC_NODNO_SECONDARY,
	ACT_MP_GESTURE_VC_HANDMOUTH_MELEE,
	ACT_MP_GESTURE_VC_FINGERPOINT_MELEE,
	ACT_MP_GESTURE_VC_FISTPUMP_MELEE,
	ACT_MP_GESTURE_VC_THUMBSUP_MELEE,
	ACT_MP_GESTURE_VC_NODYES_MELEE,
	ACT_MP_GESTURE_VC_NODNO_MELEE,
	ACT_MP_GESTURE_VC_HANDMOUTH_ITEM1,
	ACT_MP_GESTURE_VC_FINGERPOINT_ITEM1,
	ACT_MP_GESTURE_VC_FISTPUMP_ITEM1,
	ACT_MP_GESTURE_VC_THUMBSUP_ITEM1,
	ACT_MP_GESTURE_VC_NODYES_ITEM1,
	ACT_MP_GESTURE_VC_NODNO_ITEM1,
	ACT_MP_GESTURE_VC_HANDMOUTH_ITEM2,
	ACT_MP_GESTURE_VC_FINGERPOINT_ITEM2,
	ACT_MP_GESTURE_VC_FISTPUMP_ITEM2,
	ACT_MP_GESTURE_VC_THUMBSUP_ITEM2,
	ACT_MP_GESTURE_VC_NODYES_ITEM2,
	ACT_MP_GESTURE_VC_NODNO_ITEM2,
	ACT_MP_GESTURE_VC_HANDMOUTH_BUILDING,
	ACT_MP_GESTURE_VC_FINGERPOINT_BUILDING,
	ACT_MP_GESTURE_VC_FISTPUMP_BUILDING,
	ACT_MP_GESTURE_VC_THUMBSUP_BUILDING,
	ACT_MP_GESTURE_VC_NODYES_BUILDING,
	ACT_MP_GESTURE_VC_NODNO_BUILDING,
	ACT_MP_GESTURE_VC_HANDMOUTH_PDA,
	ACT_MP_GESTURE_VC_FINGERPOINT_PDA,
	ACT_MP_GESTURE_VC_FISTPUMP_PDA,
	ACT_MP_GESTURE_VC_THUMBSUP_PDA,
	ACT_MP_GESTURE_VC_NODYES_PDA,
	ACT_MP_GESTURE_VC_NODNO_PDA,
	ACT_VM_UNUSABLE,
	ACT_VM_UNUSABLE_TO_USABLE,
	ACT_VM_USABLE_TO_UNUSABLE,
	ACT_PRIMARY_VM_DRAW,
	ACT_PRIMARY_VM_HOLSTER,
	ACT_PRIMARY_VM_IDLE,
	ACT_PRIMARY_VM_PULLBACK,
	ACT_PRIMARY_VM_PRIMARYATTACK,
	ACT_PRIMARY_VM_SECONDARYATTACK,
	ACT_PRIMARY_VM_RELOAD,
	ACT_PRIMARY_VM_DRYFIRE,
	ACT_PRIMARY_VM_IDLE_TO_LOWERED,
	ACT_PRIMARY_VM_IDLE_LOWERED,
	ACT_PRIMARY_VM_LOWERED_TO_IDLE,
	ACT_SECONDARY_VM_DRAW,
	ACT_SECONDARY_VM_HOLSTER,
	ACT_SECONDARY_VM_IDLE,
	ACT_SECONDARY_VM_PULLBACK,
	ACT_SECONDARY_VM_PRIMARYATTACK,
	ACT_SECONDARY_VM_SECONDARYATTACK,
	ACT_SECONDARY_VM_RELOAD,
	ACT_SECONDARY_VM_DRYFIRE,
	ACT_SECONDARY_VM_IDLE_TO_LOWERED,
	ACT_SECONDARY_VM_IDLE_LOWERED,
	ACT_SECONDARY_VM_LOWERED_TO_IDLE,
	ACT_MELEE_VM_DRAW,
	ACT_MELEE_VM_HOLSTER,
	ACT_MELEE_VM_IDLE,
	ACT_MELEE_VM_PULLBACK,
	ACT_MELEE_VM_PRIMARYATTACK,
	ACT_MELEE_VM_SECONDARYATTACK,
	ACT_MELEE_VM_RELOAD,
	ACT_MELEE_VM_DRYFIRE,
	ACT_MELEE_VM_IDLE_TO_LOWERED,
	ACT_MELEE_VM_IDLE_LOWERED,
	ACT_MELEE_VM_LOWERED_TO_IDLE,
	ACT_PDA_VM_DRAW,
	ACT_PDA_VM_HOLSTER,
	ACT_PDA_VM_IDLE,
	ACT_PDA_VM_PULLBACK,
	ACT_PDA_VM_PRIMARYATTACK,
	ACT_PDA_VM_SECONDARYATTACK,
	ACT_PDA_VM_RELOAD,
	ACT_PDA_VM_DRYFIRE,
	ACT_PDA_VM_IDLE_TO_LOWERED,
	ACT_PDA_VM_IDLE_LOWERED,
	ACT_PDA_VM_LOWERED_TO_IDLE,
	ACT_ITEM1_VM_DRAW,
	ACT_ITEM1_VM_HOLSTER,
	ACT_ITEM1_VM_IDLE,
	ACT_ITEM1_VM_PULLBACK,
	ACT_ITEM1_VM_PRIMARYATTACK,
	ACT_ITEM1_VM_SECONDARYATTACK,
	ACT_ITEM1_VM_RELOAD,
	ACT_ITEM1_VM_DRYFIRE,
	ACT_ITEM1_VM_IDLE_TO_LOWERED,
	ACT_ITEM1_VM_IDLE_LOWERED,
	ACT_ITEM1_VM_LOWERED_TO_IDLE,
	ACT_ITEM2_VM_DRAW,
	ACT_ITEM2_VM_HOLSTER,
	ACT_ITEM2_VM_IDLE,
	ACT_ITEM2_VM_PULLBACK,
	ACT_ITEM2_VM_PRIMARYATTACK,
	ACT_ITEM2_VM_SECONDARYATTACK,
	ACT_ITEM2_VM_RELOAD,
	ACT_ITEM2_VM_DRYFIRE,
	ACT_ITEM2_VM_IDLE_TO_LOWERED,
	ACT_ITEM2_VM_IDLE_LOWERED,
	ACT_ITEM2_VM_LOWERED_TO_IDLE,
	ACT_RELOAD_SUCCEED,
	ACT_RELOAD_FAIL,
	ACT_WALK_AIM_AUTOGUN,
	ACT_RUN_AIM_AUTOGUN,
	ACT_IDLE_AUTOGUN,
	ACT_IDLE_AIM_AUTOGUN,
	ACT_RELOAD_AUTOGUN,
	ACT_CROUCH_IDLE_AUTOGUN,
	ACT_RANGE_ATTACK_AUTOGUN,
	ACT_JUMP_AUTOGUN,
	ACT_IDLE_AIM_PISTOL,
	ACT_WALK_AIM_DUAL,
	ACT_RUN_AIM_DUAL,
	ACT_IDLE_DUAL,
	ACT_IDLE_AIM_DUAL,
	ACT_RELOAD_DUAL,
	ACT_CROUCH_IDLE_DUAL,
	ACT_RANGE_ATTACK_DUAL,
	ACT_JUMP_DUAL,
	ACT_IDLE_SHOTGUN,
	ACT_IDLE_AIM_SHOTGUN,
	ACT_CROUCH_IDLE_SHOTGUN,
	ACT_JUMP_SHOTGUN,
	ACT_IDLE_AIM_RIFLE,
	ACT_RELOAD_RIFLE,
	ACT_CROUCH_IDLE_RIFLE,
	ACT_RANGE_ATTACK_RIFLE,
	ACT_JUMP_RIFLE,
	ACT_SLEEP,
	ACT_WAKE,
	ACT_FLICK_LEFT,
	ACT_FLICK_LEFT_MIDDLE,
	ACT_FLICK_RIGHT_MIDDLE,
	ACT_FLICK_RIGHT,
	ACT_SPINAROUND,
	ACT_PREP_TO_FIRE,
	ACT_FIRE,
	ACT_FIRE_RECOVER,
	ACT_SPRAY,
	ACT_PREP_EXPLODE,
	ACT_EXPLODE,
	ACT_DOTA_IDLE,
	ACT_DOTA_RUN,
	ACT_DOTA_ATTACK,
	ACT_DOTA_ATTACK_EVENT,
	ACT_DOTA_DIE,
	ACT_DOTA_FLINCH,
	ACT_DOTA_DISABLED,
	ACT_DOTA_CAST_ABILITY_1,
	ACT_DOTA_CAST_ABILITY_2,
	ACT_DOTA_CAST_ABILITY_3,
	ACT_DOTA_CAST_ABILITY_4,
	ACT_DOTA_OVERRIDE_ABILITY_1,
	ACT_DOTA_OVERRIDE_ABILITY_2,
	ACT_DOTA_OVERRIDE_ABILITY_3,
	ACT_DOTA_OVERRIDE_ABILITY_4,
	ACT_DOTA_CHANNEL_ABILITY_1,
	ACT_DOTA_CHANNEL_ABILITY_2,
	ACT_DOTA_CHANNEL_ABILITY_3,
	ACT_DOTA_CHANNEL_ABILITY_4,
	ACT_DOTA_CHANNEL_END_ABILITY_1,
	ACT_DOTA_CHANNEL_END_ABILITY_2,
	ACT_DOTA_CHANNEL_END_ABILITY_3,
	ACT_DOTA_CHANNEL_END_ABILITY_4,
	ACT_MP_RUN_SPEEDPAINT,
	ACT_MP_LONG_FALL,
	ACT_MP_TRACTORBEAM_FLOAT,
	ACT_MP_DEATH_CRUSH,
	ACT_MP_RUN_SPEEDPAINT_PRIMARY,
	ACT_MP_DROWNING_PRIMARY,
	ACT_MP_LONG_FALL_PRIMARY,
	ACT_MP_TRACTORBEAM_FLOAT_PRIMARY,
	ACT_MP_DEATH_CRUSH_PRIMARY,
	ACT_DIE_STAND,
	ACT_DIE_STAND_HEADSHOT,
	ACT_DIE_CROUCH,
	ACT_DIE_CROUCH_HEADSHOT,
	ACT_CSGO_NULL,
	ACT_CSGO_DEFUSE,
	ACT_CSGO_DEFUSE_WITH_KIT,
	ACT_CSGO_FLASHBANG_REACTION,
	ACT_CSGO_FIRE_PRIMARY,
	ACT_CSGO_FIRE_PRIMARY_OPT_1,
	ACT_CSGO_FIRE_PRIMARY_OPT_2,
	ACT_CSGO_FIRE_SECONDARY,
	ACT_CSGO_FIRE_SECONDARY_OPT_1,
	ACT_CSGO_FIRE_SECONDARY_OPT_2,
	ACT_CSGO_RELOAD,
	ACT_CSGO_RELOAD_START,
	ACT_CSGO_RELOAD_LOOP,
	ACT_CSGO_RELOAD_END,
	ACT_CSGO_OPERATE,
	ACT_CSGO_DEPLOY,
	ACT_CSGO_CATCH,
	ACT_CSGO_SILENCER_DETACH,
	ACT_CSGO_SILENCER_ATTACH,
	ACT_CSGO_TWITCH,
	ACT_CSGO_TWITCH_BUYZONE,
	ACT_CSGO_PLANT_BOMB,
	ACT_CSGO_IDLE_TURN_BALANCEADJUST,
	ACT_CSGO_IDLE_ADJUST_STOPPEDMOVING,
	ACT_CSGO_ALIVE_LOOP,
	ACT_CSGO_FLINCH,
	ACT_CSGO_FLINCH_HEAD,
	ACT_CSGO_FLINCH_MOLOTOV,
	ACT_CSGO_JUMP,
	ACT_CSGO_FALL,
	ACT_CSGO_CLIMB_LADDER,
	ACT_CSGO_LAND_LIGHT,
	ACT_CSGO_LAND_HEAVY,
	ACT_CSGO_EXIT_LADDER_TOP,
	ACT_CSGO_EXIT_LADDER_BOTTOM,
};
DWORD wait_on_mod_handle(std::string moduleName)
{
	DWORD ModuleHandle = NULL;
	while (!ModuleHandle)
	{
		ModuleHandle = (DWORD)GetModuleHandle(moduleName.c_str());
		if (!ModuleHandle)
			Sleep(50);
	}
	return ModuleHandle;
}
bool bCompare(const BYTE* Data, const BYTE* Mask, const char* szMask)
{
	for (; *szMask; ++szMask, ++Mask, ++Data)
	{
		if (*szMask == 'x' && *Mask != *Data)
		{
			return false;
		}
	}
	return (*szMask) == 0;
}
DWORD pattern(std::string moduleName, BYTE* Mask, char* szMask)
{
	DWORD Address = wait_on_mod_handle(moduleName.c_str());
	MODULEINFO ModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
	DWORD Length = ModInfo.SizeOfImage;
	for (DWORD c = 0; c < Length; c += 1)
	{
		if (bCompare((BYTE*)(Address + c), Mask, szMask))
		{
			return (DWORD)(Address + c);
		}
	}
	return 0;
}
void __stdcall Hooked_Frame_Stage_Notify(ClientFrameStage_t curStage)
{
	Hacks.LocalPlayer = Interfaces.pEntList->GetClientEntity(Interfaces.pEngine->GetLocalPlayer());
	if (Hacks.LocalPlayer)
	{
		LocalInfo.PunchAns = *Hacks.LocalPlayer->GetPunchAnglePtr();
		LocalInfo.Flags = Hacks.LocalPlayer->GetFlags();
	}

	ConVar* gitgud2 = Interfaces.g_ICVars->FindVar("mat_postprocess_enable");
	if (gitgud2)
	{
		if (Options.Visuals.VisualsMisc.bScopeRemover)
			gitgud2->SetValue(0);

		if (!Options.Visuals.VisualsMisc.bScopeRemover)
			gitgud2->SetValue(1);
	}

	static auto linegoesthrusmoke = pattern("client.dll", (PBYTE)"\x55\x8B\xEC\x83\xEC\x08\x8B\x15\x00\x00\x00\x00\x0F\x57\xC0", "xxxxxxxx????xxx");
	static auto smokecout = *(DWORD*)(linegoesthrusmoke + 0x8);

	static std::vector< const char* > smoke_materials = {
		"particle/vistasmokev1/vistasmokev1_smokegrenade",
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust",
		"particle/vistasmokev1/vistasmokev1_fire",
	};

	for (auto matName : smoke_materials) {
		IMaterial* mat = Interfaces.pMaterialSystem->FindMaterial(matName, "Other textures");
		mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, Options.Visuals.Esp.EspBox.NoSmoke);
	}


	if (Options.Visuals.Esp.EspBox.NoSmoke) {
		*(int*)(smokecout) = 0;
	}

	bool BreakingLagComp = false;
	bool ForceRealAA = false;
	bool SendPacket = true;
	bool ShowMenu = false;
	bool Opened = false;
	bool NewRound = false;
	bool Init = false;
	bool weaponfirecalled = false;
	bool playerhurtcalled = false;
	bool walkbotenabled = false;
	bool bShouldChoke = false;
	bool bFakewalking = false;
	int FakeDetection[64];
	Vector*pPunchAngle = nullptr, *pViewPunchAngle = nullptr, vPunchAngle, vViewPunchAngle;
	if (curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		if (Hacks.LocalPlayer->isAlive())
		{
			for (int i = 1; i < Interfaces.pGlobalVars->maxClients; i++)
			{
				if (i == Interfaces.pEngine->GetLocalPlayer())
					continue;
				CBaseEntity* pEntity = Interfaces.pEntList->GetClientEntity(i);

				if (pEntity && pEntity->GetHealth() > 0)
				{
					if (pEntity->IsDormant())
						continue;

					if (Options.Ragebot.Aimbot.ResolverTest)
					{
						if (weaponfirecalled)
						{
							if (!playerhurtcalled)
							{
								if (isPartOf("Brute", pResolverData[i].resolvermode))
								{
									pResolverData[i].addyaw += 65.f;
									Misc::Normalize_y(pResolverData[i].addyaw);
								}
							}
							else
								playerhurtcalled = false;
							weaponfirecalled = false;
						}

						for (int w = 0; w < 13; w++)
						{
							AnimationLayer currentLayer = pEntity->GetAnimOverlay(w);
							const int activity = pEntity->GetSequenceActivity(currentLayer.m_nSequence);
							float flcycle = currentLayer.m_flCycle, flprevcycle = currentLayer.m_flPrevCycle, flweight = currentLayer.m_flWeight, flweightdatarate = currentLayer.m_flWeightDeltaRate;
							uint32_t norder = currentLayer.m_nOrder;
							Vector* pAngles = pEntity->GetEyeAnglesPointer();


							if (activity == ACT_CSGO_IDLE_TURN_BALANCEADJUST && flweight <= 1.0f && flcycle <= 0.851166f) //[06:40AM] == > Activity > 979, Weight > 0.494118, Cycle > 0.851166
							{
								pResolverData[pEntity->GetIndex()].resolvermode = "Less BA Brute";
								FakeDetection[i] = 1;
								pAngles->y = pResolverData[pEntity->GetIndex()].lastmovinglby;
								pResolverData[pEntity->GetIndex()].addyaw > 0.f ? pAngles->y = pEntity->pelvisangs() - pResolverData[pEntity->GetIndex()].addyaw - 77.5 : pAngles->y = pEntity->pelvisangs() - 77.5;
							}

							else if (activity == ACT_CSGO_IDLE_ADJUST_STOPPEDMOVING || activity == ACT_CSGO_IDLE_TURN_BALANCEADJUST && flweight == 0.000000f && flcycle >= 0.955994) // High delta
							{
								pResolverData[pEntity->GetIndex()].resolvermode = "LBY Brute";
								FakeDetection[i] = 2;
								pAngles->y = pResolverData[pEntity->GetIndex()].lastmovinglby;
								pResolverData[pEntity->GetIndex()].addyaw > 0.f ? pAngles->y = pEntity->pelvisangs() - pResolverData[pEntity->GetIndex()].addyaw : pAngles->y = pEntity->pelvisangs() - 17.5;
							}

							pAngles->y = pEntity->pelvisangs();

							Misc::NormalizeVector(*pAngles);
						}
					}
				}
			}
		}
	}
	if (Interfaces.pEngine->IsConnected() && Interfaces.pEngine->IsInGame() && curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		CBaseEntity *pLocal = Interfaces.pEntList->GetClientEntity(Interfaces.pEngine->GetLocalPlayer());
		auto weapons = pLocal->GetActiveBaseCombatWeapon();
		//  for (int i = 0; weapons[i] != INVALID_EHANDLE_INDEX; i++)
		for (int i = 0; i <= Interfaces.pEntList->GetHighestEntityIndex(); i++) // CHANGE
		{
			CBaseEntity *pEntity = Interfaces.pEntList->GetClientEntity(i);

			if (pEntity)
			{
				ULONG hOwnerEntity = *(PULONG)((DWORD)pEntity + 0x148);
				CBaseEntity* pOwner = Interfaces.pEntList->GetClientEntityFromHandle((HANDLE)hOwnerEntity);
				ApplyCustomGloves();
				if (pOwner)
				{
					if (pOwner == pLocal)
					{
						std::string sWeapon = Interfaces.g_pModelInfo->GetModelName(pEntity->GetModel());

						auto weps = pLocal->Weapons();
						for (size_t i = 0; weps[i] != nullptr; i++)
						{
							auto pWeapons = reinterpret_cast<CBaseCombatWeapon*>(Interfaces.pEntList->GetClientEntityFromHandle(weps[i]));

							// if (sWeapon.find("models/weapons", 0) != std::string::npos)
							//      continue;



							CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)pEntity;
							if (Options.Skins.bEnableSkins)
							{
								int weapon = *pWeapon->GetItemDefinitionIndex();
								int weaponmodel = Options.Skins.WeaponModel;
								int weaponmodel1 = Options.Skins.WeaponModel1;
								int weaponmodel2 = Options.Skins.WeaponModel2;
								int weaponmodel3 = Options.Skins.WeaponModel3;
								int weaponmodel4 = Options.Skins.WeaponModel4;
								int weaponmodel5 = Options.Skins.WeaponModel5;
								int weaponmodel6 = Options.Skins.WeaponModel6;
								int weaponmodel7 = Options.Skins.WeaponModel7;
								int weaponmodel8 = Options.Skins.WeaponModel8;
								int weaponmodel9 = Options.Skins.WeaponModel9;
								int weaponmodel10 = Options.Skins.WeaponModel10;
								int weaponmodel11 = Options.Skins.WeaponModel11;
								int weaponmodel12 = Options.Skins.WeaponModel12;
								int weaponmodel13 = Options.Skins.WeaponModel13;
								int weaponmodel14 = Options.Skins.WeaponModel14;
								int weaponmodel15 = Options.Skins.WeaponModel15;
								int weaponmodel16 = Options.Skins.WeaponModel16;
								int weaponmodel17 = Options.Skins.WeaponModel17;
								int weaponmodel18 = Options.Skins.WeaponModel18;

								switch (weapon)
								{
	
								case WEAPON_AK47:
								{// AK47

										*pWeapon->GetFallbackPaintKit() = Options.Skins.ChangerSkin;
								}
								break;
								case weapon_m4a1_silencer:
								{// AK47

									*pWeapon->GetFallbackPaintKit() = Options.Skins.ChangerSkin1;



								}
								break;
								case WEAPON_M4A1:
								{// AK47

									*pWeapon->GetFallbackPaintKit() = Options.Skins.ChangerSkin2;



								}
								break;
								case WEAPON_AUG:
								{// AK47

									*pWeapon->GetFallbackPaintKit() = Options.Skins.ChangerSkin3;



								}
								break;
								case WEAPON_FAMAS:
								{// AK47

									*pWeapon->GetFallbackPaintKit() = Options.Skins.ChangerSkin4;



								}
								break;
								case WEAPON_GALIL:
								{// AK47

									*pWeapon->GetFallbackPaintKit() = Options.Skins.ChangerSkin5;



								}
								break;
								case WEAPON_SG553:
								{// AK47

									*pWeapon->GetFallbackPaintKit() = Options.Skins.ChangerSkin6;



								}
								break;
								case WEAPON_AWP:
								{// AK47

									*pWeapon->GetFallbackPaintKit() = Options.Skins.ChangerSkin7;



								}
								break;
								case WEAPON_SSG08:
								{// AK47

									*pWeapon->GetFallbackPaintKit() = Options.Skins.ChangerSkin8;



								}
								break;
								case WEAPON_SCAR20:
								{// AK47

									*pWeapon->GetFallbackPaintKit() = Options.Skins.ChangerSkin9;



								}
								break;
								case WEAPON_G3SG1:
								{// AK47

									*pWeapon->GetFallbackPaintKit() = Options.Skins.ChangerSkin10;



								}
								break;
								case WEAPON_GLOCK:
								{// AK47

									*pWeapon->GetFallbackPaintKit() = Options.Skins.ChangerSkin11;



								}
								break;
								case weapon_usp_silencer:
								{// AK47

									*pWeapon->GetFallbackPaintKit() = Options.Skins.ChangerSkin12;



								}
								break;
								case WEAPON_DEAGLE:
								{// AK47

									*pWeapon->GetFallbackPaintKit() = Options.Skins.ChangerSkin13;



								}
								break;
								case WEAPON_DUALBERETTA:
								{// AK47

									*pWeapon->GetFallbackPaintKit() = Options.Skins.ChangerSkin14;



								}
								break;
								case weapon_revolver:
								{// AK47
									*pWeapon->GetFallbackPaintKit() = Options.Skins.ChangerSkin15;



								}
								break;
								case WEAPON_FIVESEVEN:
								{// AK47

									*pWeapon->GetFallbackPaintKit() = Options.Skins.ChangerSkin16;



								}
								break;
								case WEAPON_USP:
								{// AK47

									*pWeapon->GetFallbackPaintKit() = Options.Skins.ChangerSkin17;



								}
								break;
								case WEAPON_P250:
								{// AK47

									*pWeapon->GetFallbackPaintKit() = Options.Skins.ChangerSkin18;



								}
								break;
								default:
									break;

								}
								
								if (pEntity->GetClientClass()->m_ClassID == (int)CSGOClassID::CKnife)
								{
									if (Options.Skins.knife)
									{
										int KnifeModel = Options.Skins.Knifemodels;
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
										if (KnifeModel == 0)
										{
											*pWeapon->GetModelIndex() = iBayonet;
											*pWeapon->GetViewModelIndex() = iBayonet;
											*pWeapon->GetWorldModelIndex() = iBayonet + 1;
											*pWeapon->GetItemDefinitionIndex() = 500;
											*pWeapon->GetEntityQuality() = 3;
											*pWeapon->GetFallbackPaintKit() = Options.Skins.ChangerSkin19;
										}
										else if (KnifeModel == 1)
										{
											*pWeapon->GetModelIndex() = iBowie;
											*pWeapon->GetViewModelIndex() = iBowie;
											*pWeapon->GetWorldModelIndex() = iBowie + 1;
											*pWeapon->GetItemDefinitionIndex() = 514;
											*pWeapon->GetEntityQuality() = 3;
											*pWeapon->GetFallbackPaintKit() = Options.Skins.ChangerSkin19;
										}
										else if (KnifeModel == 2)
										{
											*pWeapon->GetModelIndex() = iButterfly;
											*pWeapon->GetViewModelIndex() = iButterfly;
											*pWeapon->GetWorldModelIndex() = iButterfly + 1;
											*pWeapon->GetItemDefinitionIndex() = 515;
											*pWeapon->GetEntityQuality() = 3;
											*pWeapon->GetFallbackPaintKit() = Options.Skins.ChangerSkin19;
										}
										else if (KnifeModel == 3)
										{
											*pWeapon->GetModelIndex() = iFalchion;
											*pWeapon->GetViewModelIndex() = iFalchion;
											*pWeapon->GetWorldModelIndex() = iFalchion + 1;
											*pWeapon->GetItemDefinitionIndex() = 512;
											*pWeapon->GetEntityQuality() = 3;
											*pWeapon->GetFallbackPaintKit() = Options.Skins.ChangerSkin19;
										}
										else if (KnifeModel == 4)
										{
											*pWeapon->GetModelIndex() = iFlip;
											*pWeapon->GetViewModelIndex() = iFlip;
											*pWeapon->GetWorldModelIndex() = iFlip + 1;
											*pWeapon->GetItemDefinitionIndex() = 505;
											*pWeapon->GetEntityQuality() = 3;
											*pWeapon->GetFallbackPaintKit() = Options.Skins.ChangerSkin19;
										}
										else if (KnifeModel == 5)
										{
											*pWeapon->GetModelIndex() = iGut;
											*pWeapon->GetViewModelIndex() = iGut;
											*pWeapon->GetWorldModelIndex() = iGut + 1;
											*pWeapon->GetItemDefinitionIndex() = 506;
											*pWeapon->GetEntityQuality() = 3;
											*pWeapon->GetFallbackPaintKit() = Options.Skins.ChangerSkin19;
										}
										else if (KnifeModel == 6)
										{
											*pWeapon->GetModelIndex() = iHuntsman;
											*pWeapon->GetViewModelIndex() = iHuntsman;
											*pWeapon->GetWorldModelIndex() = iHuntsman + 1;
											*pWeapon->GetItemDefinitionIndex() = 509;
											*pWeapon->GetEntityQuality() = 3;
											*pWeapon->GetFallbackPaintKit() = Options.Skins.ChangerSkin19;
										}
										else if (KnifeModel == 7)
										{
											*pWeapon->GetModelIndex() = iKarambit;
											*pWeapon->GetViewModelIndex() = iKarambit;
											*pWeapon->GetWorldModelIndex() = iKarambit + 1;
											*pWeapon->GetItemDefinitionIndex() = 507;
											*pWeapon->GetEntityQuality() = 3;
											*pWeapon->GetFallbackPaintKit() = Options.Skins.ChangerSkin19;
										}
										else if (KnifeModel == 8)
										{
											*pWeapon->GetModelIndex() = iM9Bayonet;
											*pWeapon->GetViewModelIndex() = iM9Bayonet;
											*pWeapon->GetWorldModelIndex() = iM9Bayonet + 1;
											*pWeapon->GetItemDefinitionIndex() = 508;
											*pWeapon->GetEntityQuality() = 3;
											*pWeapon->GetFallbackPaintKit() = Options.Skins.ChangerSkin19;
										}
										else if (KnifeModel == 9)
										{
											*pWeapon->GetModelIndex() = iDagger;
											*pWeapon->GetViewModelIndex() = iDagger;
											*pWeapon->GetWorldModelIndex() = iDagger + 1;
											*pWeapon->GetItemDefinitionIndex() = 516;
											*pWeapon->GetEntityQuality() = 3;
											*pWeapon->GetFallbackPaintKit() = Options.Skins.ChangerSkin19;
										}
									}


								}
								if (Options.Skins.startrak)
								{
									*pWeapon->GetOriginalOwnerXuidHigh() = 0;
									*pWeapon->GetOriginalOwnerXuidLow() = 0;
									*pWeapon->GetFallbackWear() = 0.001f;
									*pWeapon->GetItemIDHigh() = 1;
									*pWeapon->GetFallbackStatTrak() = Options.Skins.stat;

								}
								else
								{
									*pWeapon->GetOriginalOwnerXuidHigh() = 0;
									*pWeapon->GetOriginalOwnerXuidLow() = 0;
									*pWeapon->GetFallbackWear() = 0.001f;
									*pWeapon->GetItemIDHigh() = 1;
								}

							}
						}
					}
				}
			}
		}
	}

	if (Interfaces.pEngine->IsInGame() && Interfaces.pEngine->IsConnected())
	{
		if (Hacks.LocalPlayer)
		{
			Hacks.LocalWeapon = Hacks.LocalPlayer->GetActiveBaseCombatWeapon();
			if (curStage == FRAME_RENDER_START)
			{
				MEMCHECK;
				if (Options.Visuals.VisualsMisc.bNoVisualRecoil)
				{
					pPunchAngle = Hacks.LocalPlayer->GetPunchAnglePtr();
					pViewPunchAngle = Hacks.LocalPlayer->GetViewPunchAnglePtr();

					if (pPunchAngle && pViewPunchAngle)
					{
						vPunchAngle = *pPunchAngle;
						pPunchAngle->Init();
						vViewPunchAngle = *pViewPunchAngle;
						pViewPunchAngle->Init();
					}
				}

				//DWORD deadflag = Netvar("DT_BasePlayer", "deadflag");

				static bool enabledtp = false;
				static bool check = false;
				if (GetAsyncKeyState(Options.Visuals.VisualsMisc.iThirdPersonKey))
				{
					if (!check)
						enabledtp = !enabledtp;
					check = true;
				}
				else
					check = false;

				if (Options.Visuals.VisualsMisc.iThirdPerson >= 1 && enabledtp && Hacks.LocalPlayer->isAlive())
				{
					float flThirdPersonRange = 141; //Options.Visuals.VisualsMisc.flThirdPersonRange;

					*(bool*)((DWORD)Interfaces.pInput + 0xA5) = true;
					*(float*)((DWORD)Interfaces.pInput + 0xA8 + 0x8) = flThirdPersonRange;
				}
				else
				{
					*(bool*)((DWORD)Interfaces.pInput + 0xA5) = false;
					*(float*)((DWORD)Interfaces.pInput + 0xA8 + 0x8);
				}

				/*CInput::CUserCmd* cmd;
				static Vector vecAngles;
				Interfaces.pEngine->GetViewAngles(vecAngles);
				if (Options.Visuals.VisualsMisc.iThirdPerson >= 1)
				{
					if (!cmd->m_fCameraInThirdPerson)
					{
						cmd->m_fCameraInThirdPerson = true;
						cmd->m_vecCameraOffset = Vector(vecAngles.x, vecAngles.y, Options.Visuals.VisualsMisc.flThirdPersonRange);
					}
				}
				else
				{
					cmd->m_fCameraInThirdPerson = false;
					cmd->m_vecCameraOffset = Vector(vecAngles.x, vecAngles.y, 0);
				}*/


				//if (Options.Visuals.VisualsMisc.bNoFlash)
				//	*(Vector*)((DWORD)Hacks.LocalPlayer + Netvar("DT_BasePlayer", "deadflag") + 0x4) = Global::LastAngle;

				// PVS Fix

				for (int i = 1; i <= Interfaces.pGlobalVars->maxClients; i++)
				{
					if (i == Interfaces.pEngine->GetLocalPlayer()) continue;

					CBaseEntity* pMemeEnt = Interfaces.pEntList->GetClientEntity(i);
					if (!pMemeEnt) continue;

					*(int*)((uintptr_t)pMemeEnt + 0xA30) = Interfaces.pGlobalVars->framecount; //we'll skip occlusion checks now
					*(int*)((uintptr_t)pMemeEnt + 0xA28) = 0;//clear occlusion flags
				}
				for (auto matName : smoke_materials)
				{
					IMaterial* mat = Interfaces.pMaterialSystem->FindMaterial(matName, "Other textures");
					mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, Options.Visuals.Esp.EspBox.NoSmoke);
				}


			}
		}
		if (*reinterpret_cast<bool*>(reinterpret_cast<DWORD>(Interfaces.pInput) + 0xA5))
			*reinterpret_cast<Vector*>(reinterpret_cast<DWORD>(Hacks.LocalPlayer) + 0x31C8) = Hacks.LastAngles;

	}

	Hacks.oFrameStageNotify(curStage);

	if (pPunchAngle && pViewPunchAngle)
	{
		*pPunchAngle = vPunchAngle;
		*pViewPunchAngle = vViewPunchAngle;
	}
	float MoveYaw[64]; //lby array
	if (Options.Ragebot.HvH.bResolver)
	{
		//spin += 120;
		//if( spin > 180 )
		//	spin -= 360;
		plist.Update();
		if (curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
		{
			if (Interfaces.pEngine->IsInGame() && Interfaces.pEngine->IsConnected())
			{
				for (auto i = 0; i <= Interfaces.pEntList->GetHighestEntityIndex(); i++)
				{
					auto pEntity = static_cast<CBaseEntity*>(Interfaces.pEntList->GetClientEntity(i));
					if (pEntity == nullptr)
						continue;
					if (pEntity == Hacks.LocalPlayer)
						continue;
					if (!Hacks.LocalPlayer->isAlive())
						continue;
					if (pEntity->GetTeam() == Hacks.LocalPlayer->GetTeam())
						continue;
					if (!pEntity->isAlive())
						continue;
					if (!(pEntity->GetHealth() > 0))
						continue;
					if (pEntity->IsDormant())
						continue;
					player_info_t info;
					if (!(Interfaces.pEngine->GetPlayerInfo(pEntity->GetIndex(), &info)))
						continue;
					PlayerList::CPlayer* Player = plist.FindPlayer(pEntity);
					Player->entity = pEntity;

					Vector* angs = Player->entity->GetEyeAnglesPointer();
					if (Options.Ragebot.HvH.ForceAngles)
					{
							CBaseEntity *pEntity = Interfaces.pEntList->GetClientEntity(i);
							if (pEntity->GetVecVelocity().Length2D() > 1)
							{

								angs->y = pEntity->pelvisangs();

								MoveYaw[i] = pEntity->pelvisangs();
							}
							else
							{
								angs->y = MoveYaw[i];
							}
				    }
					if (Options.Ragebot.HvH.bResolver2 )
					{
						int tick = Hacks.LocalPlayer->GetTickBase();
						float flServerTime = (tick * Interfaces.pGlobalVars->interval_per_tick);
						float LastLBYUpdateTime = flServerTime - GetOutgoingLatency();
						if (((LastLBYUpdateTime + 1.1 - GetOutgoingLatency()) <= flServerTime) && (Hacks.LocalPlayer->GetFlags() & FL_ONGROUND))
						{
							LastLBYUpdateTime += 1.1;
							Global::lbyupdate = true;
						}
						Global::lbyupdate = false;

						Vector* eYaw = pEntity->GetEyeAnglesPointer();
						float flResolveyaw;

						float lastLBY[64];
						float lbyDif[64];

						float curLbyYaw = pEntity->pelvisangs();

						if (pEntity->GetVecVelocity().Length2D() > 0)
							eYaw->y = pEntity->pelvisangs();
						else
						{
							if (!IsNearEqual(lastLBY[pEntity->GetIndex()], curLbyYaw, 15.f))
							{
								lbyDif[pEntity->GetIndex()] = curLbyYaw - eYaw->y;
								lastLBY[pEntity->GetIndex()] = curLbyYaw;
								eYaw->y += pEntity->pelvisangs();
							}
							else
							{
								eYaw->y += lbyDif[pEntity->GetIndex()];
							}
						}

						PlayerList::CPlayer* Player = plist.FindPlayer(pEntity);

						Player->resolved = eYaw->y;
						AutoResolver(angs, Player);

					}

					 if (Options.Ragebot.HvH.delta)
					{


						 static auto spin = 0;
						 static float TimedYaw;
						 static float OldYaw;
						 static float CurYaw;
						 static float curlwer;
						 static float curlwerr;

						static float lowerDelta[64];
						static float lastYaw[64];
						curlwer = pEntity->pelvisangs();
						curlwerr = pEntity->GetEyeAngles().y;
						//curlwerr = pEnt->GetEyeAngles().y;
						float curLower = OldYaw;
						float curYaw = curlwerr;

						lowerDelta[i] = curYaw - curLower;
						//U::Normalize(lowerDelta[i]); //wat? normalize float

						if (fabs(lowerDelta[i]) > 15.f)
						{
							if (pEntity->GetVecVelocity().Length2D() > 0)
							{
								pEntity->GetEyeAngles().y - pEntity->pelvisangs();
							}
							else
							{
								if (curYaw != lastYaw[i])
								{
									curlwerr += lowerDelta[i];
									lastYaw[i] = curYaw;
								}
							}

						}
						AutoResolver(angs, Player);
					}

					if (!Options.Misc.Misc2.bAntiUntrusted)
					{
						if (angs->x > 90.0f && angs->x < 270.f)
						{
							if (angs->x > 180.f)
								angs->x = -89.f;
							else
								angs->x = 89.0f;
						}
						//angs->y = spin;
					}
					AutoResolver(angs, Player);
				/*	Vector AimAngles;
					Misc::CalcAngle(Player->entity->GetEyePosition(), Hacks.LocalPlayer->GetVecOrigin(), AimAngles);
					if (AimAngles.IsValid ())
					{
					if (Player->entity->GetFlags() & FL_ONGROUND)
					Player->flEyeAng = Player->entity->pelvisangs() - AimAngles.y;
					angs->y = Player->flEyeAng + AimAngles.y;
					}*/
				}
			}
		}
	}

	/*if (Options.Ragebot.HvH.iAntiAimResolver > 0)
	{
		if (!Interfaces.pEngine->IsInGame() && Interfaces.pEngine->IsConnected())
			return;

		static auto spin = 0;
		static float TimedYaw;
		static float OldYaw;
		static float CurYaw;
		static float curlwer;
		static float curlwerr;

		//spin += 120;
		//if( spin > 180 )
		//	spin -= 360;
		plist.Update();
		if (curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
		{
			if (Interfaces.pEngine->IsInGame() && Interfaces.pEngine->IsConnected())
			{
				for (auto i = 0; i <= Interfaces.pEntList->GetHighestEntityIndex(); i++)
				{
					auto pEntity = static_cast< CBaseEntity* >(Interfaces.pEntList->GetClientEntity(i));
					if (pEntity == nullptr)
						continue;
					if (pEntity == Hacks.LocalPlayer)
						continue;
					if (!Hacks.LocalPlayer->isAlive())
						continue;
					if (pEntity->GetTeam() == Hacks.LocalPlayer->GetTeam())
						continue;
					if (!pEntity->isAlive())
						continue;
					if (!(pEntity->GetHealth() > 0))
						continue;
					if (pEntity->IsDormant())
						continue;

					player_info_t info;
					if (!(Interfaces.pEngine->GetPlayerInfo(pEntity->GetIndex(), &info)))
						continue;
					PlayerList::CPlayer* Player = plist.FindPlayer(pEntity);
					Player->entity = pEntity;

					Vector* angs = Player->entity->GetEyeAnglesPointer();

					if (!Options.Misc.Misc2.bAntiUntrusted)
					{
						if (angs->x > 90.0f || angs->x < 270.f || angs->x > 1080.f)
						{
							if (angs->x >= 180.f || angs->x >= 90.f || angs->x >= 1080.f || angs->x >= 991.f)
								angs->x = 89.f;
							else if (angs->x <= -180.f || angs->x <= -90.f || angs->x <= -1080.f || angs->x <= -991.f)
								angs->x = -89.f;
							else
								angs->x = 89.0f;
						}
						//angs->y = spin;
					}
					else
					{
						if (angs->x > 89.f || angs->x < -89.f)
						{
							if (angs->x >= -89.f || angs->x <= -89.f)
								angs->x = -89.f;
							else if (angs->x >= 89.f || angs->x <= 89.f)
								angs->x = 89.f;
							else
								angs->x = 0;
						}
						//angs->y = spin;
					}

					if (Options.Ragebot.HvH.iAntiAimResolver == 1)
					{
						int tick = Hacks.LocalPlayer->GetTickBase();
						float flServerTime = (tick * Interfaces.pGlobalVars->interval_per_tick);
						float LastLBYUpdateTime = flServerTime - GetOutgoingLatency();
						if (((LastLBYUpdateTime + 1.125 - GetOutgoingLatency()) <= flServerTime) && (Hacks.LocalPlayer->GetFlags() & FL_ONGROUND))
						{
							LastLBYUpdateTime += 1.125;
							Global::lbyupdate = true;
						}
						Global::lbyupdate = false;

						Vector* eYaw = pEntity->GetEyeAnglesPointer();
						float flResolveyaw;

						float lastLBY[64];
						float lbyDif[64];

						float curLbyYaw = pEntity->pelvisangs();

						if (pEntity->GetVecVelocity().Length2D() > 0)
							eYaw->y = pEntity->pelvisangs();
						else
						{
							if (!IsNearEqual(lastLBY[pEntity->GetIndex()], curLbyYaw, 15.f))
							{
								lbyDif[pEntity->GetIndex()] = curLbyYaw - eYaw->y;
								lastLBY[pEntity->GetIndex()] = curLbyYaw;
								eYaw->y += pEntity->pelvisangs();
							}
							else
							{
								eYaw->y += lbyDif[pEntity->GetIndex()];
							}
						}

						PlayerList::CPlayer* Player = plist.FindPlayer(pEntity);

						Player->resolved = eYaw->y;
						AutoResolver(angs, Player);

					}

					else if (Options.Ragebot.HvH.iAntiAimResolver == 2)
					{
						AutoResolver(angs, Player);
					}

					else if (Options.Ragebot.HvH.iAntiAimResolver == 3)
					{
						if (pEntity->pelvisangs() != OldYaw)
						{
							CurYaw = OldYaw;
							OldYaw = pEntity->pelvisangs();
						}
						AutoResolver(angs, Player);
					}

					else if (Options.Ragebot.HvH.iAntiAimResolver == 4)
					{
						TimedYaw = pEntity->GetEyeAngles().y - pEntity->pelvisangs();
						CurYaw = abs(TimedYaw);

						AutoResolver(angs, Player);
					}

					else if (Options.Ragebot.HvH.iAntiAimResolver == 5)
					{
						static float lowerDelta[64];
						static float lastYaw[64];
						curlwer = pEntity->pelvisangs();
						curlwerr = pEntity->GetEyeAngles().y;
						//curlwerr = pEnt->GetEyeAngles().y;
						float curLower = OldYaw;
						float curYaw = curlwerr;

						lowerDelta[i] = curYaw - curLower;
						//U::Normalize(lowerDelta[i]); //wat? normalize float

						if (fabs(lowerDelta[i]) > 15.f)
						{
							if (pEntity->GetVecVelocity().Length2D() > 0)
							{
								pEntity->GetEyeAngles().y - pEntity->pelvisangs();
							}
							else
							{
								if (curYaw != lastYaw[i])
								{
									curlwerr += lowerDelta[i];
									lastYaw[i] = curYaw;
								}
							}

						}
						AutoResolver(angs, Player);
					}

					else if (Options.Ragebot.HvH.iAntiAimResolver == 6)
					{

					}

					//*(float*)((DWORD)pEntity + Netvar("DT_CSPlayer", "m_angEyeAngles[]") + 0x4) = CurYaw;
				}
			}
		}
	}*

	/*if (Options.Ragebot.HvH.bBruteforce)
	{
		for (int i = 1; i < Interfaces.pEngine->GetMaxClients(); ++i)
		{
			CBaseEntity* entity = Interfaces.pEntList->GetClientEntity(i);
			if (entity->isValidPlayer())
			{
				if (GetAsyncKeyState(MOUSE_5))
				{
					if (!entity->GetVecVelocity().Length() > 1.0f)
					{
						int angle = rand() % 5;
						switch (angle)
						{
						case 0:entity->pelvisangs() += 135; break;
						case 2:entity->pelvisangs() += 155; break;
						case 3:entity->pelvisangs() += 165; break;
						case 4:entity->pelvisangs() += 175; break;
						case 5:entity->pelvisangs() += 185; break;
						}
					}
					else
					{
						entity->pelvisangs() += 180;
					}
				}
			}
		}
	}*/

}

void InitKeyValues(KeyValues* keyValues, char* name)
{
	DWORD dwFunction = (DWORD)Utils.PatternSearch("client.dll", (PBYTE)"\x68\x00\x00\x00\x00\x8B\xC8\xE8\x00\x00\x00\x00\x89\x45\xFC\xEB\x07\xC7\x45\x00\x00\x00\x00\x00\x8B\x03\x56", "x????xxx????xxxxxxx?????xxx", NULL, NULL);
	dwFunction += 7;
	dwFunction = dwFunction + *reinterpret_cast< PDWORD_PTR >(dwFunction + 1) + 5;
	__asm
	{
		push name
		mov ecx, keyValues
		call dwFunction
	}
}

void LoadFromBuffer(KeyValues* keyValues, char const* resourceName, const char* pBuffer)
{
	DWORD dwFunction = (DWORD)Utils.PatternSearch("client.dll", (PBYTE)"\xE8\x00\x00\x00\x00\x80\x7D\xF8\x00\x00\x00\x85\xDB", "x????xxxx??xx", NULL, NULL);
	dwFunction = dwFunction + *reinterpret_cast< PDWORD_PTR >(dwFunction + 1) + 5;
	__asm
	{
		push 0
		push 0
		push 0
		push pBuffer
		push resourceName
		mov ecx, keyValues
		call dwFunction
	}
}

IMaterial* Create_Material(bool Ignore, bool Lit, bool Wireframe)
{
	static int created = 0;
	static const char tmp[] =
	{
		"\"%s\"\
		\n{\
		\n\t\"$basetexture\" \"vgui/white_additive\"\
		\n\t\"$envmap\" \"\"\
		\n\t\"$ignorez\" \"%i\"\
		\n\t\"$model\" \"1\"\
		\n\t\"$flat\" \"1\"\
		\n\t\"$nocull\"  \"0\"\
		\n\t\"$selfillum\" \"1\"\
		\n\t\"$halflambert\" \"1\"\
		\n\t\"$nofog\"  \"0\"\
		\n\t\"$znearer\" \"0\"\
		\n\t\"$wireframe\" \"%i\"\
        \n}\n"
	};
	char* baseType = (Lit == true ? "VertexLitGeneric" : "UnlitGeneric");
	char material[512];
	char name[512];
	sprintf_s(material, sizeof(material), tmp, baseType, (Ignore) ? 1 : 0, (Wireframe) ? 1 : 0);
	sprintf_s(name, sizeof(name), "#Aimpulse_Chams_%i.vmt", created);
	++created;
	KeyValues* keyValues = static_cast< KeyValues* >(malloc(sizeof(KeyValues)));
	InitKeyValues(keyValues, baseType);
	LoadFromBuffer(keyValues, name, material);
	IMaterial* createdMaterial = Interfaces.pMaterialSystem->CreateMaterial(name, keyValues);
	createdMaterial->IncrementReferenceCount();
	return createdMaterial;
}

void __fastcall Hooked_DrawModelExecute(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	Chams::Get().OnDrawModelExecute(ctx, state, pInfo, pCustomBoneToWorld);
	//if (!Do_Chams(thisptr, edx, ctx, state, pInfo, pCustomBoneToWorld))
	Hacks.oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
	Interfaces.g_pModelRender->ForcedMaterialOverride(nullptr);
}

void __stdcall Hooked_ClientCmdUnrestricted(const char* szCmdString, char flag)
{
	string str(szCmdString);
	std::string prefix;
	if (!str.compare(0, string("Clantag ").size(), "Clantag "))
	{
		str.replace(0, string("Clantag ").size(), "");
		prefix = string("\\n");
		std::size_t found = str.find(prefix);
		if (found != std::string::npos)
			str.replace(found, found + prefix.size(), "\n");
		Misc::SetClanTag(str.c_str(), "");
	}
	else if (!str.compare(0, string("ReleaseName").size(), "ReleaseName"))
	{
		Misc::setName("\n\xAD\xAD\xAD­­­");
	}
	else if (!str.compare(0, string("Name ").size(), "Name "))
	{
		str.replace(0, string("Name ").size(), "");
		prefix = string("\\n");
		std::size_t found = str.find(prefix);
		if (found != std::string::npos)
			str.replace(found, found + prefix.size(), "\n");
		Misc::setName(str.c_str());
	}
	/*
	else if (!str.compare(0, string("Save ").size(), "Save ")) {
	str.replace(0, string("Save ").size(), "");
	std::ofstream output(str);
	Menu::Menu.Save(output);
	}
	else if (!str.compare(0, string("Load ").size(), "Load ")) {
	str.replace(0, string("Load ").size(), "");
	std::ifstream input(str);
	if (input.good()) Menu::Menu.Load(input);
	}
	else if (!str.compare(0, string("Delete ").size(), "Delete ")) {
	str.replace(0, string("Delete ").size(), "");
	std::ofstream output(str);
	if (output.good()) {
	output.close();
	std::remove(str.c_str());
	}
	}
	*/
	else
		Hacks.oClientCmdUnresticted(szCmdString, flag);
}


