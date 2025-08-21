#pragma once
#include "../../stdafx.h"
#include "CreateMoveETC.h"
#include "../../SDK/Math/Math.h"
FORCEINLINE vec_t DotProduct(const Vector& a, const Vector& b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}
Vector MEME2(Vector EntityPos, Vector LocalPlayerPos, int posX, int posY, int sizeX, int sizeY, float angle, float zoom, bool * viewCheck)
{
	float r_1, r_2;
	float x_1, y_1;

	r_1 = -(EntityPos.y - LocalPlayerPos.y);
	r_2 = EntityPos.x - LocalPlayerPos.x;
	float Yaw = angle - 90.0f;

	float yawToRadian = Yaw * (float)(M_PI / 180.0F);
	x_1 = (float)(r_2 * (float)cos((double)(yawToRadian)) - r_1 * sin((double)(yawToRadian))) / 20;
	y_1 = (float)(r_2 * (float)sin((double)(yawToRadian)) + r_1 * cos((double)(yawToRadian))) / 20;

	*viewCheck = y_1 < 0;

	x_1 *= zoom;
	y_1 *= zoom;

	int sizX = sizeX / 2;
	int sizY = sizeY / 2;

	x_1 += sizX;
	y_1 += sizY;

	if (x_1 < 5)
		x_1 = 5;

	if (x_1 > sizeX - 5)
		x_1 = sizeX - 5;

	if (y_1 < 5)
		y_1 = 5;

	if (y_1 > sizeY - 5)
		y_1 = sizeY - 5;


	x_1 += posX;
	y_1 += posY;


	return Vector(x_1, y_1, 0);
}
/*char* GetGunName()
{
	int WeaponId = *this->GetItemDefinitionIndex();
	switch (WeaponId)
	{
	case WEAPON_AK47:
		return "ak47";
		break;
	case WEAPON_AUG:
		return "aug";
		break;
	case weapon_revolver:
		return "revolver";
		break;
	case WEAPON_AWP:
		return "awp";
		break;
	case WEAPON_BIZON:
		return "bizon";
		break;
	case WEAPON_C4:
		return "bomb";
		break;
	case WEAPON_CZ75:
		return "cz75";
		break;
	case WEAPON_DEAGLE:
		return "deagle";
		break;
	case WEAPON_DECOY:
		return "decoy";
		break;
	case WEAPON_DUALBERETTA:
		return "dual beretta";
		break;
	case WEAPON_FAMAS:
		return "famas";
		break;
	case WEAPON_FIVESEVEN:
		return "fiveseven";
		break;
	case WEAPON_FLASHBANG:
		return "flash";
		break;
	case WEAPON_G3SG1:
		return "g3sg1";
		break;
	case WEAPON_GALIL:
		return "galil";
		break;
	case WEAPON_GLOCK:
		return "glock";
		break;
	case WEAPON_INC:
		return "molotov";
		break;
	case WEAPON_MOLOTOV:
		return "molotov";
		break;
	case WEAPON_SSG08:
		return "ssg08";
		break;
	case WEAPON_HEGRENADE:
		return "hegrenade";
		break;
	case WEAPON_M249:
		return "m249";
		break;
	case WEAPON_M4A1:
		return "m4a4";
		break;
	case WEAPON_MAC10:
		return "mac10";
		break;
	case WEAPON_MAG7:
		return "mag7";
		break;
	case WEAPON_MP7:
		return "mp7";
		break;
	case WEAPON_MP9:
		return "mp9";
		break;
	case WEAPON_NOVA:
		return "nova";
		break;
	case WEAPON_NEGEV:
		return "negev";
		break;
	case WEAPON_P250:
		return "p250";
		break;
	case WEAPON_P90:
		return "p90";
		break;
	case WEAPON_SAWEDOFF:
		return "sawedoff";
		break;
	case WEAPON_SCAR20:
		return "scar-20";
		break;
	case WEAPON_SMOKEGRENADE:
		return "smoke";
		break;
	case WEAPON_SG553:
		return "sg553";
		break;
	case WEAPON_TEC9:
		return "tec9";
		break;
	case WEAPON_USP:
		return "p2000";
		break;
	case WEAPON_USPS:
		return "usp";
		break;
	case WEAPON_UMP45:
		return "ump45";
		break;
	case WEAPON_XM1014:
		return "xm1014";
		break;
	case WEAPON_ZEUSX27:
		return "zeus";
		break;
	case WEAPON_M4A1S:
		return "m4a1-s";
		break;
	case weapon_knife_butterfly:
		return "knife_butterfly";
		break;
	case weapon_knife_falchion:
		return "knife_falchion";
		break;
	case weapon_knife_flip:
		return "knife_flip";
		break;
	case weapon_knife_gut:
		return "knife_gut";
		break;
	case weapon_knife_karambit:
		return "knife_karambit";
		break;
	case weapon_knife_m9_bayonet:
		return "knife_m9_bayonet";
		break;
	case weapon_knife_survival_bowie:
		return "knife_bowie";
		break;
	case weapon_bayonet:
		return "knife_bayonet";
		break;
	}
	if (e())
		return "knife";
	return "";
}*/
#define MAX_PLAYERS 64
extern float lastSeenTime[MAX_PLAYERS];
float lastSeenTime[MAX_PLAYERS];

static int GetDormantAlpha(CBaseEntity *entity)
{
	if (!entity->IsDormant())
		return 50;

	float elapsed =Interfaces.pGlobalVars->curtime - lastSeenTime[entity->GetIndex()];
	float fadeTime = 3.0f;

	return min(50, 255 - (int)(elapsed * 255 / fadeTime));
}
/*if (Options.Visuals.Esp.EspBox.Radar)
{

	ImGuiStyle& style = ImGui::GetStyle();
	ImVec2 oldPadding = style.WindowPadding;
	float oldAlpha = style.Colors[ImGuiCol_WindowBg].w;
	style.WindowPadding = ImVec2(0, 0);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.95f);
	ImGui::Begin(XorStr("Radar"), &Options.Visuals.Esp.EspBox.Radar, ImVec2(200, 200), 0.4F, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders);
	{


		//	_drawList->AddRect(ImVec2(pos.x - 6, pos.y - 6), ImVec2(pos.x + siz.x + 6, pos.y + siz.y + 6), Color::Black().GetU32(), 0.0F, -1, 1.5f);
		//_drawList->AddRect(ImVec2(pos.x - 2, pos.y - 2), ImVec2(pos.x + siz.x + 2, pos.y + siz.y + 2), Color::Black().GetU32(), 0.0F, -1, 1);

		//_drawList->AddRect(ImVec2(pos.x - 2, pos.y - 2), ImVec2(pos.x + siz.x + 2, pos.y + siz.y + 2), Color::Black().GetU32(), 0.0F, -1, 2);
		//_drawList->AddRect(ImVec2(pos.x - 2, pos.y - 2), ImVec2(pos.x + siz.x + 2, pos.y + siz.y + 2), Color::Silver().GetU32(), 0.0F, -1, 1.1f);

		//if (G::NextResetRadar)
		//	{
		//		ImGui::SetWindowSize(ImVec2(200, 200));
		//		G::NextResetRadar = false;
		//	}

		ImDrawList* windowDrawList = ImGui::GetWindowDrawList();
		windowDrawList->AddLine(ImVec2(pos.x + (siz.x / 2), pos.y + 0), ImVec2(pos.x + (siz.x / 2), pos.y + siz.y), 0xFF000000, 1.5f);
		windowDrawList->AddLine(ImVec2(pos.x + 0, pos.y + (siz.y / 2)), ImVec2(pos.x + siz.x, pos.y + (siz.y / 2)), 0xFF000000, 1.5f);

		// Rendering players

		if (Interfaces.pEngine->IsInGame() && Interfaces.pEngine->IsConnected())
		{
			CBaseEntity* pLocalEntity = Hacks.LocalPlayer;
			if (pLocalEntity)
			{
				Vector LocalPos = pLocalEntity->GetEyePosition();
				QAngle ang;
				Interfaces.pEngine->GetViewAngles(ang);
				for (int i = 0; i < Interfaces.pEngine->GetMaxClients(); i++) {
					CBaseEntity* pBaseEntity = Interfaces.pEntList->GetClientEntity(i);

					if (!pBaseEntity)
						continue;
					if (!pBaseEntity->isValidPlayer())
						continue;
					if (pBaseEntity == pLocalEntity)
						continue;

					bool bIsEnemy = pLocalEntity->GetTeam() != pBaseEntity->GetTeam();
					if (pLocalEntity->GetTeam() == pBaseEntity->GetTeam())
						continue;
					//bool isVisibled = G::VisibledPlayers[i]; /*U::IsVisible(LocalPos, pBaseEntity->GetEyePosition(), pLocalEntity, pBaseEntity, Options::Radar::SmokeCheck);*/

					//	if (false)
					//	continue;

				/*	bool viewCheck = false;
					Vector EntityPos = MEME(pBaseEntity->GetAbsOrigin(), LocalPos, pos.x, pos.y, siz.x, siz.y, ang.y, 2, &viewCheck);

					//if (false)
					//isVisibled = false;
					//ImU32 clr = (bIsEnemy ? (isVisibled ? Color::LightGreen() : Color::Blue()) : Color::White()).GetU32();
					//	ImU32 clr = /*bIsEnemy ? (isVisibled ? *///Color::White().GetU32();// : Color::Black()) : Color::Blue()).GetU32();

					//if (false)
					//	continue;

					/*int s = 4;
					windowDrawList->AddRectFilled(ImVec2(EntityPos.x - s, EntityPos.y - s),
						ImVec2(EntityPos.x + s, EntityPos.y + s),
						0xFFFFFFFF);
					break;


				}
			}
		}
	}

	ImGui::End();
	style.WindowPadding = oldPadding;
	style.Colors[ImGuiCol_WindowBg].w = oldAlpha;
}*/
static char weaponNames[55][32] = {
	"none",
	"deagle",
	"dual beretta",
	"fiveseven",
	"glock",
	"unknown",
	"unknown",
	"ak47",
	"aug",
	"awp",
	"famas",
	"g3sg1",
	"unknown",
	"galil",
	"m249",
	"unknown",
	"m4a4",
	"mac-10",
	"unknown",
	"p90",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"ump-45",
	"xm1014",
	"bizon",
	"mag7",
	"negev",
	"sawedoff",
	"tec9",
	"zeus",
	"p2000",
	"mp7",
	"mp9",
	"nova",
	"p250",
	"unknown",
	"scar-20",
	"sg553",
	"ssg08",
	"unknown",
	"knife",
	"flash",
	"hegrenade",
	"smoke",
	"molotov",
	"decoy",
	"molotov",
	"bomb",
	"m4a1-s",
	"uspS",
	"cz75",
	"revolver",
	"knife"

}; 



char* WeaponIDName(int weaponID) {
	char* name = 0;
	if (weaponID == 59) //terrorist knife
		name = weaponNames[54];
	if (weaponID == 60) //M4A1-S
		name = weaponNames[50];
	if (weaponID == 61) //USP-S
		name = weaponNames[51];
	if (weaponID == 63) //CZ-75A
		name = weaponNames[52];
	if (weaponID == 64)//revolver
		name = weaponNames[53];
	if (weaponID > 64) //non-faction-specific knife
		name = "KNIFE";// weaponNames[54];
	if (!name) //none of the above
		name = weaponNames[weaponID];
	return name;
}
class Esp
{
public:



	static std::string GetTimeString()
	{
		time_t current_time;
		struct tm *time_info;
		static char timeString[10];
		time(&current_time);
		time_info = localtime(&current_time);
		strftime(timeString, sizeof(timeString), "%X", time_info);
		return timeString;
	}
	static void bighak()
	{
		int anglereal = Global::curReal;
		int anglefake = Global::curFake;
		/*	IClientEntity *pLocal = hackManager.pLocal();
		Vector src3D, dst3D, forward, src, dst;
		trace_t tr;
		Ray_t ray;
		CTraceFilter filter;

		filter.pSkip = pLocal;
		AngleVectors(QAngle(0, pLocal->GetLowerBodyYaw(), 0), &forward);
		src3D = pLocal->GetOrigin();
		dst3D = src3D + (forward * 35.f); //replace 50 with the length you want the line to have

		ray.Init(src3D, dst3D);

		Interfaces::Trace->TraceRay(ray, 0, &filter, &tr);

		if (!Render::WorldToScreen(src3D, src) || !Render::WorldToScreen(tr.endpos, dst))
		return;

		Render::Line(src.x, src.y, dst.x, dst.y, Color(255, 165, 0, 255));
		Render::Text(dst.x, dst.y, Color(255, 165.f, 0, 255), Render::Fonts::ESP, "LBY");*/


		Vector src3D, dst3D, forward, src, dst;
		trace_t tr;
		Ray_t ray;
		CTraceFilter filter;


		Misc::AngleVectors(QAngle(0, Hacks.LocalPlayer->pelvisangs(), 0), &forward);
		src3D = Hacks.LocalPlayer->GetAbsOrigin();
		dst3D = src3D + (forward * 35.f); //replace 50 with the length you want the line to have

		ray.Init(src3D, dst3D);

		Interfaces.pTrace->TraceRay(ray, 0, &filter, &tr);

		if (g_Math.WorldToScreen(src3D, src) || g_Math.WorldToScreen(tr.endpos, dst))
			return;

		DrawLine(src.x, src.y, dst.x, dst.y, CColor(255, 165, 0, 255));
		Interfaces.pSurface->DrawT(dst.x, dst.y, CColor(255, 255, 255, 255), Hacks.Font_ESP2, false, "LBY");

		Misc::AngleVectors(QAngle(0, anglereal, 0), &forward);
		dst3D = src3D + (forward * 40.f); //replace 50 with the length you want the line to have

		ray.Init(src3D, dst3D);

		Interfaces.pTrace->TraceRay(ray, 0, &filter, &tr);

		if (g_Math.WorldToScreen(src3D, src) || g_Math.WorldToScreen(tr.endpos, dst))
			return;

		DrawLine(src.x, src.y, dst.x, dst.y, CColor(0, 255, 0, 255));
		Interfaces.pSurface->DrawT(dst.x, dst.y, CColor(255, 255, 255, 255), Hacks.Font_ESP2, false, "REAL");

		Misc::AngleVectors(QAngle(0, anglefake, 0), &forward);
		dst3D = src3D + (forward * 45.f); //replace 50 with the length you want the line to have

		ray.Init(src3D, dst3D);

		Interfaces.pTrace->TraceRay(ray, 0, &filter, &tr);

		if (g_Math.WorldToScreen(src3D, src) || g_Math.WorldToScreen(tr.endpos, dst))
			return;

		DrawLine(src.x, src.y, dst.x, dst.y, CColor(255, 0, 0, 255));
		Interfaces.pSurface->DrawT(dst.x, dst.y, CColor(255, 255, 255, 255), Hacks.Font_ESP2, false, "FAKE");
	}
	static void Test()
	{
		if (Interfaces.pEngine->IsInGame() && Interfaces.pEngine->IsConnected() && Hacks.LocalPlayer->isAlive())
		{
			auto Draw3DBox = [](Vector Org, Vector vMin, Vector vMax, int r, int g, int b, int a)
			{
				Vector _min = Org + vMin;
				Vector _max = Org + vMax;
				Vector min, max;

				Vector crnr2 = Vector(_max.x, _min.y, _min.z);
				Vector crnr3 = Vector(_max.x, _min.y, _max.z);
				Vector crnr4 = Vector(_min.x, _min.y, _max.z);
				Vector crnr5 = Vector(_min.x, _max.y, _max.z);
				Vector crnr6 = Vector(_min.x, _max.y, _min.z);
				Vector crnr7 = Vector(_max.x, _max.y, _min.z);
				Vector crnr2_;
				Vector crnr3_;
				Vector crnr4_;
				Vector crnr5_;
				Vector crnr6_;
				Vector crnr7_;

				if (g_Math.WorldToScreen(_min, min) &&
					g_Math.WorldToScreen(_max, max) &&
					g_Math.WorldToScreen(crnr2, crnr2_) &&
					g_Math.WorldToScreen(crnr3, crnr3_) &&
					g_Math.WorldToScreen(crnr4, crnr4_) &&
					g_Math.WorldToScreen(crnr5, crnr5_) &&
					g_Math.WorldToScreen(crnr6, crnr6_) &&
					g_Math.WorldToScreen(crnr7, crnr7_))
				{
					DrawLine(min.x, min.y, crnr2_.x, crnr2_.y, CColor(r, g, b, a));
					DrawLine(min.x, min.y, crnr4_.x, crnr4_.y, CColor(r, g, b, a));
					DrawLine(min.x, min.y, crnr6_.x, crnr6_.y, CColor(r, g, b, a));
					DrawLine(max.x, max.y, crnr5_.x, crnr5_.y, CColor(r, g, b, a));
					DrawLine(max.x, max.y, crnr7_.x, crnr7_.y, CColor(r, g, b, a));
					DrawLine(max.x, max.y, crnr3_.x, crnr3_.y, CColor(r, g, b, a));
					DrawLine(crnr2_.x, crnr2_.y, crnr7_.x, crnr7_.y, CColor(r, g, b, a));
					DrawLine(crnr2_.x, crnr2_.y, crnr3_.x, crnr3_.y, CColor(r, g, b, a));
					DrawLine(crnr4_.x, crnr4_.y, crnr5_.x, crnr5_.y, CColor(r, g, b, a));
					DrawLine(crnr4_.x, crnr4_.y, crnr3_.x, crnr3_.y, CColor(r, g, b, a));
					DrawLine(crnr6_.x, crnr6_.y, crnr5_.x, crnr5_.y, CColor(r, g, b, a));
					DrawLine(crnr6_.x, crnr6_.y, crnr7_.x, crnr7_.y, CColor(r, g, b, a));
				}
			};


			const float TIMEALIVE = 5.f;
			const float GRENADE_COEFFICIENT_OF_RESTITUTION = 0.4f;

			float fStep = 0.1f;
			float fGravity = 800.0f / 8.f;

			Vector vPos, vThrow, vThrow2;
			Vector vStart;

			int iCollisions = 0;

			Vector vViewAngles;
			Interfaces.pEngine->GetViewAngles(vViewAngles);

			vThrow[0] = vViewAngles[0];
			vThrow[1] = vViewAngles[1];
			vThrow[2] = vViewAngles[2];

			if (vThrow[0] < 0)
				vThrow[0] = -10 + vThrow[0] * ((90 - 10) / 90.0);
			else
				vThrow[0] = -10 + vThrow[0] * ((90 + 10) / 90.0);

			float fVel = (90 - vThrow[0]) * 4;
			if (fVel > 500)
				fVel = 500;

			Misc::AngleVectors(vThrow, &vThrow2);

			Vector vEye = Hacks.LocalPlayer->GetEyePosition();
			vStart[0] = vEye[0] + vThrow2[0] * 16;
			vStart[1] = vEye[1] + vThrow2[1] * 16;
			vStart[2] = vEye[2] + vThrow2[2] * 16;

			vThrow2[0] = (vThrow2[0] * fVel) + Hacks.LocalPlayer->GetVecVelocity().x;
			vThrow2[1] = (vThrow2[1] * fVel) + Hacks.LocalPlayer->GetVecVelocity().y;
			vThrow2[2] = (vThrow2[2] * fVel) + Hacks.LocalPlayer->GetVecVelocity().z;	// casualhacker for da magic calc help

			for (float fTime = 0.0f; fTime < TIMEALIVE; fTime += fStep)
			{
				vPos = vStart + vThrow2 * fStep;

				Ray_t ray;
				trace_t tr;
				CTraceFilter loc;
				loc.pSkip = Hacks.LocalPlayer;

				ray.Init(vStart, vPos);
				Interfaces.pTrace->TraceRay(ray, MASK_SOLID, &loc, &tr);
				Vector vOutStart, vOutEnd;
				if (tr.DidHit())
				{
					vThrow2 = tr.plane.normal * -2.0f * DotProduct(vThrow2, tr.plane.normal) + vThrow2;
					vThrow2 *= GRENADE_COEFFICIENT_OF_RESTITUTION;

					iCollisions++;
					if (iCollisions > 2) // > 2
						break;

					vPos = vStart + vThrow2 * tr.fraction * fStep;
					fTime += (fStep * (1 - tr.fraction));
					if (Interfaces.g_pDebugOverlay->ScreenPosition(vPos, vOutEnd) != 1)
					{
						if (Hacks.LocalWeapon->IsGrenade())
						{
							if (iCollisions < 2)
								Draw3DBox(vPos, Vector(-2, -2, -2), Vector(2, 2, 2), 0, 0, 255, 255);
							else
								Draw3DBox(vPos, Vector(-5, -5, -5), Vector(5, 5, 5), 255, 0, 0, 255); //<---- Boom here
						}
					}
				}


				if (iCollisions <= 2)
				{
					if (Hacks.LocalWeapon->IsGrenade())
					{
						if (Interfaces.g_pDebugOverlay->ScreenPosition(vStart, vOutStart) != 1, Interfaces.g_pDebugOverlay->ScreenPosition(vPos, vOutEnd) != 1)
						{
							DrawLine(vOutStart.x, vOutStart.y, vOutEnd.x, vOutEnd.y, CColor(255, 255, 255, 255));
						}
					}
				}


				vStart = vPos;
				vThrow2.z -= fGravity * tr.fraction * fStep;
			}
		}
		else {

		}
	}
	static void DiLight()
	{
		if (Hacks.LocalWeapon->IsGrenade())
		{
			if (Interfaces.pEngine->IsInGame() && Interfaces.pEngine->IsConnected() && Hacks.LocalPlayer->isAlive())
			{
				const float TIMEALIVE = 5.f;

				const float GRENADE_COEFFICIENT_OF_RESTITUTION = 0.4f;



				float fStep = 0.1f;

				float fGravity = 800.0f / 8.f;



				Vector vPos, vThrow, vThrow2;

				Vector vStart;



				int iCollisions = 0;



				Vector vViewAngles;

				Interfaces.pEngine->GetViewAngles(vViewAngles);



				vThrow[0] = vViewAngles[0];

				vThrow[1] = vViewAngles[1];

				vThrow[2] = vViewAngles[2];



				if (vThrow[0] < 0)

					vThrow[0] = -10 + vThrow[0] * ((90 - 10) / 90.0);

				else

					vThrow[0] = -10 + vThrow[0] * ((90 + 10) / 90.0);



				float fVel = (90 - vThrow[0]) * 4;

				if (fVel > 500)

					fVel = 500;



				Misc::AngleVectors2(vThrow, vThrow2);



				Vector vEye = Hacks.LocalPlayer->GetEyePosition();

				vStart[0] = vEye[0] + vThrow2[0] * 16;

				vStart[1] = vEye[1] + vThrow2[1] * 16;

				vStart[2] = vEye[2] + vThrow2[2] * 16;



				vThrow2[0] = (vThrow2[0] * fVel) + Hacks.LocalPlayer->GetVecVelocity()[0];

				vThrow2[1] = (vThrow2[1] * fVel) + Hacks.LocalPlayer->GetVecVelocity()[1];

				vThrow2[2] = (vThrow2[2] * fVel) + Hacks.LocalPlayer->GetVecVelocity()[2];	// casualhacker for da magic calc help



				for (float fTime = 0.0f; fTime < TIMEALIVE; fTime += fStep)

				{

					vPos = vStart + vThrow2 * fStep;



					Ray_t ray;

					trace_t tr;

					CTraceFilter loc;

					loc.pSkip = Hacks.LocalPlayer;



					ray.Init(vStart, vPos);

					Interfaces.pTrace->TraceRay(ray, MASK_SOLID, &loc, &tr);



					if (tr.DidHit())

					{

						float anglez = DotProduct(Vector(0, 0, 1), tr.plane.normal);

						float invanglez = DotProduct(Vector(0, 0, -1), tr.plane.normal);

						float angley = DotProduct(Vector(0, 1, 0), tr.plane.normal);

						float invangley = DotProduct(Vector(0, -1, 0), tr.plane.normal);

						float anglex = DotProduct(Vector(1, 0, 0), tr.plane.normal);

						float invanglex = DotProduct(Vector(-1, 0, 0), tr.plane.normal);

						float scale = tr.endpos.DistTo(Hacks.LocalPlayer->GetAbsOrigin()) / 60;

						Color color = Color(0, 0, 255);

						if (anglez > 0.5)

						{

							tr.endpos.z += 1;

							Vector startPos = tr.endpos + Vector(-scale, 0, 0);

							Vector endPos = tr.endpos + Vector(scale, 0, 0);

							Vector outStart, outEnd;

							if (g_Math.WorldToScreen(startPos, outStart) && g_Math.WorldToScreen(endPos, outEnd))

								DrawLine(outStart.x, outStart.y, outEnd.x, outEnd.y, CColor(255, 255, 255, 255));



							startPos = tr.endpos + Vector(0, -scale, 0);

							endPos = tr.endpos + Vector(0, scale, 0);

							if (g_Math.WorldToScreen(startPos, outStart) && g_Math.WorldToScreen(endPos, outEnd))

								DrawLine(outStart.x, outStart.y, outEnd.x, outEnd.y, CColor(255, 255, 255, 255));

						}

						else if (invanglez > 0.5)

						{

							tr.endpos.z += 1;

							Vector startPos = tr.endpos + Vector(-scale, 0, 0);

							Vector endPos = tr.endpos + Vector(scale, 0, 0);

							Vector outStart, outEnd;

							if (g_Math.WorldToScreen(startPos, outStart) && g_Math.WorldToScreen(endPos, outEnd))

								DrawLine(outStart.x, outStart.y, outEnd.x, outEnd.y, CColor(255, 255, 255, 255));



							startPos = tr.endpos + Vector(0, -scale, 0);

							endPos = tr.endpos + Vector(0, scale, 0);

							if (g_Math.WorldToScreen(startPos, outStart) && g_Math.WorldToScreen(endPos, outEnd))

								DrawLine(outStart.x, outStart.y, outEnd.x, outEnd.y, CColor(255, 255, 255, 255));

						}

						else if (angley > 0.5)

						{

							tr.endpos.y += 1;

							Vector startPos = tr.endpos + Vector(0, 0, -scale);

							Vector endPos = tr.endpos + Vector(0, 0, scale);

							Vector outStart, outEnd;

							if (g_Math.WorldToScreen(startPos, outStart) && g_Math.WorldToScreen(endPos, outEnd))

								DrawLine(outStart.x, outStart.y, outEnd.x, outEnd.y, CColor(255, 255, 255, 255));



							startPos = tr.endpos + Vector(-scale, 0, 0);

							endPos = tr.endpos + Vector(scale, 0, 0);

							if (g_Math.WorldToScreen(startPos, outStart) && g_Math.WorldToScreen(endPos, outEnd))

								DrawLine(outStart.x, outStart.y, outEnd.x, outEnd.y, CColor(255, 255, 255, 255));

						}

						else if (invangley > 0.5)

						{

							tr.endpos.y += 1;

							Vector startPos = tr.endpos + Vector(0, 0, -scale);

							Vector endPos = tr.endpos + Vector(0, 0, scale);

							Vector outStart, outEnd;

							if (g_Math.WorldToScreen(startPos, outStart) && g_Math.WorldToScreen(endPos, outEnd))

								DrawLine(outStart.x, outStart.y, outEnd.x, outEnd.y, CColor(255, 255, 255, 255));



							startPos = tr.endpos + Vector(-scale, 0, 0);

							endPos = tr.endpos + Vector(scale, 0, 0);

							if (g_Math.WorldToScreen(startPos, outStart) && g_Math.WorldToScreen(endPos, outEnd))

								DrawLine(outStart.x, outStart.y, outEnd.x, outEnd.y, CColor(255, 255, 255, 255));

						}

						else if (anglex > 0.5)

						{

							tr.endpos.x += 1;

							Vector startPos = tr.endpos + Vector(0, -scale, 0);

							Vector endPos = tr.endpos + Vector(0, scale, 0);

							Vector outStart, outEnd;

							if (g_Math.WorldToScreen(startPos, outStart) && g_Math.WorldToScreen(endPos, outEnd))

								DrawLine(outStart.x, outStart.y, outEnd.x, outEnd.y, CColor(255, 255, 255, 255));



							startPos = tr.endpos + Vector(0, 0, -scale);

							endPos = tr.endpos + Vector(0, 0, scale);

							if (g_Math.WorldToScreen(startPos, outStart) && g_Math.WorldToScreen(endPos, outEnd))

								DrawLine(outStart.x, outStart.y, outEnd.x, outEnd.y, CColor(255, 255, 255, 255));

						}

						else if (invanglex > 0.5)

						{

							tr.endpos.x += 1;

							Vector startPos = tr.endpos + Vector(0, -scale, 0);

							Vector endPos = tr.endpos + Vector(0, scale, 0);

							Vector outStart, outEnd;

							if (g_Math.WorldToScreen(startPos, outStart) && g_Math.WorldToScreen(endPos, outEnd))

								DrawLine(outStart.x, outStart.y, outEnd.x, outEnd.y, CColor(255, 255, 255, 255));



							startPos = tr.endpos + Vector(0, 0, -scale);

							endPos = tr.endpos + Vector(0, 0, scale);

							if (g_Math.WorldToScreen(startPos, outStart) && g_Math.WorldToScreen(endPos, outEnd))

								DrawLine(outStart.x, outStart.y, outEnd.x, outEnd.y, CColor(255, 255, 255, 255));

						}



						vThrow2 = tr.plane.normal * -2.0f * g_Math.DotProduct(vThrow2, tr.plane.normal) + vThrow2;

						vThrow2 *= GRENADE_COEFFICIENT_OF_RESTITUTION;



						iCollisions++;

						if (iCollisions > 2)

							break;



						vPos = vStart + vThrow2 * tr.fraction * fStep;

						fTime += (fStep * (1 - tr.fraction));

					}



					Vector vOutStart, vOutEnd;



					if (g_Math.WorldToScreen(vStart, vOutStart), g_Math.WorldToScreen(vPos, vOutEnd))

						DrawLine(vOutStart.x, vOutStart.y, vOutEnd.x, vOutEnd.y, CColor(255, 255, 255, 255));



					vStart = vPos;

					vThrow2.z -= fGravity * tr.fraction * fStep;

				}
			}
		}
	}
	
	static void Watermark()
	{

		float MEMES = 234523452345;
		MEMES = 2345234523462345;
		if (MEMES = 2345234623452345)
			MEMES = 23461234512341234;
		MEMES = 123412351452345;
		MEMES = 2346234623452345;
		if (MEMES = 234523462342346);
		MEMES = 1245124351242314;
		MEMES = 23412342134;
		if (MEMES = 235235424234)
			MEMES = 23523234234;
		MEMES = 1235123412341234;
		MEMES = 2342341235123;
		if (MEMES = 234523452134);
		MEMES = 234543262345;
		MEMES = 234623452346;
		if (MEMES = 2372345234125)
			MEMES = 234523462345;
		MEMES = 234523462345;
		MEMES = 124351234242343;
		if (MEMES = 12341234514325);
		MEMES = 234523462345;
		MEMES = 134252345;
		int W, H, cW, cH;
		Interfaces.pEngine->GetScreenSize(W, H);

		long currentTime_ms = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		static long timeStamp = currentTime_ms;

		timeStamp = currentTime_ms;

		std::string change;

		cW = W / 2;
		cH = H / 2;
			if(Options.Visuals.Esp.EspBox.Out)
		{
		if (Interfaces.pEngine->IsInGame() && Interfaces.pEngine->IsConnected())
		{
			CBaseEntity* pLocalEntity = Hacks.LocalPlayer;
			if (pLocalEntity)
			{
				Vector LocalPos = pLocalEntity->GetEyePosition();
				QAngle ang;
				Interfaces.pEngine->GetViewAngles(ang);
				for (int i = 0; i < Interfaces.pEngine->GetMaxClients(); i++) {
					CBaseEntity* pBaseEntity = Interfaces.pEntList->GetClientEntity(i);

					if (!pBaseEntity)
						continue;
					if (!pBaseEntity->isValidPlayer())
						continue;
					if (pBaseEntity == pLocalEntity)
						continue;

					bool bIsEnemy = pLocalEntity->GetTeam() != pBaseEntity->GetTeam();
					if (pLocalEntity->GetTeam() == pBaseEntity->GetTeam())
						continue;
					//bool isVisibled = G::VisibledPlayers[i]; /*U::IsVisible(LocalPos, pBaseEntity->GetEyePosition(), pLocalEntity, pBaseEntity, Options::Radar::SmokeCheck);*/

					//	if (false)
					//	continue;

					bool viewCheck = false;
					Vector EntityPos = MEME2(pBaseEntity->GetAbsOrigin(), LocalPos, cW - 225, cW - 500, cH, cH + 200, ang.y, 2, &viewCheck);

					//if (false)
					//isVisibled = false;
					//ImU32 clr = (bIsEnemy ? (isVisibled ? Color::LightGreen() : Color::Blue()) : Color::White()).GetU32();
					//	ImU32 clr = /*bIsEnemy ? (isVisibled ? *///Color::White().GetU32();// : Color::Black()) : Color::Blue()).GetU32();

					//if (false)
					//	continue;

					int s = 4;
					Interfaces.pSurface->DrawSetColor(255, 255, 255, 255);
					DrawRect(EntityPos.x - s - 6, EntityPos.y - s - 1, EntityPos.x + s - 3 + 9, EntityPos.y + s - 1 + 2,CColor(255,255,255,255));


				}
			}
		}
	}
		Interfaces.pSurface->DrawT(cW - 955, cH + 16, CColor(255, 255, 255, 255), Hacks.Font_ESP2, false, "floxware");
		if (Interfaces.pEngine->IsInGame() && Interfaces.pEngine->IsConnected() && Hacks.LocalPlayer->isAlive())
		{
			bool sendpacket = Hacks.SendPacket;
			if (Options.Visuals.Esp.EspBox.Speclist)
			{
				int ix = 1;
				for (int i = 0; i <= Interfaces.pGlobalVars->maxClients; i++)
				{
					CBaseEntity* Entity = Interfaces.pEntList->GetClientEntity(i);

					if (Entity &&  Entity != Hacks.LocalPlayer)
					{
						if (Entity->isAlive())
							continue;

						player_info_t pinfo;
						if (Interfaces.pEngine->GetPlayerInfo(i, &pinfo))
						{
							HANDLE obs = Entity->GetOBS();

							if (obs)
							{
								CBaseEntity *pTarget = Interfaces.pEntList->GetClientEntityFromHandle(obs);
								player_info_t pinfo2;

								if (pTarget)
								{
									if (Interfaces.pEngine->GetPlayerInfo(pTarget->GetIndex(), &pinfo2))
									{

										if (pTarget->GetIndex() == Hacks.LocalPlayer->GetIndex() && pTarget->isAlive())
										{

											//		char buf[255]; sprintf_s(buf, "%s => %s", pinfo.name, pinfo2.name);
											char buf[255]; sprintf_s(buf, "%s is Watching You ", pinfo.name);
											//D::DrawString(F::ESP, 16, 55 + (15 * ix), Color::Red(), FONT_LEFT, charenc("%s"), buf);
											Interfaces.pSurface->DrawT(cW - 450, cH + (15 * ix), CColor(112, 230, 20, 255), Hacks.Font_ESP, false, "%s", buf);

											ix++;
										}
									}
								}
							}
						}
					}
				}
			}
			if (Options.Visuals.Infobox.bInfoLby)
			{

				if (!(Hacks.CurrentCmd->viewangles.y - Hacks.LocalPlayer->pelvisangs() >= -35 && Hacks.CurrentCmd->viewangles.y - Hacks.LocalPlayer->pelvisangs() <= 35))
				{
					Interfaces.pSurface->DrawT(cW - 955, cH + 450, CColor(112, 230, 20, 255), Hacks.Font_LBY, false, "LBY");

				}
				else
				{
					Interfaces.pSurface->DrawT(cW - 955, cH + 450, CColor(255, 0, 0, 255), Hacks.Font_LBY, false, "LBY");

				}
			}

			if (Options.Visuals.Infobox.Inaccuracy)
			{
				int acc = Hacks.LocalPlayer->GetActiveBaseCombatWeapon()->GetInaccuracy() * 1000;
				Interfaces.pSurface->DrawT(cW - 40, cH + 106, CColor(255, 255, 255, 255), Hacks.Font_ESP2, false, "Inaccuracy : %i", acc);

			}
			if (Options.Visuals.Infobox.Spreadcheck)
			{
				int spre = Hacks.LocalPlayer->GetActiveBaseCombatWeapon()->GetSpread();
				Interfaces.pSurface->DrawT(cW - 40, cH + 146, CColor(255, 255, 255, 255), Hacks.Font_ESP2, false, "Spread : %i", spre);

			}

			/*if (Options.Visuals.Infobox.InfoECHO)
			{
				IGameEvent *event;
				if (strcmp(event->GetName(), "round_start") == 0)
				{
						if (Interfaces.pEngine->IsInGame() && Interfaces.pEngine->IsConnected() && Hacks.LocalPlayer->isAlive())
						{
							if (Interfaces.pEngine->GetPlayerForUserID(event->GetInt("attacker")) == Interfaces.pEngine->GetLocalPlayer())
							{
								Interfaces.pSurface->DrawT(cW - 955, cH - 60, CColor(255, 255, 255, 255), Hacks.Font_ESP2, false, "[FLOXWARE.TK] Player got hit");
								Interfaces.pEngine->ClientCmd_Unrestricted("echo [FLOXWARE.TK] Player got hit", 0);
								Sleep(0);

							}
							else
							{
								Interfaces.pSurface->DrawT(cW - 955, cH - 60, CColor(255, 255, 255, 255), Hacks.Font_ESP2, false, "[FLOXWARE.TK] Other player got hit by other player");

								Interfaces.pEngine->ClientCmd_Unrestricted("echo [FLOXWARE.TK] Other player hit other player", 0);
								Sleep(0);
							}

						}
					}
			}*/
			if (Options.Visuals.Infobox.LC)
			{
				if (sendpacket)
				{
					Interfaces.pSurface->DrawT(cW - 955, cH + 40, CColor(112, 230, 20, 255), Hacks.Font_LBY, false, "LC");
				}
				else
				{
					Interfaces.pSurface->DrawT(cW - 955, cH + 40, CColor(255, 0, 0, 255), Hacks.Font_LBY, false, "LC");
				}
			}

			if (Options.Ragebot.AntiAim.Move.lockside3)
			{


				if (GetKeyState(0x27)) // a
				{
					Interfaces.pSurface->DrawT(cW - 955, cH + 73, CColor(112, 230, 20, 255), Hacks.Font_LBY, false, "LS");
				}
				else if (GetKeyState(0x25)) //d 		
				{
					Interfaces.pSurface->DrawT(cW - 955, cH + 73, CColor(112, 230, 20, 255), Hacks.Font_LBY, false, "LS");
				}

				if (Options.Visuals.Esp.EspBox.locksidearrows)
				{
						float damage = 0.f;
						if (Autowall::CanWallbang(damage))
						{
							Interfaces.pSurface->DrawT(cW - 450, cH + 15, CColor(255, 255, 255, 255), Hacks.Font_ESP2, false, "Autowall :  - %.1f HP", damage);

						}

				}

			}

			/*if (Options.Visuals.Infobox.bInfoAngles)
			{
				int anglereal = Global::curReal;
				int anglefake = Global::curFake;
				int lby = Hacks.LocalPlayer->pelvisangs();

				Interfaces.pSurface->DrawT(cW - 955, cH + 73, CColor(0, 100, 0, 255), Hacks.Font_Watermark, false, "LBY : %s", lby);
				Interfaces.pSurface->DrawT(cW - 955, cH + 106, CColor(0, 100, 0, 255), Hacks.Font_Watermark, false, "REAL : %s", anglereal);
				Interfaces.pSurface->DrawT(cW - 955, cH + 139, CColor(0, 100, 0, 255), Hacks.Font_Watermark, false, "FAKE : %s", anglefake);

			}
			*/
		}
	}


	static void ThirdPerson()
	{
		static bool bSpoofed = false;
		static bool bThirdperson = false;
		static bool bKeyFix = false;


		if( !bSpoofed )
		{
			ConVar* sv_cheats = Interfaces.g_ICVars->FindVar("sv_cheats");
			SpoofedConvar* sv_cheats_spoofed = new SpoofedConvar(sv_cheats);
			sv_cheats_spoofed->SetInt(1);
			bSpoofed = true;
		}
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

		if(bSpoofed)
		{
			if( !bThirdperson && Options.Visuals.VisualsMisc.iThirdPerson && enabledtp && Hacks.LocalPlayer->GetHealth() > 0 )
			{
					Interfaces.pEngine->ClientCmd_Unrestricted("thirdperson", 0);
					bThirdperson = true;

					CBaseEntity* ThirdPerson = Hacks.LocalPlayer;
					Vector *ViewAngles = ThirdPerson->GetEyeAnglesPointer();
					Vector BufferViewAngles = Hacks.CurrentCmd->viewangles;
					if (BufferViewAngles.x > 89)
						BufferViewAngles.x = 89;
					if (BufferViewAngles.x < -89)
						BufferViewAngles.x = -89;
					*ViewAngles = BufferViewAngles;


			}
			else if( bThirdperson && !Options.Visuals.VisualsMisc.iThirdPerson)
			{
				Interfaces.pEngine->ClientCmd_Unrestricted( "firstperson", 0 );
				bThirdperson = false;
			}
			if( Hacks.LocalPlayer->GetHealth() <= 0 )
			{
				Interfaces.pEngine->ClientCmd_Unrestricted( "firstperson", 0 );
				bThirdperson = false;
			}
		}
	}


	static void Crosshair()
	{
		int W, H, cW, cH;
		Interfaces.pEngine->GetScreenSize(W, H);

		cW = W / 2;
		cH = H / 2;

		int dX = W / Hacks.FOV;
		int dY = H / Hacks.FOV;
		int drX, drY;

		if (Options.Visuals.VisualsMisc.bRecoilCrosshair)
		{
			if (Options.Visuals.VisualsMisc.bNoVisualRecoil)
			{
				drX = cW - (int)(dX * (Hacks.LocalPlayer->GetPunchAngle().y * 2));
				drY = cH + (int)(dY * (Hacks.LocalPlayer->GetPunchAngle().x * 2));
			}
			else
			{
				drX = cW - (int)(dX * (((Hacks.LocalPlayer->GetPunchAngle().y * 2.f))/* + Hacks.LocalPlayer->GetPunchAngle().y*/));
				drY = cH + (int)(dY * (((Hacks.LocalPlayer->GetPunchAngle().x * 2.f))/* + Hacks.LocalPlayer->GetPunchAngle().x*/));
			}
		}
		else
		{
			drX = cW;
			drY = cH;
		}

		if ((Options.Visuals.VisualsMisc.bRecoilCrosshair) && (!Options.Visuals.VisualsMisc.bNormalCrosshair))
		{
			Interfaces.pSurface->DrawSetColor(244, 244, 244, 255);
			Interfaces.pSurface->DrawFilledRect(drX - 6, drY - 1, drX - 3 + 9, drY - 1 + 2);
			Interfaces.pSurface->DrawFilledRect(drX - 1, drY - 6, drX - 1 + 2, drY - 3 + 9);
		}
	/*	if (Options.Visuals.Infobox.InfoECHO)
		{
			float radius = ((Hacks.LocalPlayer->GetActiveBaseCombatWeapon()->GetSpread()) + (Hacks.LocalPlayer->GetActiveBaseCombatWeapon()->GetInaccuracy())) * 500;

			Interfaces.pSurface->DrawFilledCircle(drX, drY,radius, 0, 0, 0, 255);
			
		}*/
		if (Options.Visuals.VisualsMisc.bCrosshair && (Options.Visuals.VisualsMisc.bNormalCrosshair))
		{
			Interfaces.pSurface->DrawSetColor(244, 244, 244, 255);
			Interfaces.pSurface->DrawFilledRect(drX - 6, drY - 1, drX - 3 + 9, drY - 1 + 2);
			Interfaces.pSurface->DrawFilledRect(drX - 1, drY - 6, drX - 1 + 2, drY - 3 + 9);
		}
		
	}
	
	static void Hitmarker()
	{
		if (Global::hitmarkerAlpha < 0.f)
			Global::hitmarkerAlpha = 0.f;
		else if (Global::hitmarkerAlpha > 0.f)
			Global::hitmarkerAlpha -= 0.01f;

		int W, H;
		Interfaces.pEngine->GetScreenSize(W, H);

		if (Global::hitmarkerAlpha > 0.f)
		{
			DrawLine(W / 2 - 10, H / 2 - 10, W / 2 - 5, H / 2 - 5, CColor(240, 240, 240, (Global::hitmarkerAlpha * 255.f)));
			DrawLine(W / 2 - 10, H / 2 + 10, W / 2 - 5, H / 2 + 5, CColor(240, 240, 240, (Global::hitmarkerAlpha * 255.f)));
			DrawLine(W / 2 + 10, H / 2 - 10, W / 2 + 5, H / 2 - 5, CColor(240, 240, 240, (Global::hitmarkerAlpha * 255.f)));
			DrawLine(W / 2 + 10, H / 2 + 10, W / 2 + 5, H / 2 + 5, CColor(240, 240, 240, (Global::hitmarkerAlpha * 255.f)));

		}
	}
	static void DrawHeaddot(CBaseEntity* player)

	{

		Vector head2D;

		Vector head3D = player->GetBonePos(6);

		if (Interfaces.g_pDebugOverlay->ScreenPosition(Vector(head3D.x, head3D.y, head3D.z), head2D))

			return;



		bool bIsVisible = false;


		DrawRect(head2D.x, head2D.y, head2D.x, head2D.y, CColor(255, 255, 255, 255));

	}
	static void Info()
	{
		int W, H, cW, cH;
		Interfaces.pEngine->GetScreenSize(W, H);

		long currentTime_ms = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		static long timeStamp = currentTime_ms;

		timeStamp = currentTime_ms;

		std::string change;

		cW = W / 2;
		cH = H / 2;

		if (Global::Infoalpha < 0.f)
			Global::Infoalpha = 0.f;
		else if (Global::Infoalpha > 0.f)
			Global::Infoalpha -= 0.01f;

		int meme = 0;
		float top2 = 60;

		if (Global::Infoalpha > 0.f)
		{
			Interfaces.pSurface->DrawT(cW - 955, cH - 60 - top2++, CColor(255, 255, 255, (Global::Infoalpha * 255)), Hacks.Font_ESP2, false, "(floxware) player got hit");
		}

		if (Global::Infoalpha2 < 0.f)
			Global::Infoalpha2 = 0.f;
		else if (Global::Infoalpha2 > 0.f)
			Global::Infoalpha2 -= 0.01f;


		if (Global::Infoalpha2 > 0.f)
		{
			Interfaces.pSurface->DrawT(cW - 955, cH - 60 - top2++, CColor(255, 255, 255, (Global::Infoalpha2 * 255)), Hacks.Font_ESP2, false, "(floxware) other player hit other player");

		}
		if (Global::Infoalpha3 > 0.f)
		{
			Interfaces.pSurface->DrawT(cW - 955, cH - 60 - top2++, CColor(255, 255, 255, (Global::Infoalpha3 * 255)), Hacks.Font_ESP2, false, "(floxware) LBY update [NEW TICK]");

		}
	}
	static void BulletTrace(CBaseEntity* entity)
	{
		Vector src3D, dst3D, forward, src, dst;
		trace_t tr;
		Ray_t ray;
		CTraceFilter filter;
		Misc::AngleVectors(entity->GetEyeAngles(), &forward);
		filter.pSkip = entity;
		src3D = entity->GetBonePos(6) - Vector(0, 0, 0);
		dst3D = src3D + (forward * 500);

		ray.Init(src3D, dst3D);

		Interfaces.pTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

		if (g_Math.WorldToScreen(src3D, src) || g_Math.WorldToScreen(tr.endpos, dst))
			return;
		Interfaces.pSurface->DrawSetColor(200, 50, 50, 150);
		Interfaces.pSurface->DrawLine(src.x, src.y, dst.x, dst.y);
		Interfaces.pSurface->DrawFilledRect(dst.x - 3, dst.y - 3, 6, 6);

	}



	static void DrawInfo(RECT rect, CBaseEntity* pPlayer)
	{
		float bar = 0;
		float top = 1.4;
		float top2 = 1.4;

		float bot = 0.8;
		float right = 0;
		float left = 0;
		float left2 = 40;
		player_info_t info;
		static class Text
		{
		public:
			string text;
			int side;
			int Font;

			Text(string text, int side, int Font) : text(text), side(side), Font(Font)
			{
			}
		};
		std::vector< Text > texts;
		int middle = ((rect.right - rect.left) / 2) + rect.left;
		int middle2 = ((rect.right - rect.left) / 2) + rect.left + left2;


		if (pPlayer->IsHostage())
		{
			if (Options.Visuals.Esp.EspFilter.bHostage)
				Interfaces.pSurface->DrawT(middle, rect.top - top++ * 9, CColor(244, 244, 244, 255), Hacks.Font_ESP, true, "Hostage");
		}
		else if (Interfaces.pEngine->GetPlayerInfo(pPlayer->GetIndex(), &info))
		{
			
			if (Options.Visuals.Esp.EspBox.bHealthbar)
			{
				bar++;
				float HealthValue = pPlayer->GetHealth();
				int iHealthValue = HealthValue;
				int Red = 255 - (HealthValue * 2.00);
				int Green = HealthValue * 2.00;
				float flBoxes = std::ceil(pPlayer->GetHealth() / 10.f);

				float height = (rect.bottom - rect.top) * (HealthValue / 100);
				float height2 = (rect.bottom - rect.top) * (100 / 100); // used for the styles like healthbar lines
				float flHeight = height2 / 10.f;

				DrawRect(rect.left - 5, rect.top - 1, rect.left - 1, rect.bottom + 1, CColor(0, 0, 0, 150));
				DrawRect(rect.left - 4, rect.bottom - height, rect.left - 2, rect.bottom, CColor(Red, Green, 0, 255));

			}
			/*if (Options.Visuals.Esp.EspBox.Bullet)
			{
				Vector src3D, dst3D, forward, src, dst;
				trace_t tr;
				Ray_t ray;
				CTraceFilter filter;

				Misc::AngleVectors(pPlayer->GetEyeAngles(), &forward);
				filter.pSkip = pPlayer;
				src3D = pPlayer->GetBonePos(6) - Vector(0, 0, 0);
				dst3D = src3D + (forward * 800);

				ray.Init(src3D, dst3D);

				Interfaces.pTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

				if (!g_Math.WorldToScreen(src3D, src) || !g_Math.WorldToScreen(tr.endpos, dst))
					return;
				//Interfaces.g_pDebugOverlay->AddLineOverlay(src, dst,255, 255, 255, true, -1.f);

				DrawLine(src.x, src.y, dst.x, dst.y, CColor(255,255,255,255));
				
			
			}*/
			if (Options.Visuals.Esp.EspBox.bArmorBar)
			{
				CBaseCombatWeapon* weapon = pPlayer->GetActiveBaseCombatWeapon();
				float WR = Options.Visuals.Esp.EspBox.MEME1[0] * 255;
				float WG = Options.Visuals.Esp.EspBox.MEME1[1] * 255;
				float WB = Options.Visuals.Esp.EspBox.MEME1[2] * 255;
					if (weapon)
					{
						if (weapon->ammogun69())
						{
							CBaseCombatWeapon* weapon = pPlayer->GetActiveBaseCombatWeapon();
							float ArmorValue = weapon->ammo();
							int red = WR;
							int blue = WB;
							int green = WG;


							float height = (rect.right - rect.left) * (ArmorValue / 10);


							DrawRect(rect.left - 1, rect.bottom + 1, rect.right + 1, rect.bottom + 5, CColor(10, 10, 10, 165));
							DrawRect(rect.left, rect.bottom + 2, rect.left + height, rect.bottom + 4, CColor(red, green, blue, 255));


						}

						else if (weapon->ammogunn())
						{
							CBaseCombatWeapon* weapon = pPlayer->GetActiveBaseCombatWeapon();
							float ArmorValue = weapon->ammo();
							int iArmorValue = ArmorValue;
							int red = WR;
							int blue = WB;
							int green = WG;
							float height = (rect.right - rect.left) * (ArmorValue / 30);


							DrawRect(rect.left - 1, rect.bottom + 1, rect.right + 1, rect.bottom + 5, CColor(10, 10, 10, 165));
							DrawRect(rect.left, rect.bottom + 2, rect.left + height, rect.bottom + 4, CColor(red, green, blue, 255));

						}
						else if (weapon->ammoogun())
						{
							CBaseCombatWeapon* weapon = pPlayer->GetActiveBaseCombatWeapon();
							float ArmorValue = weapon->ammo();
							int iArmorValue = ArmorValue;
							int red = WR;
							int blue = WB;
							int green = WG;

							float height = (rect.right - rect.left) * (ArmorValue / 25);


							DrawRect(rect.left - 1, rect.bottom + 1, rect.right + 1, rect.bottom + 5, CColor(10, 10, 10, 165));
							DrawRect(rect.left, rect.bottom + 2, rect.left + height, rect.bottom + 4, CColor(red, green, blue, 255));

						}
						else if (weapon->one6999())
						{
							CBaseCombatWeapon* weapon = pPlayer->GetActiveBaseCombatWeapon();
							float ArmorValue = weapon->ammo();
							int iArmorValue = ArmorValue;
							int red = WR;
							int blue = WB;
							int green = WG;

							float height = (rect.right - rect.left) * (ArmorValue / 35);


							DrawRect(rect.left - 1, rect.bottom + 1, rect.right + 1, rect.bottom + 5, CColor(10, 10, 10, 165));
							DrawRect(rect.left, rect.bottom + 2, rect.left + height, rect.bottom + 4, CColor(red, green, blue, 255));

						}
						else if (weapon->one555595())
						{
							CBaseCombatWeapon* weapon = pPlayer->GetActiveBaseCombatWeapon();
							float ArmorValue = weapon->ammo();
							int iArmorValue = ArmorValue;
							int red = WR;
							int blue = WB;
							int green = WG;

							float height = (rect.right - rect.left) * (ArmorValue / 20);


							DrawRect(rect.left - 1, rect.bottom + 1, rect.right + 1, rect.bottom + 5, CColor(10, 10, 10, 165));
							DrawRect(rect.left, rect.bottom + 2, rect.left + height, rect.bottom + 4, CColor(red, green, blue, 255));

						}
						else if (weapon->one1337())
						{
							CBaseCombatWeapon* weapon = pPlayer->GetActiveBaseCombatWeapon();
							float ArmorValue = weapon->ammo();
							int iArmorValue = ArmorValue;
							int red = WR;
							int blue = WB;
							int green = WG;

							float height = (rect.right - rect.left) * (ArmorValue / 12);


							DrawRect(rect.left - 1, rect.bottom + 1, rect.right + 1, rect.bottom + 5, CColor(10, 10, 10, 165));
							DrawRect(rect.left, rect.bottom + 2, rect.left + height, rect.bottom + 4, CColor(red, green, blue, 255));

						}
						else if (weapon->one13371())
						{
							CBaseCombatWeapon* weapon = pPlayer->GetActiveBaseCombatWeapon();
							float ArmorValue = weapon->ammo();
							int iArmorValue = ArmorValue;
							int red = WR;
							int blue = WB;
							int green = WG;

							float height = (rect.right - rect.left) * (ArmorValue / 8);


							DrawRect(rect.left - 1, rect.bottom + 1, rect.right + 1, rect.bottom + 5, CColor(10, 10, 10, 165));
							DrawRect(rect.left, rect.bottom + 2, rect.left + height, rect.bottom + 4, CColor(red, green, blue, 255));

						}
						else if (weapon->one133711())
						{
							CBaseCombatWeapon* weapon = pPlayer->GetActiveBaseCombatWeapon();
							float ArmorValue = weapon->ammo();
							int iArmorValue = ArmorValue;
							int red = WR;
							int blue = WB;
							int green = WG;

							float height = (rect.right - rect.left) * (ArmorValue / 7);


							DrawRect(rect.left - 1, rect.bottom + 1, rect.right + 1, rect.bottom + 5, CColor(10, 10, 10, 165));
							DrawRect(rect.left, rect.bottom + 2, rect.left + height, rect.bottom + 4, CColor(red, green, blue, 255));

						}

						else if (weapon->one12())
						{
							CBaseCombatWeapon* weapon = pPlayer->GetActiveBaseCombatWeapon();
							float ArmorValue = weapon->ammo();
							int iArmorValue = ArmorValue;
							int red = WR;
							int blue = WB;
							int green = WG;

							float height = (rect.right - rect.left) * (ArmorValue / 5);


							DrawRect(rect.left - 1, rect.bottom + 1, rect.right + 1, rect.bottom + 5, CColor(10, 10, 10, 165));
							DrawRect(rect.left, rect.bottom + 2, rect.left + height, rect.bottom + 4, CColor(red, green, blue, 255));

						}
						else if (weapon->one123())
						{
							CBaseCombatWeapon* weapon = pPlayer->GetActiveBaseCombatWeapon();
							float ArmorValue = weapon->ammo();
							int iArmorValue = ArmorValue;
							int red = WR;
							int blue = WB;
							int green = WG;

							float height = (rect.right - rect.left) * (ArmorValue / 13);


							DrawRect(rect.left - 1, rect.bottom + 1, rect.right + 1, rect.bottom + 5, CColor(10, 10, 10, 165));
							DrawRect(rect.left, rect.bottom + 2, rect.left + height, rect.bottom + 4, CColor(red, green, blue, 255));

						}

						else if (weapon->one1234())
						{
							CBaseCombatWeapon* weapon = pPlayer->GetActiveBaseCombatWeapon();
							float ArmorValue = weapon->ammo();
							int iArmorValue = ArmorValue;
							int red = WR;
							int blue = WB;
							int green = WG;

							float height = (rect.right - rect.left) * (ArmorValue / 64);


							DrawRect(rect.left - 1, rect.bottom + 1, rect.right + 1, rect.bottom + 5, CColor(10, 10, 10, 165));
							DrawRect(rect.left, rect.bottom + 2, rect.left + height, rect.bottom + 4, CColor(red, green, blue, 255));

						}

						else if (weapon->one1235())
						{
							CBaseCombatWeapon* weapon = pPlayer->GetActiveBaseCombatWeapon();
							float ArmorValue = weapon->ammo();
							int iArmorValue = ArmorValue;
							int red = WR;
							int blue = WB;
							int green = WG;

							float height = (rect.right - rect.left) * (ArmorValue / 50);


							DrawRect(rect.left - 1, rect.bottom + 1, rect.right + 1, rect.bottom + 5, CColor(10, 10, 10, 165));
							DrawRect(rect.left, rect.bottom + 2, rect.left + height, rect.bottom + 4, CColor(red, green, blue, 255));

						}
						else if (weapon->GUN2())
						{
							CBaseCombatWeapon* weapon = pPlayer->GetActiveBaseCombatWeapon();
							float ArmorValue = weapon->ammo();
							int iArmorValue = ArmorValue;
							int red = WR;
							int blue = WB;
							int green = WG;

							float height = (rect.right - rect.left) * (ArmorValue / 100);


							DrawRect(rect.left - 1, rect.bottom + 1, rect.right + 1, rect.bottom + 5, CColor(10, 10, 10, 165));
							DrawRect(rect.left, rect.bottom + 2, rect.left + height, rect.bottom + 4, CColor(red, green, blue, 255));

						}
						else if (weapon->GUN3())
						{
							CBaseCombatWeapon* weapon = pPlayer->GetActiveBaseCombatWeapon();
							float ArmorValue = weapon->ammo();
							int iArmorValue = ArmorValue;
							int red = WR;
							int blue = WB;
							int green = WG;

							float height = (rect.right - rect.left) * (ArmorValue / 150);


							DrawRect(rect.left - 1, rect.bottom + 1, rect.right + 1, rect.bottom + 5, CColor(10, 10, 10, 165));
							DrawRect(rect.left, rect.bottom + 2, rect.left + height, rect.bottom + 4, CColor(red, green, blue, 255));

						}
					}
			}

			if (Options.Visuals.Esp.EspMisc.bName)
				Interfaces.pSurface->DrawT(middle, rect.top - top++ * 9, CColor(244, 244, 244, 255), Hacks.Font_ESP2, true, "%s", pPlayer->GetName().c_str());
			for (int i = 0; i < Interfaces.pEntList->GetHighestEntityIndex(); i++)
			{
				if (pPlayer && pPlayer != Hacks.LocalPlayer && !pPlayer->IsDormant())
				{
					if (Interfaces.pEngine->GetPlayerInfo(i, &info) && pPlayer->isAlive())
					{
						if (Options.Visuals.Esp.EspBox.BackTrackTicks)
						{
							if (Hacks.LocalPlayer->isAlive())
							{
								for (int t = 0; t < 13; ++t)
								{
									Vector screenbacktrack[64][12];

									if (headPositions[i][t].simtime && headPositions[i][t].simtime + 1 > Hacks.LocalPlayer->GetSimulationTime())
									{
										if (g_Math.WorldToScreen(headPositions[i][t].hitboxPos, screenbacktrack[i][t]))
										{
											DrawOutlinedRect(screenbacktrack[i][t].x, screenbacktrack[i][t].y, screenbacktrack[i][t].x + 2, screenbacktrack[i][t].y + 2, CColor(255, 255, 255, 255));

										}
									}
								}
							}
						}
					}
				}
			}
			
			if (Options.Visuals.Esp.EspMisc.bIsFlashed)
			{
				if (Options.Visuals.Esp.EspFilter.bEnemyOnly)
				{
					//nothing here
				}
				else
				{
				if (pPlayer->GetTeam() == 2)
				{
					Interfaces.pSurface->DrawT(middle + 30, rect.top - top2++ * 9 + 10, CColor(255, 255, 255, 255), Hacks.Font_ESP, true, "T");
				}
				else
				{
					Interfaces.pSurface->DrawT(middle + 30, rect.top - top2++ * 9 + 10, CColor(255, 255, 255, 255), Hacks.Font_ESP, true, "CT");
				}
			}

			}

			if (Options.Visuals.Esp.EspBox.bPitch)
			{
				if (Interfaces.pEngine->IsInGame() && Interfaces.pEngine->IsConnected() && Hacks.LocalPlayer->isAlive())
				{
					if (Options.Ragebot.HvH.bResolver)
					{
						if (pPlayer->GetFlags() & FL_ONGROUND && pPlayer->GetVecVelocity().Length2D() <= 35) {
							Interfaces.pSurface->DrawT(middle + 30, rect.top - top2++ * 9 + 10, CColor(173, 255, 47, 255), Hacks.Font_ESP, true, "STAND");
						}
						else
							Interfaces.pSurface->DrawT(middle + 30, rect.top - top2++ * 9 + 10, CColor(173, 255, 47, 255), Hacks.Font_ESP, true, "MOVE");
					}
				}
			}
			if (Options.Visuals.Esp.EspMisc.bIsFlashed)
			{ 
				if (pPlayer->m_bIsScoped())
				{
					Interfaces.pSurface->DrawT(middle + 30, rect.top - top2++ * 9 + 10, CColor(255, 255, 255, 255), Hacks.Font_ESP, true, "ZOOM");
				}

			}

			if (Options.Visuals.Esp.EspMisc.bIsFlashed)
			{

			if (pPlayer->HasHelmet())
			{
				Interfaces.pSurface->DrawT(middle + 30, rect.top - top2++ * 9 + 10, CColor(255, 255, 255, 255), Hacks.Font_ESP, true, "HK");
			}
			if (Options.Visuals.Esp.EspMisc.Money)
			{
				int money = pPlayer->Money();
				Interfaces.pSurface->DrawT(middle + 30, rect.top - top2++ * 9 + 10, CColor(173, 255, 47, 255), Hacks.Font_ESP, true, "$%i", money);

			}
			}
			/*if (Options.Visuals.Esp.EspBox.bYaw)
			{
				float Yaw = pPlayer->GetEyeAngles().y;

				if (!(Yaw - pPlayer->pelvisangs() >= -35 && Yaw - pPlayer->pelvisangs() <= 35))
				{
					Interfaces.pSurface->DrawT(middle - 9, rect.bottom + bot++ * 9, CColor(255, 255, 255, 255), Hacks.Font_ESP, false, "FAKE");

				}
				else
				{
					Interfaces.pSurface->DrawT(middle - 9, rect.bottom + bot++ * 9, CColor(255, 255, 255, 255), Hacks.Font_ESP, false, "");
				}
			}*/


			CBaseCombatWeapon* weapon = pPlayer->GetActiveBaseCombatWeapon();
			if (weapon)
			{
				if (Options.Visuals.Esp.EspMisc.bWeapons && weapon)
				{
					std::string weaponname = weapon->GetGunName();
					std::string str = "";

						//char* weaponchar = weapon->GetGunName();


						CCSWeaponInfo* data = weapon->GetCSWpnData();

						if (Options.Visuals.Esp.EspBox.allweapons)
						{
							str += weaponname.c_str();
							Interfaces.pSurface->DrawT(middle, rect.bottom + bot++ * 9 + 1, CColor(0, 255, 0, 255), Hacks.Font_ESP, true, "%s", str.c_str());
						}
						else
						{
							str += weaponname.c_str();
							Interfaces.pSurface->DrawT(middle, rect.bottom + bot++ * 9 + 1, CColor(244, 244, 244, 255), Hacks.Font_Test, true, "%s", str.c_str());
						}
	
				}
			}
			CBaseCombatWeapon* pMainWeapon = pPlayer->GetActiveBaseCombatWeapon();

			if (Options.Visuals.Esp.EspBox.allweapons)
			{
				if (Options.Visuals.Esp.EspMisc.bWeapons)
				{ 
				UINT* hWeapons = pPlayer->GetWeapons2();
				HANDLE* hWeapons2 = pPlayer->GetWeapons3();

				for (int nIndex = 0; hWeapons[nIndex]; nIndex++) //so we wanna start with 1 and do not use the primary
				{
					CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces.pEntList->GetClientEntityFromHandle(hWeapons2[nIndex]);
					if (!pWeapon || pWeapon == pMainWeapon || pWeapon->GetWeaponType() == WEAPONCLASS::WEPCLASS_KNIFE)
						continue;

					std::string s = WeaponIDName(pWeapon->WeaponID());
					std::string str = "";
					float top = 1.4;

					str += s.c_str();
					Interfaces.pSurface->DrawT(middle, rect.bottom + bot++ * 9 + 1, CColor(244, 244, 244, 255), Hacks.Font_ESP, true, "%s", str.c_str());

					//g_pRender->String(x + width / 2, y + height + bot_add ,centered, g_pRender->Fonts.weapon_icon, true, WHITE(255), "%s", pMainWeapon->GetGunIcon());
				}
				}
			}
			/*if (Options.Visuals.Esp.EspBox.bArmorBar)
			{
				CBaseCombatWeapon* weapon = pPlayer->GetActiveBaseCombatWeapon();
				float ammo1 = weapon->ammo();
				float ammo2 = weapon->ammoleft();
				Interfaces.pSurface->DrawT(middle, rect.bottom + bot++ * 9 + 1, CColor(244, 244, 244, 255), Hacks.Font_ESP, true, "(%i)", ammo1);

			}*/
			//delete[]weapon;
		}

	}

	static void DoEsp()
	{
		CBaseEntity* pEntity;
		static bool bCrosshair;
		//Playerbox();

		if (Options.Visuals.VisualsMisc.bHitMarker)
			Hitmarker();

		if (Options.Visuals.Infobox.InfoECHO)
			Info();
		if (Options.Visuals.Esp.EspBox.DrawMultiPoints)
		DrawHeaddot(pEntity);
		//if (Options.Visuals.Infobox.InfoECHO)
			//BulletTrace(pEntity);
		//if (Options.Visuals.Infobox.Grande)
			//Test();
			if (Options.Visuals.Infobox.Grande)
			DiLight();
		//Esp::WeaponSkeleton();
		if(Options.Visuals.VisualsMisc.bNormalCrosshair)
	{
		if(Hacks.LocalPlayer->isAlive())
		{ 
			if (Hacks.LocalPlayer->m_bIsScoped())
			{
				Interfaces.pEngine->ClientCmd_Unrestricted("weapon_debug_spread_show 0", 0);
			}
			else
			{
				Interfaces.pEngine->ClientCmd_Unrestricted("weapon_debug_spread_show 6", 0);
			}
			}

	}
		int width = 0;
		int height = 0;
		Interfaces.pEngine->GetScreenSize( width, height );
		for( auto i = 0; i <= Interfaces.pEntList->GetHighestEntityIndex(); i++ )
		{
			CBaseEntity* pEntity = Interfaces.pEntList->GetClientEntity( i );
			if( pEntity == nullptr )
				continue;
			if( pEntity == Hacks.LocalPlayer )
				continue;
			if( pEntity->IsDormant() )
				continue;

			player_info_t info;
			const ModelRenderInfo_t pInfo;
			if( Interfaces.pEngine->GetPlayerInfo( pEntity->GetIndex(), &info ) )
			{
				CColor clr = pEntity->GetTeam() == Hacks.LocalPlayer->GetTeam() ? CColor( 50, 150, 255, 255 ) : CColor( 255, 0, 0, 255 );
				if( pEntity->GetTeam() == Hacks.LocalPlayer->GetTeam() && Options.Visuals.Esp.EspFilter.bEnemyOnly)
					continue;
				if( !pEntity->isAlive() )
					continue;
				
				if(Options.Visuals.Esp.EspFilter.bXQZ && Misc::isVisible(pEntity, 6))
				pEntity->IsSpotted();
				bool PVS = false;
				RECT rect = DynamicBox( pEntity, PVS );

				DrawInfo( rect, pEntity );
				
				bool EspBox = (Options.Visuals.Esp.EspBox.bBox);
				if(EspBox)
					DrawBox(rect, pEntity);
				
				if( Options.Visuals.Esp.EspBox.bAimlines )
					DrawSnapLine( Vector( rect.left + ( ( rect.right - rect.left ) / 2 ), rect.bottom, 0 ), clr );
				
				if(Options.Visuals.Esp.EspBox.bSkeleton)
					Skeleton( pEntity );	
				int x, y, height;
				
				if(Options.Visuals.VisualsMisc.bEngineRadar)
					*pEntity->IsSpottedPtr() = true;

				float flPlayerAlpha[64];
				if (pEntity->IsDormant() && flPlayerAlpha[pEntity->GetIndex()] > 0)
					flPlayerAlpha[pEntity->GetIndex()] -= 100;
				else if (flPlayerAlpha[pEntity->GetIndex()] < 255 && (!pEntity->IsDormant()))
					flPlayerAlpha[pEntity->GetIndex()] += 20;
			}

			ClientClass* cClass = (ClientClass*)pEntity->GetClientClass();
			if (Options.Visuals.Esp.EspFilter.bDroppedWeapons && cClass->m_ClassID != (int)CSGOClassID::CBaseWeaponWorldModel && ((strstr(cClass->m_pNetworkName, "Weapon") || cClass->m_ClassID == (int)CSGOClassID::CDEagle || cClass->m_ClassID == (int)CSGOClassID::CAK47)))
			{
				float WR = Options.Visuals.Esp.EspBox.MEME2[0] * 255;
				float WG = Options.Visuals.Esp.EspBox.MEME2[1] * 255;
				float WB = Options.Visuals.Esp.EspBox.MEME2[2] * 255;
				bool d = false;
				RECT rect = DynamicBox(pEntity, d);

				Vector vOrig3D = pEntity->GetAbsOrigin();
				Vector vOrig;
				CBaseCombatWeapon* wep = (CBaseCombatWeapon*)pEntity;
				//IClientEntity* plyr = Interfaces.pEntList->GetClientEntityFromHandle((HANDLE)wep->GetOwnerHandle());
				auto plr = Interfaces.pEntList->GetClientEntityFromHandle((HANDLE)wep->GetOwnerHandle());

				if (!plr && !(vOrig3D.y >= -5 && vOrig3D.y <= 5 && vOrig3D.x >= -5 && vOrig3D.x <= 5))
				{
					std::string name = wep->GetGunName();
					Interfaces.pSurface->DrawT(rect.left, rect.top, CColor(WR, WG, WB, 255), Hacks.Font_ESP, true, "%s", name.c_str());
				}
			}
			if(Options.Visuals.Esp.EspFilter.bHostage, pEntity->IsHostage() )
			{
				CColor clr = CColor( 255, 255, 255, 255 );
				bool t;
				RECT rect = DynamicBox( pEntity, t );
				DrawInfo( rect, pEntity );
			}
			if (strstr(pEntity->GetClientClass()->m_pNetworkName, "CC4"))
			{
				float WR = Options.Visuals.Esp.EspBox.MEME3[0] * 255;
				float WG = Options.Visuals.Esp.EspBox.MEME3[1] * 255;
				float WB = Options.Visuals.Esp.EspBox.MEME3[2] * 255;

				{
					Vector vPos3D = pEntity->GetAbsOrigin();
					Vector vPos;
					Interfaces.g_pDebugOverlay->ScreenPosition(vPos3D, vPos);

					Interfaces.pSurface->DrawT(vPos.x + 25, vPos.y, CColor(WR, WG, WB, 255), Hacks.Font_ESP, false, "bomb"); // 3 - red
				}
			}
			if (Options.Visuals.Esp.EspFilter.bDroppedWeapons && strstr(pEntity->GetClientClass()->m_pNetworkName, "CSmokeGrenade"))
			{
				float WR = Options.Visuals.Esp.EspBox.MEME2[0] * 255;
				float WG = Options.Visuals.Esp.EspBox.MEME2[1] * 255;
				float WB = Options.Visuals.Esp.EspBox.MEME2[2] * 255;

				{
					Vector vPos3D = pEntity->GetAbsOrigin();
					Vector vPos;
					Interfaces.g_pDebugOverlay->ScreenPosition(vPos3D, vPos);

					Interfaces.pSurface->DrawT(vPos.x + 25, vPos.y, CColor(WR, WG, WB, 255), Hacks.Font_ESP, false, "smoke"); // 3 - red
				}
			}
			if (Options.Visuals.Esp.EspFilter.bDroppedWeapons && strstr(pEntity->GetClientClass()->m_pNetworkName, "CIncendiaryGrenade"))
			{
				float WR = Options.Visuals.Esp.EspBox.MEME2[0] * 255;
				float WG = Options.Visuals.Esp.EspBox.MEME2[1] * 255;
				float WB = Options.Visuals.Esp.EspBox.MEME2[2] * 255;

				{
					Vector vPos3D = pEntity->GetAbsOrigin();
					Vector vPos;
					Interfaces.g_pDebugOverlay->ScreenPosition(vPos3D, vPos);

					Interfaces.pSurface->DrawT(vPos.x + 25, vPos.y, CColor(WR, WG, WB, 255), Hacks.Font_ESP, false, "molotov"); // 3 - red
				}
			}
			if (Options.Visuals.Esp.EspFilter.bDroppedWeapons && strstr(pEntity->GetClientClass()->m_pNetworkName, "CMolotovGrenade"))
			{
				float WR = Options.Visuals.Esp.EspBox.MEME2[0] * 255;
				float WG = Options.Visuals.Esp.EspBox.MEME2[1] * 255;
				float WB = Options.Visuals.Esp.EspBox.MEME2[2] * 255;

				{
					Vector vPos3D = pEntity->GetAbsOrigin();
					Vector vPos;
					Interfaces.g_pDebugOverlay->ScreenPosition(vPos3D, vPos);

					Interfaces.pSurface->DrawT(vPos.x + 25, vPos.y, CColor(WR, WG, WB, 255), Hacks.Font_ESP, false, "molotov"); // 3 - red
				}
			}
			if (Options.Visuals.Esp.EspFilter.bDroppedWeapons && strstr(pEntity->GetClientClass()->m_pNetworkName, "CDecoyGrenade"))
			{
				float WR = Options.Visuals.Esp.EspBox.MEME2[0] * 255;
				float WG = Options.Visuals.Esp.EspBox.MEME2[1] * 255;
				float WB = Options.Visuals.Esp.EspBox.MEME2[2] * 255;

				{
					Vector vPos3D = pEntity->GetAbsOrigin();
					Vector vPos;
					Interfaces.g_pDebugOverlay->ScreenPosition(vPos3D, vPos);

					Interfaces.pSurface->DrawT(vPos.x + 25, vPos.y, CColor(WR, WG, WB, 255), Hacks.Font_ESP, false, "decoy"); // 3 - red
				}
			}
			if (Options.Visuals.Esp.EspFilter.bDroppedWeapons && strstr(pEntity->GetClientClass()->m_pNetworkName, "CHEGrenade"))
			{
				float WR = Options.Visuals.Esp.EspBox.MEME2[0] * 255;
				float WG = Options.Visuals.Esp.EspBox.MEME2[1] * 255;
				float WB = Options.Visuals.Esp.EspBox.MEME2[2] * 255;

				{
					Vector vPos3D = pEntity->GetAbsOrigin();
					Vector vPos;
					Interfaces.g_pDebugOverlay->ScreenPosition(vPos3D, vPos);

					Interfaces.pSurface->DrawT(vPos.x + 25, vPos.y, CColor(WR, WG, WB, 255), Hacks.Font_ESP, false, "hegrenade"); // 3 - red
				}
			}
			if (Options.Visuals.Esp.EspBox.LBYLINES)
			{
				Vector vecLby, vecReal, vecFake;
				Vector org =Hacks.LocalPlayer->GetAbsOrigin(), vOrg;
				Vector end, vEnd;
				Misc::AngleVectors(Vector(0, Misc::Normalize_y(Hacks.LocalPlayer->pelvisangs()), 0), &vecLby);

				end = org + (vecLby * 70);

				if (g_Math.WorldToScreen(org, vOrg) && g_Math.WorldToScreen(end, vEnd))
				{
					DrawLine(vOrg.x, vOrg.y, vEnd.x, vEnd.y, CColor(255, 255, 255, 255));
				}
				QAngle		RealAngle;
				QAngle		FakeAngle;
				int anglereal = Global::curReal;
				int anglefake = Global::curFake;
				Misc::AngleVectors(Vector(0, Misc::Normalize_y(anglereal), 0), &vecReal);
				Misc::AngleVectors(Vector(0, Misc::Normalize_y(anglefake), 0), &vecFake);



				end = org + (vecFake * 70);

				if (g_Math.WorldToScreen(org, vOrg) && g_Math.WorldToScreen(end, vEnd))
				{
					DrawLine(vOrg.x, vOrg.y, vEnd.x, vEnd.y, CColor(0, 255, 0, 255));
				}
				end = org + (vecReal * 70);

				if (g_Math.WorldToScreen(org, vOrg) && g_Math.WorldToScreen(end, vEnd))
				{
					DrawLine(vOrg.x, vOrg.y, vEnd.x, vEnd.y, CColor(255, 0, 0, 255));
				}
			}
			if (Options.Visuals.Esp.EspFilter.bDroppedWeapons && strstr(pEntity->GetClientClass()->m_pNetworkName, "CSensorGrenade"))
			{
				float WR = Options.Visuals.Esp.EspBox.MEME2[0] * 255;
				float WG = Options.Visuals.Esp.EspBox.MEME2[1] * 255;
				float WB = Options.Visuals.Esp.EspBox.MEME2[2] * 255;

				{
					Vector vPos3D = pEntity->GetAbsOrigin();
					Vector vPos;
					Interfaces.g_pDebugOverlay->ScreenPosition(vPos3D, vPos);

					Interfaces.pSurface->DrawT(vPos.x + 25, vPos.y, CColor(WR, WG, WB, 255), Hacks.Font_ESP, false, "flash"); // 3 - red
				}
			}
			if(strstr( pEntity->GetClientClass()->m_pNetworkName, "CPlantedC4" ) )
			{
				float WR = Options.Visuals.Esp.EspBox.MEME3[0] * 255;
				float WG = Options.Visuals.Esp.EspBox.MEME3[1] * 255;
				float WB = Options.Visuals.Esp.EspBox.MEME3[2] * 255;

				if (!pEntity->BombDefused())
				{
					float flTime = pEntity->BombTimer() - Interfaces.pGlobalVars->curtime;
					if (flTime > 0.f)
					{
						Vector vPos3D = pEntity->GetAbsOrigin();
						Vector vPos;
						Interfaces.g_pDebugOverlay->ScreenPosition(vPos3D, vPos);

						if (flTime > 0.f)
						{
							bool bCanDefuse = (Hacks.LocalPlayer->HasKit() && flTime > 5.5f) || flTime > 10.5f;
							if (flTime < 5.f)
								Interfaces.pSurface->DrawT(vPos.x + 25, vPos.y, bCanDefuse ? CColor(WR, WG, WB, 255) : CColor(WR, WG, WB, 255), Hacks.Font_ESP, false, "bomb", flTime); // 3 - red
						}

						if (flTime > 5.f)
						{
							bool bCanDefuse = (Hacks.LocalPlayer->HasKit() && flTime > 5.5f) || flTime > 10.5f;
							if (flTime < 10.f)
								Interfaces.pSurface->DrawT(vPos.x + 25, vPos.y, bCanDefuse ? CColor(WR, WG, WB, 255) : CColor(WR, WG, WB, 255), Hacks.Font_ESP, false, "bomb", flTime); // 2 - blue
						}

						if (flTime > 10.f)
						{
							bool bCanDefuse = (Hacks.LocalPlayer->HasKit() && flTime > 5.5f) || flTime > 10.5f;
							Interfaces.pSurface->DrawT(vPos.x + 25, vPos.y, bCanDefuse ? CColor(WR, WG, WB, 255) : CColor(WR, WG, WB, 255), Hacks.Font_ESP, false, "bomb", flTime); // 1 - White
						}
						if (Hacks.LocalPlayer->GetHealth() > 0)
						{
							float flArmor = Hacks.LocalPlayer->GetArmor();
							float flDistance = Hacks.LocalPlayer->GetAbsOrigin().DistTo(pEntity->GetAbsOrigin());
							float a = 450.7f;
							float b = 75.68f;
							float c = 789.2f;
							float d = ((flDistance - b) / c);

							float flDmg = a * exp(-d * d);
							float flDamage = flDmg;

							if (flArmor > 0)
							{
								float flNew = flDmg * 0.5f;
								float flArmor = (flDmg - flNew) * 0.5f;

								if (flArmor > static_cast<float>(flArmor))
								{
									flArmor = static_cast<float>(flArmor) * (1.f / 0.5f);
									flNew = flDmg - flArmor;
								}

								flDamage = flNew;


							}
							if (flDamage > 100)
							{
								Interfaces.pSurface->DrawT(80, 50, CColor(255, 0, 0, 255), Hacks.Font_LBY, true, "FATAL", flDamage); // red
							}
							else
							{
								Interfaces.pSurface->DrawT(80, 50, CColor(221, 221, 174, 255), Hacks.Font_LBY, true, "-%.1f HP", flDamage); // red
							}

							int x, y;
							Interfaces.pEngine->GetScreenSize(x, y);

							if (flTime > 0.f)
							{
								bool bCanDefuse = (Hacks.LocalPlayer->HasKit() && flTime > 5.5f) || flTime > 10.5f;
								if (flTime < 5.f)
									Interfaces.pSurface->DrawT(80, 20, flDamage >= Hacks.LocalPlayer->GetHealth() ? CColor(112, 230, 20, 255) : CColor(112, 230, 20, 255), Hacks.Font_LBY, true, " %.1f ", flTime); // red
							}

							if (flTime > 5.f)
							{
								bool bCanDefuse = (Hacks.LocalPlayer->HasKit() && flTime > 5.5f) || flTime > 10.5f;
								if (flTime < 10.f)
									Interfaces.pSurface->DrawT(80, 20, flDamage >= Hacks.LocalPlayer->GetHealth() ? CColor(112, 230, 20, 255) : CColor(112, 230, 20, 255), Hacks.Font_LBY, true, "%.1f ", flTime); // blue
							}

							if (flTime > 10.f)
							{
								bool bCanDefuse = (Hacks.LocalPlayer->HasKit() && flTime > 5.5f) || flTime > 10.5f;
								Interfaces.pSurface->DrawT(80, 20, flDamage >= Hacks.LocalPlayer->GetHealth() ? CColor(112, 230, 20, 255) : CColor(112, 230, 20, 255), Hacks.Font_LBY, true, "%.1f ", flTime); // white
							}
						}
					}
				}
			}
			//delete[]pEntity;
		}
	}

	static RECT DynamicBox( CBaseEntity* pPlayer, bool& PVS )
	{
		Vector trans = pPlayer->GetVecOrigin();

		Vector min;
		Vector max;

		min = pPlayer->BBMin();
		max = pPlayer->BBMax();

		Vector pointList[] = {
			Vector( min.x, min.y, min.z ),
			Vector( min.x, max.y, min.z ),
			Vector( max.x, max.y, min.z ),
			Vector( max.x, min.y, min.z ),
			Vector( max.x, max.y, max.z ),
			Vector( min.x, max.y, max.z ),
			Vector( min.x, min.y, max.z ),
			Vector( max.x, min.y, max.z )
		};

		Vector Distance = pointList[ 0 ] - pointList[ 1 ];
		int dst = Distance.Length();
		dst /= 1.3f;
		Vector angs;
		Misc::CalcAngle( trans, Hacks.LocalPlayer->GetEyePosition(), angs );

		Vector all[8];
		angs.y += 45;
		for( int i = 0; i < 4; i++ )
		{
			g_Math.angleVectors( angs, all[ i ] );
			all[ i ] *= dst;
			all[ i + 4 ] = all[ i ];
			all[ i ].z = max.z;
			all[ i + 4 ].z = min.z;
			VectorAdd( all[ i ], trans, all[ i ] );
			VectorAdd( all[ i + 4 ], trans, all[ i + 4 ] );
			angs.y += 90;
		}

		Vector flb, brt, blb, frt, frb, brb, blt, flt;
		PVS = true;

		if( !Interfaces.g_pDebugOverlay->ScreenPosition( all[ 3 ], flb ) )
			PVS = false;
		if( !Interfaces.g_pDebugOverlay->ScreenPosition( all[ 0 ], blb ) )
			PVS = false;
		if( !Interfaces.g_pDebugOverlay->ScreenPosition( all[ 2 ], frb ) )
			PVS = false;
		if( !Interfaces.g_pDebugOverlay->ScreenPosition( all[ 6 ], blt ) )
			PVS = false;
		if( !Interfaces.g_pDebugOverlay->ScreenPosition( all[ 5 ], brt ) )
			PVS = false;
		if( !Interfaces.g_pDebugOverlay->ScreenPosition( all[ 4 ], frt ) )
			PVS = false;
		if( !Interfaces.g_pDebugOverlay->ScreenPosition( all[ 1 ], brb ) )
			PVS = false;
		if( !Interfaces.g_pDebugOverlay->ScreenPosition( all[ 7 ], flt ) )
			PVS = false;

		Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

		float left = flb.x;
		float top = flb.y;
		float right = flb.x;
		float bottom = flb.y;

		for( int i = 0; i < 8; i++ )
		{
			if( left > arr[ i ].x )
				left = arr[ i ].x;
			if( top > arr[ i ].y )
				top = arr[ i ].y;
			if( right < arr[ i ].x )
				right = arr[ i ].x;
			if( bottom < arr[ i ].y )
				bottom = arr[ i ].y;
		}
		RECT rect;
		rect.left = left;
		rect.bottom = bottom;
		rect.right = right;
		rect.top = top;
		return rect;
	}

	static void Skeleton(CBaseEntity* pEntity)
	{
		CColor col = CColor(255, 255, 255, 255);


		studiohdr_t* pStudioHdr = Interfaces.g_pModelInfo->GetStudioModel(pEntity->GetModel());

		if (!pStudioHdr)
			return;

		Vector vParent, vChild, sParent, sChild;

		for (int j = 0; j < pStudioHdr->numbones; j++)
		{
			mstudiobone_t* pBone = pStudioHdr->GetBone(j);

			if (pBone && (pBone->flags & BONE_USED_BY_HITBOX) && (pBone->parent != -1))
			{
				vChild = pEntity->GetBonePos(j);
				vParent = pEntity->GetBonePos(pBone->parent);
				Interfaces.g_pDebugOverlay->ScreenPosition(vParent, sParent);
				Interfaces.g_pDebugOverlay->ScreenPosition(vChild, sChild);
				int red = 0;
				int green = 0;
				int blue = 0;
				int alpha = 0;
				col.GetColor(red, green, blue, alpha);
				Interfaces.pSurface->DrawSetColor(red, green, blue, alpha);
				Interfaces.pSurface->DrawLine(sParent[0], sParent[1], sChild[0], sChild[1]);
			}
			//delete[]pBone;
		}

	}

	static void DrawSnapLine( Vector to, CColor clr )
	{
		int width = 0;
		int height = 0.5;
		Interfaces.pEngine->GetScreenSize( width, height );
		Vector From( ( width / 2 ), height - 1, 0 );
		DrawLine( From.x, From.y, to.x, to.y, clr );
	}

	static void DrawBox( RECT rect, CBaseEntity* pEntity)
	{

		float red;
		float green;
		float blue;
		if (pEntity->GetTeam() == 2)
		{
			if (Misc::isVisible(pEntity, 6))
			{
				red = 255;
				green = 255;
				blue = 255;
			}
			else
			{
				red = 255;
				green = 255;
				blue = 255;
			}
		}
		else
		{
			if (Misc::isVisible(pEntity, 6))
			{
				red = 255;
				green = 255;
				blue = 255;
			}
			else
			{
				red = 255;
				green = 255;
				blue = 255;
			}
		}

		float newR = Options.Visuals.Esp.EspBox.MEME[0];
		float newG = Options.Visuals.Esp.EspBox.MEME[1];
		float newB = Options.Visuals.Esp.EspBox.MEME[2];

		
		float flPlayerAlpha[64];
		int da = GetDormantAlpha(pEntity) + 55;


			DrawOutlinedRect(rect.left - 1, rect.top - 1, rect.right + 1, rect.bottom + 1, CColor(0, 0, 0, da));
			DrawOutlinedRect(rect.left + 1, rect.top + 1, rect.right - 1, rect.bottom - 1, CColor(0, 0, 0, da));
			DrawOutlinedRect(rect.left, rect.top, rect.right, rect.bottom, CColor(newR * 255, newG * 255, newB * 255, da));
			if (pEntity->IsDormant())
			{

				DrawOutlinedRect(rect.left - 1, rect.top - 1, rect.right + 1, rect.bottom + 1, CColor(0, 0, 0, da));
				DrawOutlinedRect(rect.left + 1, rect.top + 1, rect.right - 1, rect.bottom - 1, CColor(0, 0, 0, da));
				DrawOutlinedRect(rect.left, rect.top, rect.right, rect.bottom, CColor(newR * 255, newG * 255, newB * 255, da));
			}


	}

	/*
	static void DrawHealthBar(CBaseEntity* pEntity, int x, int y, int height)
	{
		float flBoxes = std::ceil(pEntity->GetHealth() / 10.f);
		float flX = x - 7 - height / 4.f;
		float flY = y - 1;
		float flHeight = height / 10.f;
		float flMultiplier = 12 / 360.f;

		flMultiplier *= flBoxes - 1;
		int Red = 255 - (flBoxes * 2.55);
		int Green = flBoxes * 2.55;
		//CColor ColHealth = CColor::FromHSB(flMultiplier, 1, 1);

		DrawRect(flX, flY, 4, height + 2, CColor(80, 80, 80, 125));
		DrawOutlinedRect(flX, flY, 4, height + 2, CColor(0, 0, 0, 255));
		DrawRect(flX + 1, flY, 2, flHeight * flBoxes + 1, CColor(Red, Green, 0, 255));

		for (int i = 0; i < 10; i++)
			DrawLine(flX, flY + i * flHeight, flX + 4, flY + i * flHeight, CColor(0, 0, 0, 255));
	}
	*/
};
