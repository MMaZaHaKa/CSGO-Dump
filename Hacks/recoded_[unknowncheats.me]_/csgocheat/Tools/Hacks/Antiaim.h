#pragma once
#include "../../stdafx.h"
#include "../Menu/Menu.h"
#include "Misc.h"
#include "Aimbot.h"
#include "../Utils/LocalInfo.h"
#include "../Menu/SettingsManager.h"
void ClampAngles(QAngle& angles)
{
	if (angles.x > 89.f)
		angles.x = 89.f;
	if (angles.x < -89.f)
		angles.x = -89.f;
	if (angles.y > 180.f)
		angles.y = 180.f;
	if (angles.y < -180.f)
		angles.y = -180.f;

	angles.z = 0.f;
}
void AngleVectors(const Vector angles, Vector& forward, Vector& right, Vector& up)
{
	float angle;
	static float sp, sy, cp, cy;

	angle = angles[0] * (M_PI / 180.f);
	sp = sin(angle);
	cp = cos(angle);

	angle = angles[1] * (M_PI / 180.f);
	sy = sin(angle);
	cy = cos(angle);


	forward[0] = cp * cy;
	forward[1] = cp * sy;
	forward[2] = -sp;


	static float sr, cr;

	angle = angles[2] * (M_PI / 180.f);
	sr = sin(angle);
	cr = cos(angle);


	right[0] = -1 * sr * sp * cy + -1 * cr * -sy;
	right[1] = -1 * sr * sp * sy + -1 * cr *cy;
	right[2] = -1 * sr * cp;

	up[0] = cr * sp *cy + -sr * -sy;
	up[1] = cr * sp *sy + -sr * cy;
	up[2] = cr * cp;


}
float GetBestHeadAngle(float yaw)
{
	float Back, Right, Left;

	Vector src3D, dst3D, forward, right, up, src, dst;
	trace_t tr;
	Ray_t ray, ray2, ray3, ray4, ray5;
	CTraceFilter filter;

	QAngle engineViewAngles;
	Interfaces.pEngine->GetViewAngles(engineViewAngles);

	engineViewAngles.x = 0;

	AngleVectors(engineViewAngles, forward, right, up);

	filter.pSkip = Hacks.LocalPlayer;
	src3D = Hacks.LocalPlayer->GetEyePosition();
	dst3D = src3D + (forward * 384);

	ray.Init(src3D, dst3D);

	Interfaces.pTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

	Back = (tr.endpos - tr.startpos).Length();

	ray2.Init(src3D + right * 35, dst3D + right * 35);

	Interfaces.pTrace->TraceRay(ray2, MASK_SHOT, &filter, &tr);

	Right = (tr.endpos - tr.startpos).Length();

	ray3.Init(src3D - right * 35, dst3D - right * 35);

	Interfaces.pTrace->TraceRay(ray3, MASK_SHOT, &filter, &tr);

	Left = (tr.endpos - tr.startpos).Length();

	if (Left > Right)
	{
		return (yaw - 90);
	}
	else if (Right > Left)
	{
		return (yaw + 90);
	}
	else if (Back > Right || Back > Left)
	{
		return (yaw - 180);
	}
	return 0;
}
float RandomFloat2(float min, float max)
{
	typedef float(*RandomFloat_t)(float, float);
	return ((RandomFloat_t)GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat")) (min, max);
}
int Random(int min, int max)
{
	if (min == max)
		return min;

	if (max % 2 == 0)
		return min + (rand() % (max - min));

	return min + (rand() % (max - min + 1));
}
static float RandomFloat(float min, float max)
{
	typedef float(*RandomFloat_t)(float, float);
	static RandomFloat_t m_RandomFloat = (RandomFloat_t)GetProcAddress(GetModuleHandle("vstdlib.dll"),("RandomFloat"));
	return m_RandomFloat(min, max);
}

float NormalizeYaw(float value)
{
	while (value > 180)
		value -= 180.f;

	while (value < -180)
		value += 180.f;
	return value;
}
float get3ddist2(Vector myCoords, Vector enemyCoords)
{
	return sqrt(
		pow(double(enemyCoords.x - myCoords.x), 2.0) +
		pow(double(enemyCoords.y - myCoords.y), 2.0) +
		pow(double(enemyCoords.z - myCoords.z), 2.0));
}
QAngle CalculateAngle(Vector vecOrigin, Vector vecOther)
{
	auto ret = Vector();
	Vector delta = vecOrigin - vecOther;
	double hyp = delta.Length2D();
	ret.y = (atan(delta.y / delta.x) * 57.295779513082f);
	ret.x = (atan(delta.z / hyp) * 57.295779513082f);
	ret[2] = 0.00;

	if (delta.x >= 0.0)
		ret.y += 180.0f;
	return ret;
}

#define KEY_DOWN(VK_NNM) ((GetAsyncKeyState(VK_NNM) & 0x8000) ? 1:0)

#define M_PI		3.14159265358979323846	// matches value in gcc v2 math.h

#define M_PI_F		((float)(M_PI))	// Shouldn't collide with anything.

#define M_PHI		1.61803398874989484820 // golden ratio

float bflip = 0;
bool sendpacket = Hacks.SendPacket;
float OldLBY;
float LBYBreakerTimer;
float LastLBYUpdateTime;
bool bSwitch;
typedef void(__cdecl* MsgFn)(const char* msg, va_list);

void Msg2(const char* msg, ...)
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
float GetOutgoingLatency2()
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
float CurrentVelocity()
{
	int vel = Hacks.LocalPlayer->GetVecVelocity().Length2D();
	return vel;
}
bool NextLBYUpdate()
{

	float flServerTime = (float)(Hacks.LocalPlayer->GetTickBase()  * Interfaces.pGlobalVars->interval_per_tick);

	if (OldLBY != Hacks.LocalPlayer->pelvisangs())
	{
		LBYBreakerTimer++;
		OldLBY = Hacks.LocalPlayer->pelvisangs();
		bSwitch = !bSwitch;
		LastLBYUpdateTime = flServerTime;
	}

	if (Hacks.LocalPlayer->GetFlags() & FL_ONGROUND && Hacks.LocalPlayer->GetVecVelocity().Length2D() > 1.1)
	{
		LastLBYUpdateTime = flServerTime;
		return false;
	}


	if ((LastLBYUpdateTime + 1.125 - GetOutgoingLatency2() < flServerTime) && (Hacks.LocalPlayer->GetFlags() & FL_ONGROUND))
	{
		if (LastLBYUpdateTime + 1.125 - GetOutgoingLatency2() < flServerTime)
		{
			LastLBYUpdateTime = flServerTime;
		}
		if (Options.Visuals.Infobox.InfoECHO)
		{
			Interfaces.pEngine->ClientCmd_Unrestricted("developer 1 ", 0);
			Sleep(0);

			Interfaces.pEngine->ClientCmd_Unrestricted("con_filter_enable 2 ", 0);
			Sleep(0);


			Interfaces.pEngine->ClientCmd_Unrestricted("con_filter_text (floxware) ", 0);
			Sleep(0);
			std::string before = ("(floxware) LBY update [new tick]");
			std::string newline = "\n";

			Msg2((before + newline).c_str());
		}

		return true;
	}
	return false;
}
bool ShouldPredict()
{
	INetChannelInfo* nci = Interfaces.pEngine->GetNetChannelInfo();

	float server_time = Interfaces.pGlobalVars->curtime + nci->GetLatency(FLOW_OUTGOING);
	float PredictedTime = 0.f;
	static bool initialized;
	bool will_update = false;

	if (!initialized && !Hacks.LocalPlayer->IsMoving())
	{
		initialized = true;
		PredictedTime = server_time + 0.22f;

		Interfaces.pEngine->ClientCmd_Unrestricted("developer 1 ", 0);
		Sleep(0);

		Interfaces.pEngine->ClientCmd_Unrestricted("con_filter_enable 2 ", 0);
		Sleep(0);


		Interfaces.pEngine->ClientCmd_Unrestricted("con_filter_text (floxware) ", 0);
		Sleep(0);
		std::string before = ("(floxware) LBY update [new tick]");
		std::string newline = "\n";

		Msg2((before + newline).c_str());
	}
	else if (Hacks.LocalPlayer->IsMoving())
	{
		initialized = false;
	}

	if (server_time >= (PredictedTime) && Hacks.LocalPlayer->GetFlags() & FL_ONGROUND)
	{
		PredictedTime = server_time + 1.1f;
		will_update = true;

		Interfaces.pEngine->ClientCmd_Unrestricted("developer 1 ", 0);
		Sleep(0);

		Interfaces.pEngine->ClientCmd_Unrestricted("con_filter_enable 2 ", 0);
		Sleep(0);


		Interfaces.pEngine->ClientCmd_Unrestricted("con_filter_text (floxware) ", 0);
		Sleep(0);
		std::string before = ("(floxware) LBY update [new tick]");
		std::string newline = "\n";

		Msg2((before + newline).c_str());
	}
	return will_update;
}

void BreakLowerbodyFreestand()
{
	QAngle Angles;
	Interfaces.pEngine->GetViewAngles(Angles);
	float BestHeadPosition = GetBestHeadAngle(Angles.y);

	int LowerbodyDelta = Options.Ragebot.AntiAim.Stand.LBYDELTA;


		if (NextLBYUpdate())
			Hacks.CurrentCmd->viewangles.y = BestHeadPosition + LowerbodyDelta;
		else
			Hacks.CurrentCmd->viewangles.y = BestHeadPosition;

		Hacks.CurrentCmd->viewangles.x = 89.0f;
}
class AntiAim
{
public:
	QAngle temp;
	bool start = false;
	bool jitter = false;
	bool flip = false;
	bool ShouldAA = true;
	int current = 0;
private:

	void MemeWalk()
	{
		if (Hacks.LocalPlayer->GetMoveType() == MOVETYPE_LADDER || Hacks.LocalPlayer->GetMoveType() == MOVETYPE_NOCLIP)
			return;

		if (Hacks.CurrentCmd->forwardmove > 0)
		{
			Hacks.CurrentCmd->buttons |= IN_BACK;
			Hacks.CurrentCmd->buttons &= ~IN_FORWARD;
		}

		if (Hacks.CurrentCmd->forwardmove < 0)
		{
			Hacks.CurrentCmd->buttons |= IN_FORWARD;
			Hacks.CurrentCmd->buttons &= ~IN_BACK;
		}

		if (Hacks.CurrentCmd->sidemove < 0)
		{
			Hacks.CurrentCmd->buttons |= IN_MOVERIGHT;
			Hacks.CurrentCmd->buttons &= ~IN_MOVELEFT;
		}

		if (Hacks.CurrentCmd->sidemove > 0)
		{
			Hacks.CurrentCmd->buttons |= IN_MOVELEFT;
			Hacks.CurrentCmd->buttons &= ~IN_MOVERIGHT;
		}
	}
	void TraceWorld(const QAngle& rotation,  Vector& start, float range, trace_t& trace)
	{
		Ray_t ray;
		CTraceWorldOnly filter;
		Vector forward;

		Misc::AngleVectors2(rotation, forward);
		forward.NormalizeInPlace();

		//filter.pLocal = G::pLocal;
		ray.Init(start, (start + forward * range));

		Interfaces.pTrace->TraceRay(ray, 0x46004003, &filter, &trace);
	}

	void AimAtPlayers(CInput::CUserCmd* cmd, bool& bSendPacket, Vector& view)
	{
		int iPitch = Options.Ragebot.AntiAim.Edge.iPitch;
		int iYaw = Options.Ragebot.AntiAim.Edge.iYaw;
		int iFakeYaw = Options.Ragebot.AntiAim.Edge.iFakeYaw;
		int random;
		float setyaw = 0;
		float setpitch = 0;
		int yaw = Options.Ragebot.AntiAim.Stand.iYaw;
		QAngle val;
		Vector eye_position = Hacks.LocalPlayer->GetEyePosition();
		int value = Hacks.CurrentCmd->viewangles.y, num = 0;
		float best_dist = Hacks.LocalWeapon->GetCSWpnData()->m_fRange;
		for (int i = 1; i < Interfaces.pGlobalVars->maxClients; ++i)
		{
			if (i == Interfaces.pEngine->GetLocalPlayer())
				continue;

			CBaseEntity* target = Interfaces.pEntList->GetClientEntity(i);
			player_info_t info;
			//Calls from left->right so we wont get an access violation error
			Vector pos;
			if (!target || /*target->IsDormant() ||*//*bc otherwise we get 1tapped by enemies throug dormant walls*/ target->GetHealth() < 1)
				continue;
			if (Options.Ragebot.AntiAim.Stand.FreeStanding && Hacks.LocalPlayer->GetTeam() != target->GetTeam())
			{
				pos = target->GetEyePosition();
				float dist = get3ddist2(pos, Hacks.LocalPlayer->GetEyePosition());
				if (dist >= best_dist)
					continue;
				best_dist = dist;

				QAngle angle = CalculateAngle(Hacks.LocalPlayer->GetEyePosition(), pos);
				angle.y = Misc::Normalize_y(angle.y);
				ClampAngles(angle);

				num++;
					if (NextLBYUpdate())
						value -= angle.y - 180;
					else
						value += angle.y - 93;

				
			}
		}
		cmd->viewangles.y = yaw + value;


		if(num++)
		cmd->viewangles.x = 89.0;

	}
	void FakeWalk()
	{
		if (KEY_DOWN(Options.Ragebot.AntiAim.Misc.iFakeWalkKey) && Options.Ragebot.AntiAim.Misc.bFakeWalk)
		{
			static int iChoked = -1;
			iChoked++;

			if (iChoked < 3)
			{
				Hacks.SendPacket = false;
				Hacks.CurrentCmd->tick_count += 10;
				Hacks.CurrentCmd += 7 + Hacks.CurrentCmd->tick_count % 2 ? 0 : 1;
				Hacks.CurrentCmd->buttons |= Hacks.LocalPlayer->GetMoveType() == IN_BACK;
				Hacks.CurrentCmd->forwardmove = Hacks.CurrentCmd->sidemove = 0.f;
			}
			else
			{
				Hacks.SendPacket = true;
				iChoked = -1;
				Interfaces.pGlobalVars->frametime *= (Hacks.LocalPlayer->GetVecVelocity().Length2D()) / 1.f;
				Hacks.CurrentCmd->buttons |= Hacks.LocalPlayer->GetMoveType() == IN_FORWARD;
			}
		}
	}



	void Fakelag()
	{
		static int Ticks = 6;
		static int Ticks1 = 0;
		int ticksMax = 16;
		static Vector pos = Hacks.LocalPlayer->GetVecOrigin();
		int Factor = Options.Misc.Misc1.iFakeLagFactor;
		bool InAirOnly = Options.Misc.Misc1.bInAirOnly;
		int Mode = Options.Misc.Misc1.iFakeLag;
		switch (Mode)
		{
		case 0: // Off
			break;
		case 1: // Normal
			if (InAirOnly)
			{
				if (Hacks.LocalPlayer->GetFlags() & FL_ONGROUND)
					return;

				if (Factor > 0)
				{
					if (Ticks1 >= ticksMax)
					{
						Hacks.SendPacket = true;
						Ticks1 = 0;
					}
					else
					{
						int packetsToChoke;
						if (Hacks.LocalPlayer->GetVecVelocity().Length() > 0.f)
						{
							packetsToChoke = (int)((128.f / Interfaces.pGlobalVars->interval_per_tick) / Hacks.LocalPlayer->GetVecVelocity().Length()) + 1;
							if (packetsToChoke >= 15)
								packetsToChoke = 14;
							if (packetsToChoke < Factor)
								packetsToChoke = Factor;
						}
						else
							packetsToChoke = 0;

						Hacks.SendPacket = Ticks1 < 18 - packetsToChoke;;
					}
					Ticks1++;
				}
			}
			else
			{
				if (Factor > 0)
				{
					if (Ticks1 >= ticksMax)
					{
						Hacks.SendPacket = true;
						Ticks1 = 0;
					}
					else
					{
						int packetsToChoke;
						if (Hacks.LocalPlayer->GetVecVelocity().Length() > 0.f)
						{
							packetsToChoke = (int)((128.f / Interfaces.pGlobalVars->interval_per_tick) / Hacks.LocalPlayer->GetVecVelocity().Length()) + 1;
							if (packetsToChoke >= 15)
								packetsToChoke = 14;
							if (packetsToChoke < Factor)
								packetsToChoke = Factor;
						}
						else
							packetsToChoke = 0;

						Hacks.SendPacket = Ticks1 < 18 - packetsToChoke;;
					}
					Ticks1++;
				}
			}
			break;
		case 2: // Adaptive
			if (InAirOnly)
			{
				if (Hacks.LocalPlayer->GetFlags() & FL_ONGROUND)
					return;

				if (!(LocalInfo.Choked > 12))
					Hacks.SendPacket = false;

			}
			else
			{
				if (!(LocalInfo.Choked > 12))
					Hacks.SendPacket = false;
			}
			break;
		case 3: // Switch
			if (InAirOnly)
			{
				if (Hacks.LocalPlayer->GetFlags() & FL_ONGROUND)
					return;

				if (!((pos - LocalInfo.LastPos).Length2DSqr() > 4096.f) && !(LocalInfo.Choked > 12))
					Hacks.SendPacket = false;
			}
			else
			{
				if (!((pos - LocalInfo.LastPos).Length2DSqr() > 4096.f) && !(LocalInfo.Choked > 12))
					Hacks.SendPacket = false;
			}
			break;
		case 4: // Random
			if (InAirOnly)
			{
				if (Hacks.LocalPlayer->GetFlags() & FL_ONGROUND)
					return;

				if (LocalInfo.Choked > Ticks)
				{
					Ticks += ((rand() % 4) + 2) * ((rand() % 2) + 2);
					while (Ticks > 12)
						Ticks -= 7;
				}
				else
					Hacks.SendPacket = false;

			}
			else
			{
				if (LocalInfo.Choked > Ticks)
				{
					Ticks += ((rand() % 4) + 2) * ((rand() % 2) + 2);
					while (Ticks > 12)
						Ticks -= 7;
				}
				else
					Hacks.SendPacket = false;
			}
			break;
		case 5: // Fluctuate
			if (Hacks.LocalPlayer->GetFlags() & FL_ONGROUND)
			{
				Hacks.SendPacket = true;
			}
			else
			{
				Hacks.SendPacket = false;
			}
			break;
		}
		pos = Hacks.LocalPlayer->GetVecOrigin();
	}

	bool CanFireNext(CBaseEntity* pEntity)
	{
		CBaseCombatWeapon* pWeapon = pEntity->GetActiveBaseCombatWeapon();
		float flServerTime = pEntity->GetTickBase() * Interfaces.pGlobalVars->interval_per_tick;
		float flNextShot = pWeapon->NextPrimaryAttack() - flServerTime;
		return flNextShot == 1;
	}
	void AtTarget(Vector& viewangles)
	{

		//if (Aimbot.Target != Vector(0, 0, 0))
		//{
		//	Misc::CalcAngle(Hacks.LocalPlayer->GetVecOrigin(), Aimbot.Target + Hacks.LocalPlayer->GetVecOrigin(), viewangles);
		//}

		QAngle angle = QAngle(0, 0, 0);
		float lowest = 99999999.f;
		Vector EyePos = Hacks.LocalPlayer->GetEyePosition() + Hacks.LocalPlayer->GetAbsOrigin();

		for (int i = 1; i < 65; i++) {
			if (i == Interfaces.pEngine->GetLocalPlayer()) continue;
			CBaseEntity* pEnt = Interfaces.pEntList->GetClientEntity(i);
			if (!pEnt) continue;
			if (pEnt->IsDormant()) continue;
			if (!pEnt->isAlive()) continue;
			if (pEnt->GetTeam() == Hacks.LocalPlayer->GetTeam()) continue;

			Vector CurPos = pEnt->GetEyePosition() + pEnt->GetAbsOrigin();

			if (CurPos.DistToSqr(EyePos) < lowest) {
				lowest = CurPos.DistTo(EyePos);
				Misc::CalcAngle(EyePos, CurPos, angle);
			}
		}
	}
	float doSqchAA(bool& move, bool& move2, QAngle newAngle) {
		float flUpdateTime = move2 ? 0.22000001f : 1.1f; //+ interval cause fakelag
		static auto start = std::chrono::high_resolution_clock::now();
		int yaw = 0;

		if (move) {
			start = std::chrono::high_resolution_clock::now();
			move = false;
		}

		auto end = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> delta = end - start;

		//Find bestside
		QAngle tmpAngle = newAngle;

		Misc::FindBestSide(Hacks.LocalPlayer, 512.f, Hacks.CurrentCmd, tmpAngle);


		if (delta.count() >= flUpdateTime) {
			move2 = false;
			start = std::chrono::high_resolution_clock::now();



			return Hacks.CurrentCmd->viewangles.y + 180;//180
		}

		if (move2)
			return Hacks.CurrentCmd->viewangles.y + Options.Ragebot.AntiAim.Stand.iFakeYaw;


		return Hacks.CurrentCmd->viewangles.y - 70;//tmpAngle.y
	}

	void DoAA(int style, Vector& view)
	{
		long currentTime_ms = std::chrono::duration_cast< std::chrono::seconds >(std::chrono::system_clock::now().time_since_epoch()).count();
		static long timeStamp = currentTime_ms;

		timeStamp = currentTime_ms;

		int yaw = 0;
		int pitch = 0;
		int maxJitter;
		int random;
		float temp;
		double factor;
		static int SpinYaw = 0;
		static float StoredAng = 0;
		static bool fake;
		fake = !fake;

		//static int jitter = 0;
		static int last = 0;
		int help = {};
		int Fake = rand() % 3;
		static int Spin[2] = { 0, 0 };
		for (int& s : Spin)
			if (s > 180)
				s -= 360;
			else if (s < -180)
				s += 360;

		static bool bFlip = false;
		static bool bSwitch = false;
		static int state = 0;

		float flCurTime = Interfaces.pGlobalVars->curtime;
		static float flTimeUpdate = 1.09f;
		static float flNextTimeUpdate = flCurTime + flTimeUpdate;
		if (flCurTime >= flNextTimeUpdate) {
			bFlip = !bFlip;
			state = 0;
		}

		if (flNextTimeUpdate < flCurTime || flNextTimeUpdate - flCurTime > 10.f)
			flNextTimeUpdate = flCurTime + flTimeUpdate;

		Hacks.SendPacket = false;

		// Stand Anti-Anon huehuehuehue
		if (style == 0)
		{

			int iFakeYawAdd = Options.Ragebot.AntiAim.Stand.iFakeYawAdd;
			int iYawAdd = Options.Ragebot.AntiAim.Stand.iYawAdd;

			if (fake)
			{
				if (Options.Ragebot.AntiAim.Stand.FreeStanding)
				{
					QAngle val;
					Vector eye_position = Hacks.LocalPlayer->GetEyePosition();
					int value = Hacks.CurrentCmd->viewangles.y, num = 0;
					float best_dist = Hacks.LocalWeapon->GetCSWpnData()->m_fRange;
					for (int i = 1; i < Interfaces.pGlobalVars->maxClients; ++i)
					{
						if (i == Interfaces.pEngine->GetLocalPlayer())
							continue;

						CBaseEntity* target = Interfaces.pEntList->GetClientEntity(i);
						player_info_t info;
						//Calls from left->right so we wont get an access violation error
						Vector pos;
						if (!target || /*target->IsDormant() ||*//*bc otherwise we get 1tapped by enemies throug dormant walls*/ target->GetHealth() < 1)
							continue;
						if (Options.Ragebot.AntiAim.Stand.FreeStanding && Hacks.LocalPlayer->GetTeam() != target->GetTeam())
						{
							pos = target->GetEyePosition();
							float dist = get3ddist2(pos, Hacks.LocalPlayer->GetEyePosition());
							if (dist >= best_dist)
								continue;
							best_dist = dist;

							QAngle angle = CalculateAngle(Hacks.LocalPlayer->GetEyePosition(), pos);
							angle.y = Misc::Normalize_y(angle.y);
							ClampAngles(angle);

							num++;
								value -= yaw - 137;



						}
					}
					yaw = value;
				}
				Hacks.SendPacket = true;
				//Forward jitter2
				static int fJitter1 = 1;
				static float fJitterAngles[] = { 10.f,-10.f,-30.f,30.f,0.f };
				bool stop = false;
				static bool flip = false;

				switch (Options.Ragebot.AntiAim.Stand.iFakeYaw) {
				case 1:
					yaw += 180.0f;
					//Jitter LBY Basic
					break;
				case 2:
					yaw = Hacks.LocalPlayer->pelvisangs() + (Hacks.SendPacket ? 180.0f : 0); //Jitter LBY Reversed Basic
					break;
				case 3:
					yaw = Hacks.LocalPlayer->pelvisangs() + (Hacks.SendPacket ? +(90 + 35.0f) : 0); //Back Jitter LBY Basic
					break;
				case 4:
					yaw = Hacks.LocalPlayer->pelvisangs() + 180.0f + (float)Random(-35, 35); //Spin LBY Basic
					break;
				case 5:
					yaw = 180.f; //Backward Basic
					break;
				case 6:
					yaw = Hacks.LocalPlayer->pelvisangs(); //Side Unhittable
					break;
				case 7:
					yaw = fJitterAngles[Hacks.CurrentCmd->tick_count % fJitter1];
					fJitter1++;

					if (fJitter1 > ARRAYSIZE(fJitterAngles))
						fJitter1 = 1;
					break;
				}

					flip = !flip;
			}
			else
			{
				Hacks.SendPacket = false;

				switch (Options.Ragebot.AntiAim.Stand.iYaw)
				{
				case 0:
					break;
				case 1: // Backwards
					yaw = 180.f - Options.Ragebot.AntiAim.Stand.iYawAdd;
					break;
				case 2: // Backwards Jitter
					yaw -= 180 - Options.Ragebot.AntiAim.Stand.iYawAdd;
					random = rand() % 100;
					maxJitter = rand() % (85 - 70 + 1) + 70;
					temp = maxJitter - (rand() % maxJitter);
					if (random < 35 + (rand() % 15))
						yaw -= temp;
					else if (random < 85 + (rand() % 15))
						yaw += temp;
					break;
				case 3: // Spin
					if (flip)
						SpinYaw -= 35;
					else
						SpinYaw += 35;
					flip != flip;
					StoredAng = SpinYaw;
					yaw = SpinYaw;
					break;
				case 4: // Sideways
				{// Sideways
					static bool f_flip = true;
					f_flip = !f_flip;
					if (GetKeyState(0x12)) // a
					{
						if (f_flip)
						{
							Hacks.SendPacket = false;
							if (NextLBYUpdate())
							{
								yaw -= 155;
							}
							else
							{

								yaw -= 90;
							}
						}
						else if (!f_flip)
						{
							Hacks.SendPacket = true;
							if (NextLBYUpdate())
							{
								yaw += 90;
							}
							else
							{
								yaw -= 155;
							}
						}
					}
					else
					{
						if (f_flip)
						{
							Hacks.SendPacket = false;
							if (NextLBYUpdate())
							{
								yaw += 155;
							}
							else
							{

								yaw += 90;
							}
						}
						else if (!f_flip)
						{
							Hacks.SendPacket = true;
							if (NextLBYUpdate())
							{
								yaw -= 90;
							}
							else
							{
								yaw += 155;
							}
						}
					}
				}
				break;
				case 5: // Random
					static bool swap;
					if (GetAsyncKeyState(GetKeyState(0x12)))
						swap = true;
					else
						swap = false;

					if (GetKeyState(0x12))
					{
						if (Hacks.SendPacket)
							yaw -= 105;
						else
						{
							if (NextLBYUpdate())
								yaw -= 133.8f;
							else
								yaw += 93;
						}
					}
					else
					{
						if (Hacks.SendPacket)
							yaw += 105;
						else
						{
							if (NextLBYUpdate())
								yaw += 133.8f;
							else
								yaw -= 93;
						}
					}
					break;
				case 6:
				{// 180Z
					float randfloat = rand() % -180 + 360;
					static int ChokedPackets = -1;
					ChokedPackets++;

					if (NextLBYUpdate())
					{
						yaw -= randfloat;
					}
					else
					{
						yaw += 90;
					}
					if (ChokedPackets < 1)
					{
						Hacks.SendPacket = false;
						yaw -= randfloat;
					}
					else
					{
						Hacks.SendPacket = true;
						yaw += 90;
						ChokedPackets = -1;
					}
				}
				break;
				case 7:

					if (GetKeyState(0x27)) // a
					{
						yaw = 90.f + state * 34.f;
						if (state > 1)
							yaw = 90.f + 34.8f;
					}
					if (GetKeyState(0x25)) //d 		
					{
						yaw = -90.f - state * 34.f;
						if (state > 1)
							yaw = -90.f - 34.8f;
					}
					break;
				case 8: // Static
					yaw = Options.Ragebot.AntiAim.Stand.iYawAdd;
					break;
				case 9: // Backwards Corruption
					switch (timeStamp % 8)
					{
					case 1: yaw = 170 + rand() % ((90 - 1) + 1) + 1;  break;
					case 2: yaw = 180;  break;
					case 3: yaw = 170 + rand() % ((180 - 90) + 1) + 1;  break;
					case 4: yaw = 180;  break;
					case 5: yaw = 170 + rand() % (((-90) - (-180)) + 1) + 1;  break;
					case 6: yaw = 180;  break;
					case 7: yaw = 170 + rand() % (((-1) - (-90)) + 1) + 1;  break;
					case 8: yaw = 180;  break;
					}
					break;
				}
			}

			switch (Options.Ragebot.AntiAim.Stand.iPitch)
			{
				case 1:
					pitch = 89.f; //Down
					break;
				case 2:
					pitch = 70.f; //Down test
					break;
				case 3:
					pitch = 0.f; //Mid
					break;
				case 4:
					pitch = -89.f; //Up
					break;
			}


			if (Options.Ragebot.AntiAim.Stand.bAtTargets)
				AtTarget(view);
		}

		// Move Anti-Anon huehuehuehue
		else if (style == 1)
		{
			int iFakeYawAdd = Options.Ragebot.AntiAim.Move.iFakeYawAdd;
			int iYawAdd = Options.Ragebot.AntiAim.Move.iYawAdd;
			if (fake)
			{
				Hacks.SendPacket = true;

				switch (Options.Ragebot.AntiAim.Stand.iFakeYaw)
				{
				case 0:
					break;
				case 1: // 180
					yaw = 180.f - Options.Ragebot.AntiAim.Stand.iFakeYawAdd;
					break;
				case 2: // Backwards Jitter
					yaw -= 180 - Options.Ragebot.AntiAim.Stand.iFakeYawAdd;
					random = rand() % 100;
					maxJitter = rand() % (85 - 70 + 1) + 70;
					temp = maxJitter - (rand() % maxJitter);
					if (random < 35 + (rand() % 15))
						yaw -= temp;
					else if (random < 85 + (rand() % 15))
						yaw += temp;
					break;
				case 3:
				{// Sideways
					static bool f_flip = true;
					f_flip = !f_flip;
					if (GetKeyState(0x12)) // a
					{
						if (f_flip)
						{
							Hacks.SendPacket = false;
							if (NextLBYUpdate())
							{
								yaw -= 155;
							}
							else
							{

								yaw -= 90;
							}
						}
						else if (!f_flip)
						{
							Hacks.SendPacket = true;
							if (NextLBYUpdate())
							{
								yaw += 90;
							}
							else
							{
								yaw -= 155;
							}
						}
					}
					else
					{
						if (f_flip)
						{
							Hacks.SendPacket = false;
							if (NextLBYUpdate())
							{
								yaw += 155;
							}
							else
							{

								yaw += 90;
							}
						}
						else if (!f_flip)
						{
							Hacks.SendPacket = true;
							if (NextLBYUpdate())
							{
								yaw -= 90;
							}
							else
							{
								yaw += 155;
							}
						}
					}
				}
				break;
				case 4:
					static bool swap;
					if (GetAsyncKeyState(GetKeyState(0x12)))
						swap = true;
					else
						swap = false;

					if (GetKeyState(0x12))
					{
						if (Hacks.SendPacket)
							yaw -= 105;
						else
						{
							if (NextLBYUpdate())
								yaw -= 133.8f;
							else
								yaw += 93;
						}
					}
					else
					{
						if (Hacks.SendPacket)
							yaw += 105;
						else
						{
							if (NextLBYUpdate())
								yaw += 133.8f;
							else
								yaw -= 93;
						}
					}
					break;
				case 5: // 180 Z
				{// 180Z
					float randfloat = rand() % -180 + 360;
					static int ChokedPackets = -1;
					ChokedPackets++;

					if (NextLBYUpdate())
					{
						yaw -= randfloat;
					}
					else
					{
						yaw += 90;
					}
					if (ChokedPackets < 1)
					{
						Hacks.SendPacket = false;
						yaw -= randfloat;
					}
					else
					{
						Hacks.SendPacket = true;
						yaw += 90;
						ChokedPackets = -1;
					}
				}				break;
				case 6:


					if (GetKeyState(0x27)) // a
					{
						yaw = 90.f + state * 34.f;
						if (state > 1)
							yaw = 90.f + 34.8f;
					}
					if (GetKeyState(0x25)) //d 		
					{
						yaw = -90.f - state * 34.f;
						if (state > 1)
							yaw = -90.f - 34.8f;
					}
					break;
				case 7: // Static
					yaw = Options.Ragebot.AntiAim.Stand.iFakeYawAdd;
					break;
				case 8: // Opposite
					if (Options.Ragebot.AntiAim.Stand.iYaw == 1)
					{
						yaw = 0 + Options.Ragebot.AntiAim.Stand.iYawAdd;
					}
					else if (Options.Ragebot.AntiAim.Stand.iYaw == 2)
					{
						yaw = 0 + iFakeYawAdd;
						random = rand() % 100;
						maxJitter = rand() % (85 - 70 + 1) + 70;
						temp = maxJitter - (rand() % maxJitter);
						if (random < 35 + (rand() % 15))
							yaw -= temp;
						else if (random < 85 + (rand() % 15))
							yaw += temp;
					}
					else if (Options.Ragebot.AntiAim.Stand.iYaw == 3)
					{
						if (flip)
							SpinYaw -= 90;
						else
							SpinYaw += 90;
						flip != flip;
						StoredAng = SpinYaw;
						yaw = SpinYaw;
					}
					else if (Options.Ragebot.AntiAim.Stand.iYaw == 4)
					{
						if (Options.Ragebot.AntiAim.Stand.iYawAdd >= 35 && Options.Ragebot.AntiAim.Stand.iYawAdd <= 95)
							yaw = -90;
						else if (Options.Ragebot.AntiAim.Stand.iYawAdd <= -35 && Options.Ragebot.AntiAim.Stand.iYawAdd >= -95)
							yaw = 90;
					}
					else if (Options.Ragebot.AntiAim.Stand.iYaw == 5)
					{
						yaw -= rand();
					}
					else if (Options.Ragebot.AntiAim.Stand.iYaw == 6)
					{
						yaw -= Options.Ragebot.AntiAim.Stand.iYawAdd;
					}
					else if (Options.Ragebot.AntiAim.Stand.iYaw == 7)
					{
						switch (timeStamp % 8)
						{
						case 1: yaw -= rand() % ((45 - 1) + 1) + 1;  break;
						case 2: yaw -= 10;  break;
						case 3: yaw -= rand() % ((135 - 45) + 1) + 1;  break;
						case 4: yaw -= 10;  break;
						case 5: yaw -= rand() % ((180 - 135) + 1) + 1;  break;
						case 6: yaw -= 10;  break;
						case 7: yaw -= rand() % ((270 - 270) + 1) + 1;  break;
						case 8: yaw -= 10;  break;
						}
					}
					break;

					if (Options.Ragebot.AntiAim.Move.lockside3)
					{

						int W, H, cW, cH;
						Interfaces.pEngine->GetScreenSize(W, H);


						cW = W / 2;
						cH = H / 2;


						if (GetKeyState(0x27)) // a
						{
							if(NextLBYUpdate)
							yaw = -90;
							else
								yaw = 90;

						}
						else if (GetKeyState(0x25)) //d 		
						{
							if (NextLBYUpdate)
								yaw = 90;
							else
							yaw = -90;
						}


					}
				}
			}

			else
			{
				Hacks.SendPacket = false;

				switch (Options.Ragebot.AntiAim.Move.iYaw)
				{
				case 0:
					break;
				case 1: // Backwards
					yaw = 180.f - Options.Ragebot.AntiAim.Move.iYawAdd;
					break;
				case 2: // Backwards Jitter
					yaw -= 180 - Options.Ragebot.AntiAim.Move.iYawAdd;
					random = rand() % 100;
					maxJitter = rand() % (85 - 70 + 1) + 70;
					temp = maxJitter - (rand() % maxJitter);
					if (random < 35 + (rand() % 15))
						yaw -= temp;
					else if (random < 85 + (rand() % 15))
						yaw += temp;
					break;
				case 3: // Spin
					if (flip)
						SpinYaw -= 35;
					else
						SpinYaw += 35;
					flip != flip;
					StoredAng = SpinYaw;
					yaw = SpinYaw;
					break;
				case 4: // Sideways
				{// Sideways
					static bool f_flip = true;
					f_flip = !f_flip;
					if (GetKeyState(0x12)) // a
					{
						if (f_flip)
						{
							Hacks.SendPacket = false;
							if (NextLBYUpdate())
							{
								yaw -= 155;
							}
							else
							{

								yaw -= 90;
							}
						}
						else if (!f_flip)
						{
							Hacks.SendPacket = true;
							if (NextLBYUpdate())
							{
								yaw += 90;
							}
							else
							{
								yaw -= 155;
							}
						}
					}
					else
					{
						if (f_flip)
						{
							Hacks.SendPacket = false;
							if (NextLBYUpdate())
							{
								yaw += 155;
							}
							else
							{

								yaw += 90;
							}
						}
						else if (!f_flip)
						{
							Hacks.SendPacket = true;
							if (NextLBYUpdate())
							{
								yaw -= 90;
							}
							else
							{
								yaw += 155;
							}
						}
					}
				}
				break;
				case 5: // Random
				{
					static bool swap;
					if (GetKeyState(0x12))
					{
						if (Hacks.SendPacket)
							yaw -= 105;
						else
						{
							if (NextLBYUpdate())
								yaw -= 133.8f;
							else
								yaw += 93;
						}
					}
					else
					{
						if (Hacks.SendPacket)
							yaw += 105;
						else
						{
							if (NextLBYUpdate())
								yaw += 133.8f;
							else
								yaw -= 93;
						}
					}
				}
				break;
				case 6:
				{// 180Z
					float randfloat = rand() % -180 + 360;
					static int ChokedPackets = -1;
					ChokedPackets++;

					if (NextLBYUpdate())
					{
						yaw -= randfloat;
					}
					else
					{
						yaw += 90;
					}
					if (ChokedPackets < 1)
					{
						Hacks.SendPacket = false;
						yaw -= randfloat;
					}
					else
					{
						Hacks.SendPacket = true;
						yaw += 90;
						ChokedPackets = -1;
					}
				}
				break;
				case 7:


					if (GetKeyState(0x27)) // a
					{
						yaw = 90.f + state * 34.f;
						if (state > 1)
							yaw = 90.f + 34.8f;
					}
					if (GetKeyState(0x25)) //d 		
					{
						yaw = -90.f - state * 34.f;
						if (state > 1)
							yaw = -90.f - 34.8f;
					}
					break;
				case 8: // Static
					yaw = Options.Ragebot.AntiAim.Move.iYawAdd;
					break;
				case 9: // Backwards Corruption
					switch (timeStamp % 8)
					{
					case 1: yaw = 170 + rand() % ((90 - 1) + 1) + 1;  break;
					case 2: yaw = 180;  break;
					case 3: yaw = 170 + rand() % ((180 - 90) + 1) + 1;  break;
					case 4: yaw = 180;  break;
					case 5: yaw = 170 + rand() % (((-90) - (-180)) + 1) + 1;  break;
					case 6: yaw = 180;  break;
					case 7: yaw = 170 + rand() % (((-1) - (-90)) + 1) + 1;  break;
					case 8: yaw = 180;  break;
					}
					break;
				}

				if (Options.Ragebot.AntiAim.Move.lockside3)
				{

					int W, H, cW, cH;
					Interfaces.pEngine->GetScreenSize(W, H);


					cW = W / 2;
					cH = H / 2;

					if (GetKeyState(0x27)) // a
					{
						if(NextLBYUpdate)
						yaw = 90;
						else
							yaw = -90;

					}
					else if (GetKeyState(0x25)) //d 		
					{
						if (NextLBYUpdate)

						yaw = -90;
					else
						yaw = 90;
					}


				}
			}

			switch (Options.Ragebot.AntiAim.Stand.iPitch)
			{
			case 0:
				break;
			case 1: //down
				pitch = 88.99f;
				break;
			case 2: //up
				pitch = -88.99f;
				break;
			case 3: // Minimal
				pitch = 89;
				if (Hacks.LocalWeapon->isSniper())
					pitch = 85;
				else if (Hacks.LocalWeapon->isRifle() || Hacks.LocalWeapon->isSmgW())
					pitch = 80;
				else if (Hacks.LocalWeapon->isPistol())
					pitch = 89;
				else if (Hacks.LocalWeapon->isMachineW())
					pitch = 78.50;
				else if (Hacks.LocalWeapon->isShotgun())
					pitch = 75.00;
				break;
			case 4:
				pitch = rand();
				break;
			case 5: // Fake Down
				pitch = -991;
				break;
			case 6: // Fake Up
				pitch = 991;
				break;
			case 7: // Fake Zero
				pitch = 1080;
				break;
			}

			if (Options.Ragebot.AntiAim.Move.bAtTargets)
				AtTarget(view);
		}

		if (Options.Ragebot.AntiAim.Move.lockside3)
		{

			int W, H, cW, cH;
			Interfaces.pEngine->GetScreenSize(W, H);


			cW = W / 2;
			cH = H / 2;


			if (GetKeyState(0x27)) // a
			{
				yaw = -90;
			}
			else if (GetKeyState(0x25)) //d 		
			{
				yaw = 90;
			}


		}

		//Hacks.CurrentCmd->viewangles.y = view.y + yaw;
		Hacks.CurrentCmd->viewangles.y = view.y + yaw;
		Hacks.CurrentCmd->viewangles.x = pitch;

	}

	bool isEdging(CBaseEntity* pLocalBaseEntity, CInput::CUserCmd* cmd, float flWall, float flCornor, bool& bSendPacket, Vector& view)
	{
		Ray_t ray;
		trace_t tr;

		bool isEdging;

		CTraceFilter traceFilter;
		traceFilter.pSkip = pLocalBaseEntity;

		auto bRetVal = false;
		auto vecCurPos = pLocalBaseEntity->GetEyePosition();
		Vector meme = cmd->viewangles;
		Misc::NormalizeVector(meme);
		int iPitch = Options.Ragebot.AntiAim.Edge.iPitch;
		int iYaw = Options.Ragebot.AntiAim.Edge.iYaw;
		int iFakeYaw = Options.Ragebot.AntiAim.Edge.iFakeYaw;
		int random;
		float setyaw = 0;
		float setpitch = 0;
		bool switchb = cmd->command_number % 2;

		for (float i = 0; i < 360; i += 2)
		{
			Vector vecDummy(10.f, meme.y, 0.f);
			vecDummy.y += i;

			Misc::NormalizeVector(vecDummy);

			Vector vecForward;
			Misc::AngleVectors3(vecDummy, vecForward);

			auto flLength = ((16.f + 3.f) + ((16.f + 3.f) * sin(DEG2RAD(10.f)))) + 7.f;
			vecForward *= flLength;

			ray.Init(vecCurPos, (vecCurPos + vecForward));
			Interfaces.pTrace->TraceRay(ray, MASK_SHOT, (CTraceFilter *)&traceFilter, &tr);

			if (tr.fraction != 1.0f)
			{
				Vector qAngles;
				auto vecNegate = tr.plane.normal;

				vecNegate *= -1.f;
				Misc::VectorAngles3(vecNegate, qAngles);

				vecDummy.y = qAngles.y;

				Misc::NormalizeVector(vecDummy);
				trace_t leftTrace, rightTrace;

				Vector vecLeft;
				Misc::AngleVectors3(vecDummy + Vector(0.f, 30.f, 0.f), vecLeft);

				Vector vecRight;
				Misc::AngleVectors3(vecDummy - Vector(0.f, 30.f, 0.f), vecRight);

				vecLeft *= (flLength + (flLength * sin(DEG2RAD(30.f))));
				vecRight *= (flLength + (flLength * sin(DEG2RAD(30.f))));

				ray.Init(vecCurPos, (vecCurPos + vecLeft));
				Interfaces.pTrace->TraceRay(ray, MASK_SHOT, (CTraceFilter*)&traceFilter, &leftTrace);

				ray.Init(vecCurPos, (vecCurPos + vecRight));
				Interfaces.pTrace->TraceRay(ray, MASK_SHOT, (CTraceFilter*)&traceFilter, &rightTrace);

				if ((leftTrace.fraction == 1.f) && (rightTrace.fraction != 1.f))
					vecDummy.y -= flCornor; // left
				else if ((leftTrace.fraction != 1.f) && (rightTrace.fraction == 1.f))
					vecDummy.y += flCornor; // right			
				cmd->viewangles.x = setpitch;
				cmd->viewangles.y = vecDummy.y + setyaw;
				bRetVal = true;
			}
		}
		return bRetVal;

		isEdging = true;
	}

	bool Edge(CBaseEntity* pLocalBaseEntity, CInput::CUserCmd* cmd, float flWall, float flCornor, bool& bSendPacket, Vector& view)
	{
		long currentTime_ms = std::chrono::duration_cast< std::chrono::seconds >(std::chrono::system_clock::now().time_since_epoch()).count();
		static long timeStamp = currentTime_ms;

		timeStamp = currentTime_ms;

		static int SpinYaw = 0;
		static float StoredAng = 0;
		static bool fake;
		fake = !fake;

		//static int jitter = 0;
		static int last = 0;
		int help = {};
		int Fake = rand() % 3;
		static int Spin[2] = { 0, 0 };
		for (int& s : Spin)
			if (s > 180)
				s -= 360;
			else if (s < -180)
				s += 360;

		Hacks.SendPacket = true;

		Ray_t ray;
		trace_t tr;

		CTraceFilter traceFilter;
		traceFilter.pSkip = pLocalBaseEntity;

		auto bRetVal = false;
		auto vecCurPos = pLocalBaseEntity->GetEyePosition();
		Vector meme = cmd->viewangles;
		Misc::NormalizeVector(meme);
		int iPitch = Options.Ragebot.AntiAim.Edge.iPitch;
		int iYaw = Options.Ragebot.AntiAim.Edge.iYaw;
		int iFakeYaw = Options.Ragebot.AntiAim.Edge.iFakeYaw;
		int maxJitter;
		int random;
		float temp;
		double factor;
		float setyaw = 0;
		float setpitch = 0;
		bool switchb = cmd->command_number % 2;

		int iFakeYawAdd = Options.Ragebot.AntiAim.Edge.iFakeYawAdd;
		int iYawAdd = Options.Ragebot.AntiAim.Edge.iYawAdd;
		if (fake)
		{
			Hacks.SendPacket = true;

			switch (Options.Ragebot.AntiAim.Edge.iFakeYaw)
			{
			case 0:
				break;
			case 1: // Forwards
				setyaw = 0 - Options.Ragebot.AntiAim.Edge.iFakeYawAdd;
				break;
			case 2: // Backwards
				setyaw = 180 - Options.Ragebot.AntiAim.Edge.iFakeYawAdd;
				break;
			case 3: // Backwards Jitter
				setyaw -= 180 - Options.Ragebot.AntiAim.Edge.iFakeYawAdd;
				random = rand() % 100;
				maxJitter = rand() % (85 - 70 + 1) + 70;
				temp = maxJitter - (rand() % maxJitter);
				if (random < 35 + (rand() % 15))
					setyaw -= temp;
				else if (random < 85 + (rand() % 15))
					setyaw += temp;
				break;
			case 4: // Sideways
				if (Options.Ragebot.AntiAim.Edge.iFakeYawAdd >= 35 && Options.Ragebot.AntiAim.Edge.iFakeYawAdd <= 95)
					setyaw = 90;
				else if (Options.Ragebot.AntiAim.Edge.iFakeYawAdd <= -35 && Options.Ragebot.AntiAim.Edge.iFakeYawAdd >= -95)
					setyaw = -90;
				break;
			case 5: // Random
				setyaw = rand();
				break;
			case 6: // Localview
				setyaw = Hacks.CurrentCmd->viewangles.y;
				break;
			case 7: // Static
				setyaw = Options.Ragebot.AntiAim.Edge.iFakeYaw;
				break;
			case 8:
				if (Options.Ragebot.AntiAim.Edge.iYaw == 1)
				{
					setyaw = 180 - Options.Ragebot.AntiAim.Edge.iYawAdd;
				}
				if (Options.Ragebot.AntiAim.Edge.iYaw == 2)
				{
					setyaw = 0 - Options.Ragebot.AntiAim.Edge.iYawAdd;
				}
				else if (Options.Ragebot.AntiAim.Edge.iYaw == 3)
				{
					setyaw = 0 - iFakeYawAdd;
					random = rand() % 100;
					maxJitter = rand() % (85 - 70 + 1) + 70;
					temp = maxJitter - (rand() % maxJitter);
					if (random < 35 + (rand() % 15))
						setyaw -= temp;
					else if (random < 85 + (rand() % 15))
						setyaw += temp;
				}
				else if (Options.Ragebot.AntiAim.Edge.iYaw == 5)
				{
					if (flip)
						SpinYaw -= 90;
					else
						SpinYaw += 90;
					flip != flip;
					StoredAng = SpinYaw;
					setyaw = SpinYaw;
				}
				else if (Options.Ragebot.AntiAim.Edge.iYaw == 6)
				{
					if (Options.Ragebot.AntiAim.Edge.iYawAdd >= 35 && Options.Ragebot.AntiAim.Edge.iYawAdd <= 95)
						setyaw = -90;
					else if (Options.Ragebot.AntiAim.Edge.iYawAdd <= -35 && Options.Ragebot.AntiAim.Edge.iYawAdd >= -95)
						setyaw = 90;
				}
				else if (Options.Ragebot.AntiAim.Edge.iYaw == 7)
				{
					setyaw -= rand();
				}
				else if (Options.Ragebot.AntiAim.Edge.iYaw == 8)
				{
					setyaw -= Options.Ragebot.AntiAim.Edge.iYawAdd;
				}
				else if (Options.Ragebot.AntiAim.Edge.iYaw == 9)
				{
					switch (timeStamp % 8)
					{
					case 1: setyaw -= 0 - rand() % ((90 - 1) + 1) + 1;  break;
					case 2: setyaw -= 0;  break;
					case 3: setyaw -= 0 - rand() % ((180 - 90) + 1) + 1;  break;
					case 4: setyaw -= 0;  break;
					case 5: setyaw -= 0 - rand() % (((-90) - (-180)) + 1) + 1;  break;
					case 6: setyaw -= 0;  break;
					case 7: setyaw -= 0 - rand() % (((-1) - (-90)) + 1) + 1;  break;
					case 8: setyaw -= 0;  break;
					}
				}
				break;
			}
		}
		else
		{
			Hacks.SendPacket = false;

			switch (Options.Ragebot.AntiAim.Edge.iYaw)
			{
			case 0:
				break;
			case 1: // Forward
				setyaw = 0 - Options.Ragebot.AntiAim.Edge.iYawAdd;
				break;
			case 2: // Backwards
				setyaw = 180 - Options.Ragebot.AntiAim.Edge.iYawAdd;
				break;
			case 3: // Backwards Jitter
			{
				setyaw = 180 + iFakeYawAdd;
				random = rand() % 100;
				maxJitter = rand() % (85 - 70 + 1) + 70;
				temp = maxJitter - (rand() % maxJitter);
				if (random < 35 + (rand() % 15))
					setyaw -= temp;
				else if (random < 85 + (rand() % 15))
					setyaw += temp;
			}
			break;
			case 4: // Spin
				if (flip)
					SpinYaw -= 35;
				else
					SpinYaw += 35;
				flip != flip;
				StoredAng = SpinYaw;
				setyaw = SpinYaw;
				break;
			case 5: // Sideways
				if (Options.Ragebot.AntiAim.Edge.iFakeYawAdd >= 35 && Options.Ragebot.AntiAim.Edge.iFakeYawAdd <= 95)
					setyaw = 90;
				else if (Options.Ragebot.AntiAim.Edge.iFakeYawAdd <= -35 && Options.Ragebot.AntiAim.Edge.iFakeYawAdd >= -95)
					setyaw = -90;
				break;
			case 6: // Random
				setyaw = rand();
				break;
			case 7: // Static
				setyaw = Options.Ragebot.AntiAim.Edge.iYawAdd;
				break;
			case 8: // Backwards Corruption
				switch (timeStamp % 8)
				{
				case 1: setyaw = 170 + rand() % ((90 - 1) + 1) + 1;  break;
				case 2: setyaw = 180;  break;
				case 3: setyaw = 170 + rand() % ((180 - 90) + 1) + 1;  break;
				case 4: setyaw = 180;  break;
				case 5: setyaw = 170 + rand() % (((-90) - (-180)) + 1) + 1;  break;
				case 6: setyaw = 180;  break;
				case 7: setyaw = 170 + rand() % (((-1) - (-90)) + 1) + 1;  break;
				case 8: setyaw = 180;  break;
				}
				break;
			}
		}


		switch (Options.Ragebot.AntiAim.Edge.iPitch)
		{
		case 0:
			break;
		case 1: //down
			setpitch = 88.99f;
			break;
		case 2: //up
			setpitch = -88.99f;
			break;
		case 3: // Minimal
			setpitch = 89;
			if (Hacks.LocalWeapon->isSniper())
				setpitch = 85;
			else if (Hacks.LocalWeapon->isRifle() || Hacks.LocalWeapon->isSmgW())
				setpitch = 80;
			else if (Hacks.LocalWeapon->isPistol())
				setpitch = 89;
			else if (Hacks.LocalWeapon->isMachineW())
				setpitch = 78.50;
			else if (Hacks.LocalWeapon->isShotgun())
				setpitch = 75.00;
			break;
		case 4:
			setpitch = rand();
			break;
		case 5: // Fake Down
			setpitch = -991;
			break;
		case 6: // Fake Up
			setpitch = 991;
			break;
		case 7: // Fake Zero
			setpitch = 1080;
			break;
		}

		Hacks.CurrentCmd->viewangles.y = view.y + setyaw;
		Hacks.CurrentCmd->viewangles.x = setpitch;

		for (float i = 0; i < 360; i += 2)
		{
			Vector vecDummy(10.f, meme.y, 0.f);
			vecDummy.y += i;

			Misc::NormalizeVector(vecDummy);

			Vector vecForward;
			Misc::AngleVectors3(vecDummy, vecForward);

			auto flLength = ((16.f + 3.f) + ((16.f + 3.f) * sin(DEG2RAD(10.f)))) + 7.f;
			vecForward *= flLength;

			ray.Init(vecCurPos, (vecCurPos + vecForward));
			Interfaces.pTrace->TraceRay(ray, MASK_SHOT, (CTraceFilter *)&traceFilter, &tr);

			if (tr.fraction != 1.0f)
			{
				Vector qAngles;
				auto vecNegate = tr.plane.normal;

				vecNegate *= -1.f;
				Misc::VectorAngles3(vecNegate, qAngles);

				vecDummy.y = qAngles.y;

				Misc::NormalizeVector(vecDummy);
				trace_t leftTrace, rightTrace;

				Vector vecLeft;
				Misc::AngleVectors3(vecDummy + Vector(0.f, 30.f, 0.f), vecLeft);

				Vector vecRight;
				Misc::AngleVectors3(vecDummy - Vector(0.f, 30.f, 0.f), vecRight);

				vecLeft *= (flLength + (flLength * sin(DEG2RAD(30.f))));
				vecRight *= (flLength + (flLength * sin(DEG2RAD(30.f))));

				ray.Init(vecCurPos, (vecCurPos + vecLeft));
				Interfaces.pTrace->TraceRay(ray, MASK_SHOT, (CTraceFilter*)&traceFilter, &leftTrace);

				ray.Init(vecCurPos, (vecCurPos + vecRight));
				Interfaces.pTrace->TraceRay(ray, MASK_SHOT, (CTraceFilter*)&traceFilter, &rightTrace);

				if ((leftTrace.fraction == 1.f) && (rightTrace.fraction != 1.f))
					vecDummy.y -= flCornor; // left
				else if ((leftTrace.fraction != 1.f) && (rightTrace.fraction == 1.f))
					vecDummy.y += flCornor; // right			
				cmd->viewangles.x = setpitch;
				cmd->viewangles.y = vecDummy.y + setyaw;
				bRetVal = true;
			}
		}
		return bRetVal;
	}


public:
	void Run()
	{
		if (Options.Ragebot.AntiAim.Move.bMemeWalk)
			MemeWalk();

		if (Options.Ragebot.AntiAim.Move.lockside3)
		{
			int W, H, cW, cH;
			Interfaces.pEngine->GetScreenSize(W, H);

			cW = W / 2;
			cH = H / 2;
			float setyaw = 0;


			if (KEY_DOWN(0x41)) // a
			{
				setyaw = -90;
			}
			if (KEY_DOWN(0x44)) //d 		
			{
				setyaw = 90;
			}

			Interfaces.pSurface->DrawT(cW - 955, cH + 73, CColor(112, 230, 20, 255), Hacks.Font_LBY, false, "LOCK SIDE");
		}

		if (Options.Ragebot.AntiAim.Misc.bFakeWalk)
			FakeWalk();
		if (Options.Misc.Misc2.bBlockBot && GetAsyncKeyState(Options.Misc.Misc2.iBlockBotKey))
			return;
		
		if (Hacks.LocalPlayer->GetMoveType() == MOVETYPE_LADDER || Hacks.LocalPlayer->GetMoveType() == MOVETYPE_NOCLIP)
			return;
		if (Hacks.CurrentCmd->buttons & IN_USE)
			return;
		ShouldAA = true;
		Vector view = Hacks.CurrentCmd->viewangles;
		
		bool move; bool move2; QAngle newAngle;

		//isEdging(Hacks.LocalPlayer, Hacks.CurrentCmd, 0.f, 90.f, Hacks.SendPacket, view);
		if (ShouldAA && Options.Ragebot.AntiAim.Stand.bEnableAntiAim)
		{
			if (Options.Ragebot.AntiAim.Stand.FreeStanding)
			{
				if (Hacks.LocalPlayer->GetVecVelocity().Length2D() < 80 && Options.Ragebot.AntiAim.Stand.bEnableAntiAim)
					BreakLowerbodyFreestand();

				else if (Hacks.LocalPlayer->GetVecVelocity().Length2D() > 80 && Options.Ragebot.AntiAim.Stand.bEnableAntiAim)
					BreakLowerbodyFreestand();
			}
			else
			{
				
					if (Hacks.LocalPlayer->GetVecVelocity().Length2D() < 80 && Options.Ragebot.AntiAim.Stand.bEnableAntiAim)
						DoAA(0, view);

					else if (Hacks.LocalPlayer->GetVecVelocity().Length2D() > 80 && Options.Ragebot.AntiAim.Stand.bEnableAntiAim)
						DoAA(0, view);
			}
			if (Options.Ragebot.AntiAim.Edge.bEnableEdge)
			{
				if (isEdging(Hacks.LocalPlayer, Hacks.CurrentCmd, 0.f, 90.f, Hacks.SendPacket, view))
					if (Hacks.LocalPlayer->GetVecVelocity().Length2D() < 300 && (Hacks.LocalPlayer->GetFlags() & FL_ONGROUND))
					{
						bool bEdge = Edge(Hacks.LocalPlayer, Hacks.CurrentCmd, 0.f, 90.f, Hacks.SendPacket, view);
						if (bEdge)
							return;
					}
			}
		}
		Fakelag();
	}
} AA;



/*	float Pitch(int selected) {
		float tmp = 0.f;

		switch (selected) {
		case 1:
			tmp = 89.f; //Down
			break;
		case 2:
			tmp = 70.f; //Down test
			break;
		case 3:
			tmp = 0.f; //Mid
			break;
		case 4:
			tmp = -89.f; //Up
			break;
		}
		return tmp;
	}

	float Yaw(int selected, CInput::CUserCmd*  pCmd, bool stop = false) {
		//Forward jitter2
		static int fJitter1 = 1;
		static float fJitterAngles[] = { 10.f,-10.f,-30.f,30.f,0.f };
	
		float tmp;
		static bool flip = false;

		switch (selected) {
		case 1:
			tmp = flip ? 90.f : -90.f; //Jitter LBY Basic
			break;
		case 2:
			tmp = flip ? -90.f : 90.f; //Jitter LBY Reversed Basic
			break;
		case 3:
			tmp = flip ? 140.f : -140.f; //Back Jitter LBY Basic
			break;
		case 4:
			tmp = (float)(fmod(Interfaces.pGlobalVars->GlobalVars()->curtime / Options.Ragebot.AntiAim.Stand.SpinSpeed * 360.0f, 360.0f)); //Spin LBY Basic
			break;
		case 5:
			tmp = 180.f; //Backward Basic
			break;
		case 6:
			tmp = Hacks.LocalPlayer->pelvisangs(); //Side Unhittable
			break;
		case 7:
			tmp = fJitterAngles[pCmd->tick_count % fJitter1];
			fJitter1++;

			if (fJitter1 > ARRAYSIZE(fJitterAngles))
				fJitter1 = 1;
			break;
		}

		if (Options.Ragebot.AntiAim.Stand.bLocalLowerbodyUpdate && !stop) {
			flip = !flip;
			Options.Ragebot.AntiAim.Stand.bLocalLowerbodyUpdate = false;
		}

		return tmp;
	}

	float FakeYawUnhittable(int selected, CInput::CUserCmd*  pCmd)
	{
		float tmp;

		switch (selected)
		{
		case 0:
			tmp = Options.Ragebot.AntiAim.Stand.UnHit ? -60.f : 60.f; //rand 30-140
			break;
		case 1:
			tmp = Options.Ragebot.AntiAim.Stand.UnHit ? -90.f : 90.f; //rand 30-140
			break;
		case 2:
			tmp = Options.Ragebot.AntiAim.Stand.UnHit ? RandomFloat(-30.f, -140.f) : RandomFloat(30.f, 140.f); //rand 30-140
			break;
		case 3:
			if (pCmd->tick_count % 2)
			{
				tmp = Options.Ragebot.AntiAim.Stand.UnHit ? -160.f : 160.f; // -160 - +160
			}
			else
			{
				tmp = Options.Ragebot.AntiAim.Stand.UnHit ? -80.f : 80.f; //
			}
			break;
		case 4:
			tmp = Options.Ragebot.AntiAim.Stand.UnHit ? -160.f : 160.f; // -160 +160
			break;
		case 5:
			tmp = Options.Ragebot.AntiAim.Stand.UnHit ? RandomFloat(-140.f, -160.f) : RandomFloat(140.f, 160.f);
			break;
		case 6:
			tmp = Options.Ragebot.AntiAim.Stand.UnHit ? -135.f : 135.f; // -160 +160
			break;
		case 7:
			tmp = Options.Ragebot.AntiAim.Stand.UnHit ? -70.f : 70.f; // -160 +160
			break;
		}

		return tmp;
	}





	float doSqchAA(bool& move, bool& move2, QAngle newAngle, CInput::CUserCmd* pCmd, CBaseCombatWeapon* pActiveWeapon) {
		float flUpdateTime = move2 ? 0.22000001f : 1.1f; //+ interval cause fakelag
		static auto start = std::chrono::high_resolution_clock::now();


		if (move) {
			start = std::chrono::high_resolution_clock::now();
			move = false;
		}

		auto end = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> delta = end - start;

		//Find bestside
		QAngle tmpAngle = newAngle;
		if (Options.Ragebot.AntiAim.Stand.g_unhittableBestSide)
		{
			Misc::FindBestSide(Hacks.LocalPlayer, 512.f, pCmd, tmpAngle);
		}

		if (delta.count() >= flUpdateTime) {
			move2 = false;
			start = std::chrono::high_resolution_clock::now();

			if (!Options.Ragebot.AntiAim.Stand.g_unhittableBestSide)
				return  Options.Ragebot.AntiAim.Stand.UnHit ? newAngle.y + 90.f : newAngle.y - 90.f; //90,-90

			return tmpAngle.y + 180;//180
		}

		if (move2)
			return newAngle.y + Yaw(Options.Ragebot.AntiAim.Stand.iFakeYaw, pCmd, true);

		if (!Options.Ragebot.AntiAim.Stand.g_unhittableBestSide)
			return newAngle.y + FakeYawUnhittable(Options.Ragebot.AntiAim.Stand.g_fakeUnhittable, pCmd);

		return tmpAngle.y - 70;//tmpAngle.y
	}
	*/
