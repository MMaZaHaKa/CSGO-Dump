#include "Chams.h"
#include <fstream>
//#include "SDK\SDK Headers\Entitys.h"

#include "Globals.h"
#define KEY_DOWN(VK_NNM) ((GetAsyncKeyState(VK_NNM) & 0x8000) ? 1:0)

//#include "SDK\SDK Headers\CInput.h"

Chams::Chams()
{
	std::ofstream("csgo\\materials\\simple_regular.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
	std::ofstream("csgo\\materials\\simple_ignorez.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "1"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
	std::ofstream("csgo\\materials\\simple_flat.vmt") << R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
	std::ofstream("csgo\\materials\\simple_flat_ignorez.vmt") << R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "1"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";

	materialRegular = Interfaces.pMaterialSystem->FindMaterial("simple_regular", TEXTURE_GROUP_MODEL);
	materialRegularIgnoreZ = Interfaces.pMaterialSystem->FindMaterial("simple_ignorez", TEXTURE_GROUP_MODEL);
	materialFlatIgnoreZ = Interfaces.pMaterialSystem->FindMaterial("simple_flat_ignorez", TEXTURE_GROUP_MODEL);
	materialFlat = Interfaces.pMaterialSystem->FindMaterial("simple_flat", TEXTURE_GROUP_MODEL);
}

Chams::~Chams()
{
	std::remove("csgo\\materials\\simple_regular.vmt");
	std::remove("csgo\\materials\\simple_ignorez.vmt");
	std::remove("csgo\\materials\\simple_flat.vmt");
	std::remove("csgo\\materials\\simple_flat_ignorez.vmt");
}

void Chams::OverrideMaterial(bool ignoreZ, bool flat, bool wireframe, bool glass, const Color& rgba)
{
	IMaterial* material = nullptr;

	if (flat) {
		if (ignoreZ)
			material = materialFlatIgnoreZ;
		else
			material = materialFlat;
	}
	else {
		if (ignoreZ)
			material = materialRegularIgnoreZ;
		else
			material = materialRegular;
	}


	if (glass) {
		material = materialFlat;
		material->AlphaModulate(0.45f);
	}
	else {
		material->AlphaModulate(
			rgba.a() / 255.0f);
	}

	material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, wireframe);
	material->ColorModulate(
		rgba.r() / 255.0f,
		rgba.g() / 255.0f,
		rgba.b() / 255.0f);

	Interfaces.g_pModelRender->ForcedMaterialOverride(material);
}
void Chams::OnDrawModelExecute(void* ctx, void* state, const ModelRenderInfo_t& info, matrix3x4_t* matrix)
{
	if (Interfaces.pEngine->IsInGame() && Hacks.LocalPlayer && info.pModel)
	{
		/*if (Options.Visuals.Esp.EspBox.Glowmeme)
		{
			IMaterial * visible = Interfaces.pMaterialSystem->FindMaterial("cstm_visible", TEXTURE_GROUP_MODEL);
			visible->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);

			if (Hacks.LocalPlayer)
			{
				auto CoveredLit = visible;
				if (CoveredLit)
				{
					QAngle OrigAng;
					OrigAng = Hacks.LocalPlayer->GetEyeAngles();
					Hacks.LocalPlayer->SetAngle2(QAngle(0, Hacks.LocalPlayer->GetEyeAngles().y, 0));
					bool LbyColor = false; // u can make LBY INDICATOR. When LbyColor is true. Color will be Green , if false it will be White
					float NormalColor[3] = { 1, 1, 1 };
					float lbyUpdateColor[3] = { 0, 1, 0 };
					Interfaces.g_pRenderView->SetColorModulation(LbyColor ? lbyUpdateColor : NormalColor);
					Interfaces.g_pModelRender->ForcedMaterialOverride(CoveredLit);
					Hacks.LocalPlayer->DrawModel(STUDIO_RENDER, 200);
					Interfaces.g_pModelRender->ForcedMaterialOverride(nullptr);
					Hacks.LocalPlayer->SetAngle2(OrigAng);

				}
			}
		}*/
		std::string mN = Interfaces.g_pModelInfo->GetModelName(info.pModel);

		const auto mdl = info.pModel;
		//pNPTR check uff $$$$ Anon ist ne schwuchtel ;))))
		bool isArm = strstr(mdl->szName, "arms") != nullptr;
		bool isPlayer = strstr(mdl->szName, "models/player") != nullptr;
		bool isSleeve = strstr(mdl->szName, "sleeve") != nullptr;
		bool weapon = strstr(mdl->szName, "weapons/v_") != nullptr;
		bool isknife = strstr(mdl->szName, "weapons/v_knife_") != nullptr;


		if (isPlayer && Options.Visuals.Chams.bPlayers)
		{
			//nigga chams

			auto ent = Interfaces.pEntList->GetClientEntity(info.entity_index);

			if (ent && Hacks.LocalPlayer)
			{
				const auto enemy = ent->GetTeam() != Hacks.LocalPlayer->GetTeam();
				bool terror = ent->GetTeam() == 2;
				bool ct = ent->GetTeam() == 3;
				Color col_vis;
				Color col_invis;
				float WR = Options.Visuals.Esp.EspBox.MEME4[0] * 255;
				float WG = Options.Visuals.Esp.EspBox.MEME4[1] * 255;
				float WB = Options.Visuals.Esp.EspBox.MEME4[2] * 255;
				float WR1 = Options.Visuals.Esp.EspBox.MEME5[0] * 255;
				float WG1 = Options.Visuals.Esp.EspBox.MEME5[1] * 255;
				float WB1 = Options.Visuals.Esp.EspBox.MEME5[2] * 255;
				if (!enemy && Options.Visuals.Chams.bEnemyOnly)
					return;

				if (terror)
					col_vis = Color(WR, WG, WB);
				else
					col_vis = Color(WR, WG, WB);

				if (terror)
					col_invis = Color(WR1, WG1, WB1);
				else
					col_invis = Color(WR1, WG1, WB1);


				if (Options.Visuals.Chams.bThroughWalls)
				{
					OverrideMaterial(
						true,
						Options.Visuals.Chams.bFlat,
						Options.Visuals.Chams.bWireframe,
						false,
						col_invis);
					Hacks.oDrawModelExecute(Interfaces.g_pModelRender, ctx, state, info, matrix);
					OverrideMaterial(
						false,
						Options.Visuals.Chams.bFlat,
						Options.Visuals.Chams.bWireframe,
						false,
						col_vis);
				}
				else
				{
					OverrideMaterial(
						false,
						Options.Visuals.Chams.bFlat,
						Options.Visuals.Chams.bWireframe,
						Options.Visuals.Chams.bTransparent,
						col_vis);
				}

			}
		}


		else if (isSleeve && Options.Visuals.Chams.bArms)
		{
			auto material = Interfaces.pMaterialSystem->FindMaterial(mdl->szName, TEXTURE_GROUP_MODEL);
			if (!material)
				return;

			material->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			Interfaces.g_pModelRender->ForcedMaterialOverride(material);
		}


			else if (isArm && Options.Visuals.Chams.bArms)
			{
				float WR = Options.Visuals.Esp.EspBox.MEME6[0] * 255;
				float WG = Options.Visuals.Esp.EspBox.MEME6[1] * 255;
				float WB = Options.Visuals.Esp.EspBox.MEME6[2] * 255;

				auto material = Interfaces.pMaterialSystem->FindMaterial(mdl->szName, TEXTURE_GROUP_MODEL);
				if (!material)
					return;
				Color col_vis = Color(WR, WG, WB);
				Color col_invis = Color(WR, WG, WB);

				if (Options.Visuals.Chams.bArmsThroughWalls)
				{
					OverrideMaterial(
						true,
						Options.Visuals.Chams.bArmsFlat,
						Options.Visuals.Chams.bArmsWireframe,
						Options.Visuals.Chams.bArmsTransparent,
						col_invis
					);
					Hacks.oDrawModelExecute(Interfaces.g_pModelRender, ctx, state, info, matrix);
					OverrideMaterial(
						false,
						Options.Visuals.Chams.bArmsFlat,
						Options.Visuals.Chams.bArmsWireframe,
						Options.Visuals.Chams.bArmsTransparent,
						col_vis
					);
				}
				else
				{
					OverrideMaterial(
						false,
						Options.Visuals.Chams.bArmsFlat,
						Options.Visuals.Chams.bArmsWireframe,
						Options.Visuals.Chams.bArmsTransparent,
						col_vis
					);
				}
			}
			if (Options.Visuals.Chams.Weapon)
			{
				float WR = Options.Visuals.Esp.EspBox.MEME6[0] * 255;
				float WG = Options.Visuals.Esp.EspBox.MEME6[1] * 255;
				float WB = Options.Visuals.Esp.EspBox.MEME6[2] * 255;
				if (isknife)
				{
					auto material = Interfaces.pMaterialSystem->FindMaterial(mdl->szName, TEXTURE_GROUP_MODEL);
					if (!material)
						return;

					material->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
					Interfaces.g_pModelRender->ForcedMaterialOverride(material);

					Color col_vis = Color(WR, WG, WB);
					Color col_invis = Color(WR, WG, WB);
					OverrideMaterial(
						false,
						Options.Visuals.Chams.bArmsFlat,
						Options.Visuals.Chams.bArmsWireframe,
						Options.Visuals.Chams.bArmsTransparent,
						col_vis
					);
				}
				if (weapon)
				{
					auto material = Interfaces.pMaterialSystem->FindMaterial(mdl->szName, TEXTURE_GROUP_MODEL);
					if (!material)
						return;

					material->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
					Interfaces.g_pModelRender->ForcedMaterialOverride(material);

					Color col_vis = Color(WR, WG, WB);
					Color col_invis = Color(WR, WG, WB);
					OverrideMaterial(
						false,
						Options.Visuals.Chams.bArmsFlat,
						Options.Visuals.Chams.bArmsWireframe,
						Options.Visuals.Chams.bArmsTransparent,
						col_vis
					);
				}
			}

			/*if (Options.Visuals.Chams.bFakeAngleChams)
			{
				if (Hacks.LocalPlayer)
				{
					IMaterial* mat = Interfaces.pMaterialSystem->CreateMaterial(false, false);
					//IMaterial *covered = ChamsStyle1337 ? CoveredLit : CoveredFlat;
					if (mat)
					{
						Vector memen;
						memen.y = (Hacks.CurrentCmd->viewangles.y);
						Vector OrigAng;
						OrigAng = memen;
						Hacks.LocalPlayer->SetAngle2(Vector(0, Hacks.LocalPlayer->pelvisangs(), 0)); // paste here ur AA.y value or pLocal->GetLby() (for example)
						bool LbyColor = false; // u can make LBY INDICATOR. When LbyColor is true. Color will be Green , if false it will be White
						float NormalColor[3] = { 1, 1, 1 };
						float lbyUpdateColor[3] = { 0, 1, 0 };
						Interfaces.g_pRenderView->SetColorModulation(LbyColor ? lbyUpdateColor : NormalColor);
						Interfaces.g_pRenderView->SetBlend(1.0f);
						Interfaces.g_pModelRender->ForcedMaterialOverride(mat);

						Interfaces.g_pModelRender->ForcedMaterialOverride(nullptr);
						Hacks.LocalPlayer->SetAngle2(OrigAng);
					}
				}
			}*/

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

			if (isPlayer && Options.Visuals.VisualsMisc.iThirdPerson >= 1 && enabledtp && Hacks.LocalPlayer->GetHealth() > 0)
			{
				auto ent = Interfaces.pEntList->GetClientEntity(info.entity_index);

				if (ent == Hacks.LocalPlayer)
					if (Hacks.LocalWeapon->isSniper())
						if (Hacks.LocalPlayer->m_bIsScoped())
							OverrideMaterialTP(false, false, false, true);
			}

		}
	}

void Chams::OverrideMaterialTP(bool ignoreZ, bool flat, bool wireframe, bool glass)
{
	IMaterial* material = nullptr;

	if (flat) {
		if (ignoreZ)
			material = materialFlatIgnoreZ;
		else
			material = materialFlat;
	}
	else {
		if (ignoreZ)
			material = materialRegularIgnoreZ;
		else
			material = materialRegular;
	}


	if (glass) {
		material = materialFlat;
		material->AlphaModulate(0.20f);
	}

	Interfaces.g_pModelRender->ForcedMaterialOverride(material);
}

