#include "Hooks.h"
#include "newMenu.h"
#include "Tools/Hacks/Misc.h"

Vector MEME(Vector EntityPos, Vector LocalPlayerPos, int posX, int posY, int sizeX, int sizeY, float angle, float zoom, bool * viewCheck)
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
bool rage;
bool legit;
bool visuals;
bool misc;
bool skins;
bool colors;
//cOptions Options;
void Unhook();
void Gui_Init(IDirect3DDevice9* pDevice);
extern IMGUI_API LRESULT ImGui_ImplDX9_WndProcHandler(HWND, UINT msg, WPARAM wParam, LPARAM lParam);
#define XorStr
#define XorStr1
#define XorStr2
#define XorStr3
#define XorStr4
#define XorStr5
#define XorStr6
#define XorStr7
#define XorStr8
#define XorStr9
#define XorStr10
#define XorStr11
#define XorStr12
#define XorStr13
#define XorStr14
#define XorStr15
#define XorStr16
#define XorStr17
#define XorStr18
#define IM_PI                   3.14159265358979323846f
static inline int    ImMax(int lhs, int rhs) { return lhs >= rhs ? lhs : rhs; }

int PiePopupSelectMenu(const ImVec2& center, const char* popup_id, const char** items, int items_count, int* p_selected)
{
	int ret = -1;

	// FIXME: Missing a call to query if Popup is open so we can move the PushStyleColor inside the BeginPopupBlock (e.g. IsPopupOpen() in imgui.cpp)
	// FIXME: Our PathFill function only handle convex polygons, so we can't have items spanning an arc too large else inner concave edge artifact is too visible, hence the ImMax(7,items_count)
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
	if (ImGui::BeginPopup(popup_id))
	{
		const ImVec2 drag_delta = ImVec2(ImGui::GetIO().MousePos.x - center.x, ImGui::GetIO().MousePos.y - center.y);
		const float drag_dist2 = drag_delta.x*drag_delta.x + drag_delta.y*drag_delta.y;

		const ImGuiStyle& style = ImGui::GetStyle();
		const float RADIUS_MIN = 30.0f;
		const float RADIUS_MAX = 120.0f;
		const float RADIUS_INTERACT_MIN = 20.0f;
		const int ITEMS_MIN = 6;

		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		//ImGuiWindow* window = ImGui::GetCurrentWindow();
		draw_list->PushClipRectFullScreen();
		draw_list->PathArcTo(center, (RADIUS_MIN + RADIUS_MAX)*0.5f, 0.0f, IM_PI*2.0f*0.99f, 32);   // FIXME: 0.99f look like full arc with closed thick stroke has a bug now
		draw_list->PathStroke(ImColor(0, 0, 0), true, RADIUS_MAX - RADIUS_MIN);

		const float item_arc_span = 2 * IM_PI / ImMax(ITEMS_MIN, items_count);
		float drag_angle = atan2f(drag_delta.y, drag_delta.x);
		if (drag_angle < -0.5f*item_arc_span)
			drag_angle += 2.0f*IM_PI;
		//ImGui::Text("%f", drag_angle);    // [Debug]

		int item_hovered = -1;
		for (int item_n = 0; item_n < items_count; item_n++)
		{
			const char* item_label = items[item_n];
			const float item_ang_min = item_arc_span * (item_n + 0.02f) - item_arc_span*0.5f; // FIXME: Could calculate padding angle based on how many pixels they'll take
			const float item_ang_max = item_arc_span * (item_n + 0.98f) - item_arc_span*0.5f;

			bool hovered = false;
			if (drag_dist2 >= RADIUS_INTERACT_MIN*RADIUS_INTERACT_MIN)
			{
				if (drag_angle >= item_ang_min && drag_angle < item_ang_max)
					hovered = true;
			}
			bool selected = p_selected && (*p_selected == item_n);

			int arc_segments = (int)(32 * item_arc_span / (2 * IM_PI)) + 1;
			draw_list->PathArcTo(center, RADIUS_MAX - style.ItemInnerSpacing.x, item_ang_min, item_ang_max, arc_segments);
			draw_list->PathArcTo(center, RADIUS_MIN + style.ItemInnerSpacing.x, item_ang_max, item_ang_min, arc_segments);
			//draw_list->PathFill(window->Color(hovered ? ImGuiCol_HeaderHovered : ImGuiCol_FrameBg));
			draw_list->PathFillConvex(hovered ? ImColor(100, 100, 150) : ImColor(70, 70, 70));

			ImVec2 text_size = ImGui::GetWindowFont()->CalcTextSizeA(ImGui::GetWindowFontSize(), FLT_MAX, 0.0f, item_label);
			ImVec2 text_pos = ImVec2(
				center.x + cosf((item_ang_min + item_ang_max) * 0.5f) * (RADIUS_MIN + RADIUS_MAX) * 0.5f - text_size.x * 0.5f,
				center.y + sinf((item_ang_min + item_ang_max) * 0.5f) * (RADIUS_MIN + RADIUS_MAX) * 0.5f - text_size.y * 0.5f);
			draw_list->AddText(text_pos, ImColor(255, 255, 255), item_label);

			if (hovered)
				item_hovered = item_n;
		}
		draw_list->PopClipRect();

		if (ImGui::IsMouseReleased(0))
		{
			ImGui::CloseCurrentPopup();
			ret = item_hovered;
			if (p_selected)
				*p_selected = item_hovered;
		}
		ImGui::EndPopup();
	}
	ImGui::PopStyleColor(2);
	return ret;
}

void Gui_Init(IDirect3DDevice9* pDevice)
{
	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = 1.f / 60.f;
	D3DDEVICE_CREATION_PARAMETERS d3dcp{ 0 };
	pDevice->GetCreationParameters(&d3dcp);

	auto& style = ImGui::GetStyle();

	Global::fDefault = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Visitor TT2 BRK Regular", 15.0f);
	Global::fSubTabs = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Visitor TT2 BRK Regular", 25.0f);



	ImGui_ImplDX9_Init(INIT::Window, pDevice);

	Global::MenuInit = true;
}

void TextColor(bool active)
{
	auto& style = ImGui::GetStyle();

	if (active)
	{
		style.Colors[ImGuiCol_Text] = ImVec4(0.98f, 0.98f, 0.98f, 1.f);
	}
	else
		style.Colors[ImGuiCol_Text] = ImVec4(0.39f, 0.39f, 0.39f, 1.f);
}

void DefineTab()
{
	if (Global::MenuTab == 0)
	{
		rage = true;
		visuals = false;
		misc = false;
		colors = false;
		skins = false;
		legit = false;

	}
	else if (Global::MenuTab == 1)
	{
		rage = false;
		visuals = true;
		misc = false;
		colors = false;
		skins = false;
		legit = false;

	}
	else if (Global::MenuTab == 2)
	{
		rage = false;
		visuals = false;
		misc = true;
		colors = false;
		skins = false;
		legit = false;

	}
	else if (Global::MenuTab == 3)
	{
		rage = false;
		visuals = false;
		misc = false;
		colors = true;
		skins = false;
		legit = false;

	}
	else if (Global::MenuTab == 4)
	{
		rage = false;
		visuals = false;
		misc = false;
		colors = false;
		skins = true;
		legit = false;


	}
	else if (Global::MenuTab == 5)
	{
		rage = false;
		visuals = false;
		misc = false;
		colors = false;
		skins = false;
		legit = true;
	}
}

void BtnNormal()
{
	auto& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Button] = ImVec4(0.10, 0.10, 0.10, .98f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.13f, 0.13f, 0.13f, 1.f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.f);
}

void BtnActive()
{
	auto& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Button] = ImVec4(0.90f, 0.90f, 0.90f, 0.90f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.15f, 0.15f, 0.15f, 1.f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.f);
}

void DrawRageTab()
{
	auto& style = ImGui::GetStyle();
	BtnNormal();

	style.ItemSpacing = ImVec2(0, 4);

	// Sub-Tabs ghetto


	style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.f);

	style.WindowPadding = ImVec2(8, 8);
	style.ItemSpacing = ImVec2(4, 4);

	if (Global::RageSubTab == 0)
	{
		ImGui::BeginChild("Aimbot - General", ImVec2(220, 398), true);
		{
			ImGui::Text("Aimbot");

			if (Options.Ragebot.Aimbot.bAimbotEnable)
			{
				ImGui::Checkbox("Enabled", &Options.Ragebot.Aimbot.bAimbotEnable);
			}
			else
			{
				ImGui::Checkbox("Enable", &Options.Ragebot.Aimbot.bAimbotEnable);
			}

			ImGui::Text("Target selection");
			ImGui::Combo("##tgtslt", &Options.Ragebot.Aimbot.iSelection, iSelection, ARRAYSIZE(iSelection));
			ImGui::Text("Target hitbox");
			ImGui::Combo("##hitbox", &Options.Ragebot.Aimbot.iHitbox, iHitbox, ARRAYSIZE(iHitbox));
			ImGui::Checkbox("Multi-point", &Options.Ragebot.Accuracy.bMultipoint);
			ImGui::Text("Multi-point");
			ImGui::Combo("##multibox", &Options.Ragebot.Accuracy.iMultibox, iMultibox, ARRAYSIZE(iMultibox));
			ImGui::Text("Body hitbox scale");
			ImGui::SliderInt("scale", &Options.Ragebot.Accuracy.iPointscale, 0.f, 100.f, "%.f %");
			ImGui::Checkbox("Automatic fire", &Options.Ragebot.Aimbot.bAutoShoot);
			//ImGui::Checkbox("Automatic Scope", &Options.Ragebot.Aimbot.bAutoScope);
			ImGui::Checkbox("Automatic penetration", &Options.Ragebot.Aimbot.bAutoPistol);
			//ImGui::Checkbox("Auto Revolver", &Options.Ragebot.Aimbot.bAutoRevolver);
			ImGui::Checkbox("Silent aim", &Options.Ragebot.Aimbot.bSilentAim);
			ImGui::Text("Minimum hit chance");
			ImGui::SliderInt("hitchance", &Options.Ragebot.Accuracy.iHitchance, 0.f, 100.f, "%.f");
			ImGui::Text("Minimum damage");
			ImGui::SliderInt("damage", &Options.Ragebot.Accuracy.iMinDmg, 0.f, 125.f, "%.f");
			ImGui::Checkbox("Automatic Scope", &Options.Ragebot.Aimbot.bAutoScope);
			ImGui::Checkbox("Auto Zeus", &Options.Ragebot.Aimbot.AutoZeus);

		/*static const char* test_data = "Menu";
			const char* items[] = { "Orange", "Blue", "Purple", "Gray", "Yellow", "Las Vegas" };
			int items_count = sizeof(items) / sizeof(*items);

			static int selected = 0;

			ImGui::Button(selected >= 0 ? items[selected] : "Menu", ImVec2(50, 50));
			if (ImGui::IsItemActive())          // Don't wait for button release to activate the pie menu
				ImGui::OpenPopup("##piepopup");

			ImVec2 pie_menu_center = ImGui::GetIO().MouseClickedPos[0];
			PiePopupSelectMenu(pie_menu_center, "##piepopup", items, items_count, &selected);*/


		}ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("Aimbot - Accuracy", ImVec2(220, 398), true);
		{
			ImGui::Text("Other");
			ImGui::Checkbox("Remove recoil", &Options.Ragebot.Accuracy.bNoRecoil);
			ImGui::Checkbox("Qucik stop", &Options.Ragebot.Accuracy.bAutoStop);
		    ImGui::Checkbox("Position Adjustment", &Options.Ragebot.Accuracy.bVelocityPrediction);
		ImGui::Checkbox("Accuracy boost", &Options.Ragebot.HvH.bPositionAdj);
			ImGui::Checkbox("Backtrack", &Options.Ragebot.Aimbot.RageBacktrack);
			ImGui::Checkbox("Anti-aim store angles", &Options.Ragebot.HvH.ForceAngles);
			ImGui::Checkbox("Anti-aim correction", &Options.Ragebot.Aimbot.ResolverTest);
			ImGui::Checkbox("Anti-aim correction2", &Options.Ragebot.HvH.bResolver);

			if (Options.Ragebot.HvH.bResolver)
			{
				ImGui::NewLine();
				ImGui::SameLine(28);
				ImGui::Checkbox("Delta correction", &Options.Ragebot.HvH.delta2);
				ImGui::NewLine();
				ImGui::SameLine(28);
				ImGui::Text("Prefer Body aim");
				ImGui::NewLine();
				ImGui::SameLine(28);
				ImGui::Combo("##prf", &Options.Ragebot.HvH.iPreferBaim, iPreferBaim, ARRAYSIZE(iPreferBaim));
			}
			//ImGui::Text("Multibox");
			//ImGui::Combo("##multibox", &Options.Ragebot.Accuracy.iMultibox, iMultibox, ARRAYSIZE(iMultibox));
			//ImGui::Checkbox("Multipoint", &Options.Ragebot.Accuracy.bMultipoint);
			//ImGui::SliderInt("Pointscale", &Options.Ragebot.Accuracy.iPointscale, 0.f, 100.f, "%.f");
			////ImGui::SliderInt("Hitchance ", &Options.Ragebot.Accuracy.iHitchance, 0.f, 100.f, "%.f");
			//ImGui::SliderInt("Min Dmg", &Options.Ragebot.Accuracy.iMinDmg, 0.f, 125.f, "%.f");
			//ImGui::Checkbox("Remove Recoil", &Options.Ragebot.Accuracy.bNoRecoil);
			//ImGui::Checkbox("Qucik Autostop", &Options.Ragebot.Accuracy.bAutoStop);
			//if (Options.Ragebot.Accuracy.bAutoStop)
			//	ImGui::Hotkey("Key##mgkbmksfgbsgfdb", &Options.Ragebot.Accuracy.iAutoStopKey, ImVec2(150, 20));

		}ImGui::EndChild();
		ImGui::BeginChild("Hack vs Hack", ImVec2(220, 398), true);
		{
		ImGui::Text("Fake lag");
		ImGui::Combo("##faglag", &Options.Misc.Misc1.iFakeLag, iFakeLag, ARRAYSIZE(iFakeLag));
		if(Options.Misc.Misc1.iFakeLag == 1)
		ImGui::SliderInt("Factor", &Options.Misc.Misc1.iFakeLagFactor, 0, 20, "%.0f");

		}ImGui::EndChild();
		/*ImGui::SameLine();
		ImGui::BeginChild("Work in Progress", ImVec2(190, 398), true);
		{
			ImGui::Text("Indicators");
			ImGui::Separator();
			ImGui::Checkbox("LBY", &Options.Visuals.Infobox.bInfoLby);
			ImGui::Checkbox("LC", &Options.Visuals.Infobox.LC);
			ImGui::Checkbox("CLAN TAG", &Options.Visuals.Infobox.bClantag);
			ImGui::Checkbox("INFO", &Options.Visuals.Infobox.InfoECHO);
			ImGui::Separator();
		}ImGui::EndChild();*/
	}
	else if (Global::RageSubTab == 1)
	{
		ImGui::BeginChild("##aastand", ImVec2(190, 398), true);
		{		
			ImGui::Text("Anti-aimbot");
			if (Options.Ragebot.AntiAim.Stand.bEnableAntiAim)
			{
				ImGui::Checkbox("Enabled", &Options.Ragebot.AntiAim.Stand.bEnableAntiAim);
			}
			else
			{
				ImGui::Checkbox("Enable", &Options.Ragebot.AntiAim.Stand.bEnableAntiAim);
			}

			ImGui::Text("Pitch");
			if(Options.Misc.Misc2.bAntiUntrusted)
				ImGui::Combo("##sfgjwftj", &Options.Ragebot.AntiAim.Stand.iPitch, iPitch_Spread, ARRAYSIZE(iPitch_Spread));
			else
				ImGui::Combo("##fsghjdtzjudghj", &Options.Ragebot.AntiAim.Stand.iPitch, iPitch_Nospread, ARRAYSIZE(iPitch_Nospread));
			ImGui::Text("Yaw");
			ImGui::Combo("##35673j", &Options.Ragebot.AntiAim.Stand.iFakeYaw, iFakeYaw, ARRAYSIZE(iFakeYaw));
			ImGui::SliderInt(" 		       Add##standfake", &Options.Ragebot.AntiAim.Stand.iFakeYawAdd, -180, 180, "%.0f");
			ImGui::Text("Yaw while running");
			ImGui::Combo("##fhjkdtrhfdgsj", &Options.Ragebot.AntiAim.Move.iFakeYaw, iFakeYaw, ARRAYSIZE(iFakeYaw));
			ImGui::SliderInt(" 		       Add##movefake", &Options.Ragebot.AntiAim.Move.iFakeYawAdd, -180, 180, "%.0f");
			ImGui::Text("Fake yaw");
			ImGui::Combo("##dghkfgh", &Options.Ragebot.AntiAim.Stand.iYaw, iYaw, ARRAYSIZE(iYaw));
			ImGui::SliderInt(" 		       Add##standreal", &Options.Ragebot.AntiAim.Stand.iYawAdd, -180, 180, "%.0f");
			ImGui::Checkbox("Moon walk", &Options.Ragebot.AntiAim.Move.bMemeWalk);
			ImGui::Checkbox("Fake walk", &Options.Ragebot.AntiAim.Misc.bFakeWalk);
			ImGui::Hotkey("Key##fakewalkkey", &Options.Ragebot.AntiAim.Misc.iFakeWalkKey, ImVec2(150, 20));
			//ImGui::Checkbox("Sideways Key", &Options.Ragebot.AntiAim.Move.lockside3);
			ImGui::Checkbox("Free Standing", &Options.Ragebot.AntiAim.Stand.FreeStanding);
			ImGui::Text("Lby delta");
			ImGui::SliderInt("##LBYDELTA", &Options.Ragebot.AntiAim.Stand.LBYDELTA, 0, 180, "%.0f");




		}ImGui::EndChild();
		/*ImGui::SameLine();
		ImGui::BeginChild("##aamove", ImVec2(190, 398), true);
		{
			ImGui::Text("Anti Aim Move");
			ImGui::Separator();
			ImGui::Checkbox("Enable Anti Aim", &Options.Ragebot.AntiAim.Move.bEnableAntiAim);
			ImGui::Checkbox("Memewalk", &Options.Ragebot.AntiAim.Move.bMemeWalk);
			ImGui::Separator();
			ImGui::Text("Pitch");
			if (Options.Misc.Misc2.bAntiUntrusted)
				ImGui::Combo("##etzjdghj", &Options.Ragebot.AntiAim.Move.iPitch, iPitch_Spread, ARRAYSIZE(iPitch_Spread));
			else
				ImGui::Combo("##bhfnkfhujku", &Options.Ragebot.AntiAim.Move.iPitch, iPitch_Nospread, ARRAYSIZE(iPitch_Nospread));
			ImGui::Text("Yaw");
			ImGui::Combo("##sfgjsdzkjsdtjsfgh", &Options.Ragebot.AntiAim.Move.iYaw, iYaw, ARRAYSIZE(iYaw));
			ImGui::SliderInt(" 		       Add##movereal", &Options.Ragebot.AntiAim.Move.iYawAdd, -180, 180, "%.0f");
			ImGui::Text("Fake Yaw");
			ImGui::Combo("##fhjkdtrhfdgsj", &Options.Ragebot.AntiAim.Move.iFakeYaw, iFakeYaw, ARRAYSIZE(iFakeYaw));
			ImGui::SliderInt(" 		       Add##movefake", &Options.Ragebot.AntiAim.Move.iFakeYawAdd, -180, 180, "%.0f");
			ImGui::Checkbox("LOCK SIDE", &Options.Ragebot.AntiAim.Move.lockside3);

		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("##edge", ImVec2(190, 398), true);
		{
			ImGui::Text("Edge Detection");
			ImGui::Separator();
			ImGui::Checkbox("Enable Edge Detection", &Options.Ragebot.AntiAim.Edge.bEnableEdge);
			ImGui::Separator();
			ImGui::Text("Pitch");
			if (Options.Misc.Misc2.bAntiUntrusted)
				ImGui::Combo("##etzkgrh", &Options.Ragebot.AntiAim.Edge.iPitch, iPitch_Spread, ARRAYSIZE(iPitch_Spread));
			else
				ImGui::Combo("##gzhk367i", &Options.Ragebot.AntiAim.Edge.iPitch, iPitch_Nospread, ARRAYSIZE(iPitch_Nospread));
			ImGui::Text("Yaw");
			ImGui::Combo("##fhjlkzuo", &Options.Ragebot.AntiAim.Edge.iYaw, iEdgeReal, ARRAYSIZE(iEdgeReal));
			ImGui::SliderInt(" 		       Add##edgereal", &Options.Ragebot.AntiAim.Edge.iYawAdd, -180, 180, "%.0f");
			ImGui::Text("Fake Yaw");
			ImGui::Combo("##ghkmtzk", &Options.Ragebot.AntiAim.Edge.iFakeYaw, iEdgeFake, ARRAYSIZE(iEdgeFake));
			ImGui::SliderInt(" 		       Add##edgefake", &Options.Ragebot.AntiAim.Edge.iFakeYawAdd, -180, 180, "%.0f");
		
		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("##aamisc", ImVec2(190, 398), true);
		{
			ImGui::Text("Anti Aim Misc");
			ImGui::Separator();
			ImGui::Checkbox("Fakewalk", &Options.Ragebot.AntiAim.Misc.bFakeWalk);
			ImGui::Hotkey("Key##fakewalkkey", &Options.Ragebot.AntiAim.Misc.iFakeWalkKey, ImVec2(150, 20));

		}ImGui::EndChild();*/
	}
}

void DrawLegitTab()
{
	auto& style = ImGui::GetStyle();
	BtnNormal();
	ImGui::PushFont(Global::fSubTabs);
	style.ItemSpacing = ImVec2(0, 4);

	if (Global::LegitSubTab == 0)
		style.Colors[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1.f, 1.f);
	else
		style.Colors[ImGuiCol_Text] = ImVec4(.3f, .3f, .3f, 1.f);

	if (ImGui::Button("G", ImVec2(128.5, 32))) Global::LegitSubTab = 0;
	ImGui::SameLine();
	
	if (Global::LegitSubTab == 1)
		style.Colors[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1.f, 1.f);
	else
		style.Colors[ImGuiCol_Text] = ImVec4(.3f, .3f, .3f, 1.f);
	if (ImGui::Button("P", ImVec2(128.5, 32))) Global::LegitSubTab = 1;
	ImGui::SameLine();

	if (Global::LegitSubTab == 2)
		style.Colors[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1.f, 1.f);
	else
		style.Colors[ImGuiCol_Text] = ImVec4(.3f, .3f, .3f, 1.f);
	if (ImGui::Button("W", ImVec2(128.5, 32))) Global::LegitSubTab = 2;
	ImGui::SameLine();

	if (Global::LegitSubTab == 3)
		style.Colors[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1.f, 1.f);
	else
		style.Colors[ImGuiCol_Text] = ImVec4(.3f, .3f, .3f, 1.f);
	if (ImGui::Button("d", ImVec2(128.5, 32))) Global::LegitSubTab = 3;
	ImGui::SameLine();

	if (Global::LegitSubTab == 4)
		style.Colors[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1.f, 1.f);
	else
		style.Colors[ImGuiCol_Text] = ImVec4(.3f, .3f, .3f, 1.f);

	if (ImGui::Button("f", ImVec2(128.5, 32))) Global::LegitSubTab = 4;
	ImGui::SameLine();

	if (Global::LegitSubTab == 5)
		style.Colors[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1.f, 1.f);
	else
		style.Colors[ImGuiCol_Text] = ImVec4(.3f, .3f, .3f, 1.f);
	if (ImGui::Button("a", ImVec2(128.5, 32))) Global::LegitSubTab = 5;


	style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.f);

	style.WindowPadding = ImVec2(8, 8);
	style.ItemSpacing = ImVec2(4, 4);

	ImGui::PushFont(Global::fDefault);
	if (Global::LegitSubTab == 0)
	{
		ImGui::BeginChild("Pistols##aimbot", ImVec2(200, 398), true);
		{
			ImGui::Text("Aimbot - Pistols");
			ImGui::Separator();
			ImGui::Checkbox("Enable Aimbot", &Options.Legitbot.Aimbot.Pistols.bEnableAimbot);
			ImGui::SameLine();
			ImGui::Checkbox("On Key", &Options.Legitbot.Aimbot.Pistols.bOnKey);
			if (Options.Legitbot.Aimbot.Pistols.bOnKey)
				ImGui::Hotkey("##pistolkey", &Options.Legitbot.Aimbot.Pistols.iKey, ImVec2(150, 20));
			ImGui::Separator();
			ImGui::Text("Hitbox");
			ImGui::Combo("##hitbox", &Options.Legitbot.Aimbot.Pistols.iAimbotHitbox, iHitboxLegit, ARRAYSIZE(iHitboxLegit));
			ImGui::Checkbox("Silent Aim", &Options.Legitbot.Aimbot.Pistols.bSilentAim);
			ImGui::Text("Aimbot Fov");
			ImGui::SliderFloat("Degrees##fov", &Options.Legitbot.Aimbot.Pistols.iAimbotFov, 0.0, 20, "%.1f");
			ImGui::Text("Aimbot Rcs");
			ImGui::SliderFloat("%##rcs", &Options.Legitbot.Aimbot.Pistols.flAimbotRecoil, 0, 100, "%.0f", 1.0f);

		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("Pistols##trigger", ImVec2(200, 398), true);
		{
			ImGui::Text("Trigger - Pistols");
			ImGui::Separator();
			ImGui::Checkbox("Enable", &Options.Legitbot.Trigger.Pistols.bEnableTrigger);
			ImGui::SameLine();
			ImGui::Checkbox("On Key", &Options.Legitbot.Trigger.Pistols.bTriggerOnKey);
			if(Options.Legitbot.Trigger.Pistols.bTriggerOnKey)
				ImGui::Hotkey("##pistolkey", &Options.Legitbot.Trigger.Pistols.iTriggerKey, ImVec2(150, 20));
			/*ImGui::Text("Delay in");
			ImGui::SliderInt("ms", &Options.Legitbot.Trigger.Pistols.iTriggerDelay, 10, 250, "%0.f");*/
			ImGui::Checkbox("Rcs", &Options.Legitbot.Trigger.Pistols.bTriggerRecoil);
			ImGui::Separator();
			ImGui::Checkbox("Auto Revolver", &Options.Legitbot.Trigger.Pistols.bAutoRevolver);
			ImGui::Checkbox("Revolver Secondary Fire", &Options.Legitbot.Trigger.Pistols.bRevolverSecondary);
			ImGui::Checkbox("Auto Pistol", &Options.Legitbot.Trigger.Pistols.bAutoPistol);
			ImGui::Separator();
			ImGui::Text("Trigger - Filter");
			ImGui::Separator();
			ImGui::Checkbox("Full Body", &Options.Legitbot.Trigger.Pistols.bTriggerFullBody);
			ImGui::Checkbox("Head", &Options.Legitbot.Trigger.Pistols.bTriggerHead);
			ImGui::Checkbox("Chest", &Options.Legitbot.Trigger.Pistols.bTriggerChest);
			ImGui::Checkbox("Stomach", &Options.Legitbot.Trigger.Pistols.bTriggerStomach);
			ImGui::Checkbox("Arms", &Options.Legitbot.Trigger.Pistols.bTriggerArms);
			ImGui::Checkbox("Legs", &Options.Legitbot.Trigger.Pistols.bTriggerLegs);
		}ImGui::EndChild();
		ImGui::SameLine();
	}

	if (Global::LegitSubTab == 1)
	{
		ImGui::BeginChild("Smgs##aimbot", ImVec2(200, 398), true);
		{

			ImGui::Text("Aimbot - Smgs");
			ImGui::Separator();
			ImGui::Checkbox("Enable Aimbot", &Options.Legitbot.Aimbot.Smgs.bEnableAimbot);
			ImGui::SameLine();
			ImGui::Checkbox("On Key", &Options.Legitbot.Aimbot.Smgs.bOnKey);
			if (Options.Legitbot.Aimbot.Smgs.bOnKey)
				ImGui::Hotkey("##pistolkey", &Options.Legitbot.Aimbot.Smgs.iKey, ImVec2(150, 20));
			ImGui::Separator();
			ImGui::Text("Hitbox");
			ImGui::Combo("##hitbox", &Options.Legitbot.Aimbot.Smgs.iAimbotHitbox, iHitboxLegit, ARRAYSIZE(iHitboxLegit));
			ImGui::Checkbox("Silent Aim", &Options.Legitbot.Aimbot.Smgs.bSilentAim);
			ImGui::Text("Aimbot Fov");
			ImGui::SliderFloat("Degrees##fov", &Options.Legitbot.Aimbot.Smgs.iAimbotFov, 0.0, 20, "%.1f");
			ImGui::Text("Aimbot Rcs");
			ImGui::SliderFloat("%##rcs", &Options.Legitbot.Aimbot.Smgs.flAimbotRecoil, 0, 100, "%.0f", 1.0f);

		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("Smgs##trigger", ImVec2(200, 398), true);
		{
			ImGui::Text("Trigger - Smgs");
			ImGui::Separator();
			ImGui::Checkbox("Enable", &Options.Legitbot.Trigger.Smgs.bEnableTrigger);
			ImGui::SameLine();
			ImGui::Checkbox("On Key", &Options.Legitbot.Trigger.Smgs.bTriggerOnKey);
			if (Options.Legitbot.Trigger.Smgs.bTriggerOnKey)
				ImGui::Hotkey("##smgskey", &Options.Legitbot.Trigger.Smgs.iTriggerKey, ImVec2(150, 20));
			//ImGui::Text("Delay in");
			//ImGui::SliderInt("ms", &Options.Legitbot.Trigger.Smgs.iTriggerDelay, 10, 250, "%0.f");
			ImGui::Checkbox("Rcs", &Options.Legitbot.Trigger.Smgs.bTriggerRecoil);
			ImGui::Separator();
			ImGui::Text("Trigger - Filter");
			ImGui::Separator();
			ImGui::Checkbox("Full Body", &Options.Legitbot.Trigger.Smgs.bTriggerFullBody);
			ImGui::Checkbox("Head", &Options.Legitbot.Trigger.Smgs.bTriggerHead);
			ImGui::Checkbox("Chest", &Options.Legitbot.Trigger.Smgs.bTriggerChest);
			ImGui::Checkbox("Stomach", &Options.Legitbot.Trigger.Smgs.bTriggerStomach);
			ImGui::Checkbox("Arms", &Options.Legitbot.Trigger.Smgs.bTriggerArms);
			ImGui::Checkbox("Legs", &Options.Legitbot.Trigger.Smgs.bTriggerLegs);
		}ImGui::EndChild();
		ImGui::SameLine();
	}

	if (Global::LegitSubTab == 2)
	{
		ImGui::BeginChild("Rifles##aimbot", ImVec2(200, 398), true);
		{
			ImGui::Text("Aimbot - Rifles");
			ImGui::Separator();
			ImGui::Checkbox("Enable Aimbot", &Options.Legitbot.Aimbot.Rifles.bEnableAimbot);
			ImGui::SameLine();
			ImGui::Checkbox("On Key", &Options.Legitbot.Aimbot.Rifles.bOnKey);
			if (Options.Legitbot.Aimbot.Rifles.bOnKey)
				ImGui::Hotkey("##pistolkey", &Options.Legitbot.Aimbot.Rifles.iKey, ImVec2(150, 20));
			ImGui::Separator();
			ImGui::Text("Hitbox");
			ImGui::Combo("##hitbox", &Options.Legitbot.Aimbot.Rifles.iAimbotHitbox, iHitboxLegit, ARRAYSIZE(iHitboxLegit));
			ImGui::Checkbox("Silent Aim", &Options.Legitbot.Aimbot.Rifles.bSilentAim);
			ImGui::Text("Aimbot Fov");
			ImGui::SliderFloat("Degrees##fov", &Options.Legitbot.Aimbot.Rifles.iAimbotFov, 0.0, 20, "%.1f");
			ImGui::Text("Aimbot Rcs");
			ImGui::SliderFloat("%##rcs", &Options.Legitbot.Aimbot.Rifles.flAimbotRecoil, 0, 100, "%.0f", 1.0f);

		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("Rifles##trigger", ImVec2(200, 398), true);
		{
			ImGui::Text("Trigger - Rifles");
			ImGui::Separator();
			ImGui::Checkbox("Enable", &Options.Legitbot.Trigger.Rifles.bEnableTrigger);
			ImGui::SameLine();
			ImGui::Checkbox("On Key", &Options.Legitbot.Trigger.Rifles.bTriggerOnKey);
			if (Options.Legitbot.Trigger.Rifles.bTriggerOnKey)
				ImGui::Hotkey("##rifleskey", &Options.Legitbot.Trigger.Rifles.iTriggerKey, ImVec2(150, 20));
			//ImGui::Text("Delay in");
			//ImGui::SliderInt("ms", &Options.Legitbot.Trigger.Rifles.iTriggerDelay, 10, 250, "%0.f");
			ImGui::Checkbox("Rcs", &Options.Legitbot.Trigger.Rifles.bTriggerRecoil);
			ImGui::Separator();
			ImGui::Text("Trigger - Filter");
			ImGui::Separator();
			ImGui::Checkbox("Full Body", &Options.Legitbot.Trigger.Rifles.bTriggerFullBody);
			ImGui::Checkbox("Head", &Options.Legitbot.Trigger.Rifles.bTriggerHead);
			ImGui::Checkbox("Chest", &Options.Legitbot.Trigger.Rifles.bTriggerChest);
			ImGui::Checkbox("Stomach", &Options.Legitbot.Trigger.Rifles.bTriggerStomach);
			ImGui::Checkbox("Arms", &Options.Legitbot.Trigger.Rifles.bTriggerArms);
			ImGui::Checkbox("Legs", &Options.Legitbot.Trigger.Rifles.bTriggerLegs);
		}ImGui::EndChild();
		ImGui::SameLine();
	}

	if (Global::LegitSubTab == 3)
	{
		ImGui::BeginChild("Shotguns##aimbot", ImVec2(200, 398), true);
		{
			ImGui::Text("Aimbot - Shotguns");
			ImGui::Separator();
			ImGui::Checkbox("Enable Aimbot", &Options.Legitbot.Aimbot.Shotguns.bEnableAimbot);
			ImGui::SameLine();
			ImGui::Checkbox("On Key", &Options.Legitbot.Aimbot.Shotguns.bOnKey);
			if (Options.Legitbot.Aimbot.Shotguns.bOnKey)
				ImGui::Hotkey("##pistolkey", &Options.Legitbot.Aimbot.Shotguns.iKey, ImVec2(150, 20));
			ImGui::Separator();
			ImGui::Text("Hitbox");
			ImGui::Combo("##hitbox", &Options.Legitbot.Aimbot.Shotguns.iAimbotHitbox, iHitboxLegit, ARRAYSIZE(iHitboxLegit));
			ImGui::Checkbox("Silent Aim", &Options.Legitbot.Aimbot.Shotguns.bSilentAim);
			ImGui::Text("Aimbot Fov");
			ImGui::SliderFloat("Degrees##fov", &Options.Legitbot.Aimbot.Shotguns.iAimbotFov, 0.0, 20, "%.1f");
			ImGui::Text("Aimbot Rcs");
			ImGui::SliderFloat("%##rcs", &Options.Legitbot.Aimbot.Shotguns.flAimbotRecoil, 0, 100, "%.0f", 1.0f);

		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("Shotguns##trigger", ImVec2(200, 398), true);
		{
			ImGui::Text("Trigger - Shotguns");
			ImGui::Separator();
			ImGui::Checkbox("Enable", &Options.Legitbot.Trigger.Shotguns.bEnableTrigger);
			ImGui::SameLine();
			ImGui::Checkbox("On Key", &Options.Legitbot.Trigger.Shotguns.bTriggerOnKey);
			if (Options.Legitbot.Trigger.Shotguns.bTriggerOnKey)
				ImGui::Hotkey("##Shotgunskey", &Options.Legitbot.Trigger.Shotguns.iTriggerKey, ImVec2(150, 20));
			//ImGui::Text("Delay in");
			//ImGui::SliderInt("ms", &Options.Legitbot.Trigger.Shotguns.iTriggerDelay, 10, 250, "%0.f");
			ImGui::Checkbox("Rcs", &Options.Legitbot.Trigger.Shotguns.bTriggerRecoil);
			ImGui::Separator();
			ImGui::Text("Trigger - Filter");
			ImGui::Separator();
			ImGui::Checkbox("Full Body", &Options.Legitbot.Trigger.Shotguns.bTriggerFullBody);
			ImGui::Checkbox("Head", &Options.Legitbot.Trigger.Shotguns.bTriggerHead);
			ImGui::Checkbox("Chest", &Options.Legitbot.Trigger.Shotguns.bTriggerChest);
			ImGui::Checkbox("Stomach", &Options.Legitbot.Trigger.Shotguns.bTriggerStomach);
			ImGui::Checkbox("Arms", &Options.Legitbot.Trigger.Shotguns.bTriggerArms);
			ImGui::Checkbox("Legs", &Options.Legitbot.Trigger.Shotguns.bTriggerLegs);
		}ImGui::EndChild();
		ImGui::SameLine();
	}

	if (Global::LegitSubTab == 4)
	{
		ImGui::BeginChild("Heavy##aimbot", ImVec2(200, 398), true);
		{
			ImGui::Text("Aimbot - Heavy");
			ImGui::Separator();
			ImGui::Checkbox("Enable Aimbot", &Options.Legitbot.Aimbot.Heavy.bEnableAimbot);
			ImGui::SameLine();
			ImGui::Checkbox("On Key", &Options.Legitbot.Aimbot.Heavy.bOnKey);
			if (Options.Legitbot.Aimbot.Heavy.bOnKey)
				ImGui::Hotkey("##pistolkey", &Options.Legitbot.Aimbot.Heavy.iKey, ImVec2(150, 20));
			ImGui::Separator();
			ImGui::Text("Hitbox");
			ImGui::Combo("##hitbox", &Options.Legitbot.Aimbot.Heavy.iAimbotHitbox, iHitboxLegit, ARRAYSIZE(iHitboxLegit));
			ImGui::Checkbox("Silent Aim", &Options.Legitbot.Aimbot.Heavy.bSilentAim);
			ImGui::Text("Aimbot Fov");
			ImGui::SliderFloat("Degrees##fov", &Options.Legitbot.Aimbot.Heavy.iAimbotFov, 0.0, 20, "%.1f");
			ImGui::Text("Aimbot Rcs");
			ImGui::SliderFloat("%##rcs", &Options.Legitbot.Aimbot.Heavy.flAimbotRecoil, 0, 100, "%.0f", 1.0f);

		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("Heavy##trigger", ImVec2(200, 398), true);
		{
			ImGui::Text("Trigger - Heavy");
			ImGui::Separator();
			ImGui::Checkbox("Enable", &Options.Legitbot.Trigger.Heavy.bEnableTrigger);
			ImGui::SameLine();
			ImGui::Checkbox("On Key", &Options.Legitbot.Trigger.Heavy.bTriggerOnKey);
			if (Options.Legitbot.Trigger.Heavy.bTriggerOnKey)
				ImGui::Hotkey("##Heavykey", &Options.Legitbot.Trigger.Heavy.iTriggerKey, ImVec2(150, 20));
			//ImGui::Text("Delay in");
			//ImGui::SliderInt("ms", &Options.Legitbot.Trigger.Heavy.iTriggerDelay, 10, 250, "%0.f");
			ImGui::Checkbox("Rcs", &Options.Legitbot.Trigger.Heavy.bTriggerRecoil);
			ImGui::Separator();
			ImGui::Text("Trigger - Filter");
			ImGui::Separator();
			ImGui::Checkbox("Full Body", &Options.Legitbot.Trigger.Heavy.bTriggerFullBody);
			ImGui::Checkbox("Head", &Options.Legitbot.Trigger.Heavy.bTriggerHead);
			ImGui::Checkbox("Chest", &Options.Legitbot.Trigger.Heavy.bTriggerChest);
			ImGui::Checkbox("Stomach", &Options.Legitbot.Trigger.Heavy.bTriggerStomach);
			ImGui::Checkbox("Arms", &Options.Legitbot.Trigger.Heavy.bTriggerArms);
			ImGui::Checkbox("Legs", &Options.Legitbot.Trigger.Heavy.bTriggerLegs);
		}ImGui::EndChild();
		ImGui::SameLine();
	}

	if (Global::LegitSubTab == 5)
	{
		ImGui::BeginChild("Snipers##aimbot", ImVec2(200, 398), true);
		{
			ImGui::Text("Aimbot - Snipers");
			ImGui::Separator();
			ImGui::Checkbox("Enable Aimbot", &Options.Legitbot.Aimbot.Snipers.bEnableAimbot);
			ImGui::SameLine();
			ImGui::Checkbox("On Key", &Options.Legitbot.Aimbot.Snipers.bOnKey);
			if (Options.Legitbot.Aimbot.Snipers.bOnKey)
				ImGui::Hotkey("##pistolkey", &Options.Legitbot.Aimbot.Snipers.iKey, ImVec2(150, 20));
			ImGui::Separator();
			ImGui::Text("Hitbox");
			ImGui::Combo("##hitbox", &Options.Legitbot.Aimbot.Snipers.iAimbotHitbox, iHitboxLegit, ARRAYSIZE(iHitboxLegit));
			ImGui::Checkbox("Silent Aim", &Options.Legitbot.Aimbot.Snipers.bSilentAim);
			ImGui::Text("Aimbot Fov");
			ImGui::SliderFloat("Degrees##fov", &Options.Legitbot.Aimbot.Snipers.iAimbotFov, 0.0, 20, "%.1f");
			ImGui::Text("Aimbot Rcs");
			ImGui::SliderFloat("%##rcs", &Options.Legitbot.Aimbot.Snipers.flAimbotRecoil, 0, 100, "%.0f", 1.0f);

		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("Snipers##trigger", ImVec2(200, 398), true);
		{
			ImGui::Text("Trigger - Snipers");
			ImGui::Separator();
			ImGui::Checkbox("Enable", &Options.Legitbot.Trigger.Snipers.bEnableTrigger);
			ImGui::SameLine();
			ImGui::Checkbox("On Key", &Options.Legitbot.Trigger.Snipers.bTriggerOnKey);
			if (Options.Legitbot.Trigger.Snipers.bTriggerOnKey)
				ImGui::Hotkey("##sniperskey", &Options.Legitbot.Trigger.Snipers.iTriggerKey, ImVec2(150, 20));
			//ImGui::Text("Delay in");
			//ImGui::SliderInt("ms", &Options.Legitbot.Trigger.Snipers.iTriggerDelay, 10, 250, "%0.f");
			ImGui::Checkbox("Rcs", &Options.Legitbot.Trigger.Snipers.bTriggerRecoil);
			ImGui::Checkbox("Scoped Check", &Options.Legitbot.Trigger.Snipers.bScoped);
			ImGui::Separator();
			ImGui::Text("Trigger - Filter");
			ImGui::Separator();
			ImGui::Checkbox("Full Body", &Options.Legitbot.Trigger.Snipers.bTriggerFullBody);
			ImGui::Checkbox("Head", &Options.Legitbot.Trigger.Snipers.bTriggerHead);
			ImGui::Checkbox("Chest", &Options.Legitbot.Trigger.Snipers.bTriggerChest);
			ImGui::Checkbox("Stomach", &Options.Legitbot.Trigger.Snipers.bTriggerStomach);
			ImGui::Checkbox("Arms", &Options.Legitbot.Trigger.Snipers.bTriggerArms);
			ImGui::Checkbox("Legs", &Options.Legitbot.Trigger.Snipers.bTriggerLegs);
		}ImGui::EndChild();
		ImGui::SameLine();
	}

}

void DrawVisualsTab()
{
	auto& style = ImGui::GetStyle();
	BtnNormal();

	style.ItemSpacing = ImVec2(0, 4);
	style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.f);

	style.WindowPadding = ImVec2(8, 8);
	style.ItemSpacing = ImVec2(4, 4);

	ImGui::BeginChild("ESP - General", ImVec2(220, 398), true);
	{
		ImGui::Text("Player ESP");
		ImGui::Checkbox("Enemy only", &Options.Visuals.Esp.EspFilter.bEnemyOnly);

		ImGui::Checkbox("Bounding box", &Options.Visuals.Esp.EspBox.bBox);
	//	ImGui::ColorEdit3(("##gdfjnvsdhfnvhsdfughsdfsvsdfgsdfg"), &Options.Visuals.Esp.EspBox.MEME);
		ImGui::Checkbox("Health bar", &Options.Visuals.Esp.EspBox.bHealthbar);
		ImGui::Checkbox("Name", &Options.Visuals.Esp.EspMisc.bName);
		ImGui::Checkbox("Flags", &Options.Visuals.Esp.EspMisc.bIsFlashed);
		ImGui::Checkbox("Weapon", &Options.Visuals.Esp.EspMisc.bWeapons);
		if (Options.Visuals.Esp.EspMisc.bWeapons)
		ImGui::Checkbox("All weapons", &Options.Visuals.Esp.EspBox.allweapons);
		ImGui::Checkbox("Ammo", &Options.Visuals.Esp.EspBox.bArmorBar);
		ImGui::Checkbox("Hit marker", &Options.Visuals.VisualsMisc.bHitMarker);
		ImGui::Checkbox("Money", &Options.Visuals.Esp.EspMisc.Money);
		ImGui::Checkbox("Skeleton", &Options.Visuals.Esp.EspBox.bSkeleton);
		ImGui::Checkbox("Dropped weapons", &Options.Visuals.Esp.EspFilter.bDroppedWeapons);
		ImGui::Checkbox("False angles", &Options.Visuals.Esp.EspBox.bYaw);
		//ImGui::Checkbox("Location spam", &Options.Visuals.Esp.EspBox.Glowmeme);
		ImGui::Checkbox("Snap lines", &Options.Visuals.Esp.EspBox.bAimlines);
		ImGui::Checkbox("Bullet trace", &Options.Visuals.Esp.EspBox.Bullet);
		ImGui::Checkbox("AA lines", &Options.Visuals.Esp.EspBox.LBYLINES);
		ImGui::Checkbox("Radar", &Options.Visuals.Esp.EspBox.Radar);
		ImGui::Checkbox("Away from fov", &Options.Visuals.Esp.EspBox.Out);
		//ImGui::Checkbox("Draw multipoints", &Options.Visuals.Esp.EspBox.DrawMultiPoints);

		ImGui::Checkbox("Ghost angle", &Options.Visuals.Chams.bFakeAngleChams);

		ImGui::Checkbox("Glow", &Options.Visuals.Esp.EspBox.Glowmeme);
		ImGui::Checkbox("Entity glow", &Options.Visuals.Esp.EspBox.Glowmeme2);

		ImGui::Checkbox("Backtrack dots", &Options.Visuals.Esp.EspBox.BackTrackTicks);

		if (Options.Ragebot.HvH.bResolver)
		{
		 ImGui::Checkbox("Resolve type", &Options.Visuals.Esp.EspBox.bPitch);
        }



	}ImGui::EndChild();

	ImGui::SameLine();

	ImGui::BeginChild("ESP - Misc", ImVec2(220, 398), true);
	{
		ImGui::Text("Effects");
		ImGui::Checkbox("Spec list", &Options.Visuals.Esp.EspBox.Speclist);
		ImGui::Checkbox("Remove flashbang effects", &Options.Visuals.VisualsWorlds.bNoFlash);
		ImGui::Checkbox("Remove smoke effects", &Options.Visuals.Esp.EspBox.NoSmoke);
		ImGui::Checkbox("Remove visual recoil", &Options.Visuals.VisualsMisc.bNoVisualRecoil);
		ImGui::Checkbox("Remove scope", &Options.Visuals.VisualsMisc.bScopeRemover);
		ImGui::NewLine();
		ImGui::SameLine(28);
		ImGui::Checkbox("Nightmode", &Options.Visuals.VisualsWorlds.bNightMode);
		ImGui::NewLine();
		ImGui::SameLine(28);
		ImGui::Text("Force third person");
		ImGui::NewLine();
		ImGui::SameLine(28);
		ImGui::Combo("##tp", &Options.Visuals.VisualsMisc.iThirdPerson, iThirdperson, ARRAYSIZE(iThirdperson));
		ImGui::Hotkey("##dfgedrtes", &Options.Visuals.VisualsMisc.iThirdPersonKey, ImVec2(150, 20));
		//ImGui::SliderFloat("Thirdperson Modifier:", &Options.Visuals.VisualsMisc.flThirdPersonRange, 0.f, 800.f, "%.0f", 1.0f);
	}ImGui::EndChild();

	ImGui::BeginChild("Visuals - Misc", ImVec2(220, 398), true);
	{
		ImGui::Text("Other ESP");
		ImGui::Checkbox("Crosshair", &Options.Visuals.VisualsMisc.bNormalCrosshair);
		ImGui::Checkbox("Inaccuracy crosshair ", &Options.Visuals.Infobox.Inaccuracy);
		ImGui::Checkbox("Grenade Prediction ", &Options.Visuals.Infobox.Grande);
		ImGui::Checkbox("Autowall crosshair", &Options.Visuals.Esp.EspBox.locksidearrows);

	}ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("Chams", ImVec2(220, 398), true);
	{
		ImGui::Text("Colored models");
		ImGui::Text("	Models");
		ImGui::Checkbox("Enable", &Options.Visuals.Chams.bPlayers);
		{
			ImGui::Separator();
			ImGui::Selectable("Flat", &Options.Visuals.Chams.bFlat);
			ImGui::Selectable("Transparent", &Options.Visuals.Chams.bTransparent);
			ImGui::Selectable("Through Walls", &Options.Visuals.Chams.bThroughWalls);
			ImGui::Selectable("Wireframe", &Options.Visuals.Chams.bWireframe);
			ImGui::Selectable("Enemy Only", &Options.Visuals.Chams.bEnemyOnly);
		}
		ImGui::Text("	Weapons \ Hands");
		ImGui::Checkbox("Enable ", &Options.Visuals.Chams.Weapon);
		{
			ImGui::Separator();
			ImGui::Selectable("Flat ", &Options.Visuals.Chams.bArmsFlat);
			ImGui::Selectable("Transparent ", &Options.Visuals.Chams.bArmsTransparent);
			ImGui::Selectable("Through Walls ", &Options.Visuals.Chams.bArmsThroughWalls);
			ImGui::Selectable("Wireframe ", &Options.Visuals.Chams.bArmsWireframe);
			ImGui::Separator();
		}




	}ImGui::EndChild();

}

void DrawMiscTab()
{
	auto& style = ImGui::GetStyle();
	BtnNormal();

	style.ItemSpacing = ImVec2(0, 4);
	style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.f);

	style.WindowPadding = ImVec2(8, 8);
	style.ItemSpacing = ImVec2(4, 4);

	ImGui::BeginChild("Misc I", ImVec2(220, 398), true);
	{
		ImGui::Text("Misc");
		ImGui::Checkbox("Anti-untrusted", &Options.Misc.Misc2.bAntiUntrusted);
		//ImGui::Checkbox("NasaWalk (Alt)", &Options.Visuals.Esp.EspBox.NasaWalk);

		ImGui::Checkbox("Override FOV", &Options.Misc.Misc2.bEnableFov);
		ImGui::SliderFloat("##fdgsdfgsdfg", &Options.Misc.Misc2.flFovValue, 30.f, 190.f, "%.f", 1.f);
		ImGui::SliderFloat("##fdgsdfgsdfgdfdfdf", &Options.Misc.Misc2.flViewModelValue, 30.f, 190.f, "%.f", 1.f);
		ImGui::Checkbox("Bunny hop", &Options.Misc.Movement.bAutoJump);
		ImGui::Checkbox("Air strafe", &Options.Misc.Movement.bAutoStrafe);
		if (Options.Misc.Movement.bAutoStrafe)
		{
			ImGui::NewLine();
			ImGui::SameLine(28);
			ImGui::Text("Strafe mode");
			ImGui::NewLine();
			ImGui::SameLine(28);
			ImGui::Combo("##strmd", &Options.Misc.Movement.iAutoStrafeMode, iAutostrafer, ARRAYSIZE(iAutostrafer));
		}
		ImGui::Checkbox("Knifebot", &Options.Misc.Misc2.bKnifeBot);
		ImGui::Checkbox("Check if antiaim is crocked", &Options.Visuals.Infobox.bInfoLby);
		ImGui::Checkbox("Clan tag spammer", &Options.Visuals.Infobox.bClantag);
		ImGui::Checkbox("Event log", &Options.Visuals.Infobox.InfoECHO);
		ImGui::Checkbox("Rank reveal", &Options.Visuals.VisualsMisc.bRankReveal);

		//ImGui::Checkbox("Stats window", &Options.Visuals.Infobox.Stats);


	}ImGui::EndChild();
	/*ImGui::SameLine();
	ImGui::BeginChild("Misc II", ImVec2(190, 398), true);
	{
		ImGui::Text("Misc II");
		ImGui::Separator();
		ImGui::Checkbox("Anti-Untrusted", &Options.Misc.Misc2.bAntiUntrusted);
		ImGui::Checkbox("KnifeBot", &Options.Misc.Misc2.bKnifeBot);
 	ImGui::Checkbox("Auto buy", &Options.Misc.Misc2.Autobuy);
	if (Options.Misc.Misc2.Autobuy)
	{
		ImGui::Combo("", &Options.Misc.Misc2.iAutoBuy, iAutoBuy, ARRAYSIZE(iAutoBuy));
	}
	ImGui::Checkbox("buy list", &Options.Misc.Misc2.buylist);

	}ImGui::EndChild();	
	ImGui::SameLine();
	ImGui::BeginChild("Movements", ImVec2(190, 398), true);
	{
		ImGui::Text("Movement");
		ImGui::Separator();
		if (Options.Misc.Movement.bAutoStrafe)
		{
			ImGui::Separator();
			ImGui::Text("Strafe Mode");
			ImGui::Combo("##strmd", &Options.Misc.Movement.iAutoStrafeMode, iAutostrafer, ARRAYSIZE(iAutostrafer));
			ImGui::Checkbox("Z-Axis Strafe", &Options.Misc.Movement.bZStrafe);
			ImGui::Hotkey("Key##zstrafekey", &Options.Misc.Movement.iZStrafeKey, ImVec2(150, 20));
			ImGui::Text("Circle Strafer");
			ImGui::Combo("Circle Strafer", &Options.Misc.Movement.iCircleStraferMode, iCircleStrafer, ARRAYSIZE(iCircleStrafer));
			ImGui::Hotkey("Key##cskey", &Options.Misc.Movement.iCircleStraferKey, ImVec2(150, 20));
			ImGui::Separator();
		}
		ImGui::Separator();
		ImGui::Checkbox("Slowmotion", &Options.Misc.Movement.bSlowmotion);
		ImGui::Hotkey("Key##slowmotionkey", &Options.Misc.Movement.iSlowMotionKey, ImVec2(150, 20));
		ImGui::Text("Airstuck");
		ImGui::Combo("##airstuck", &Options.Misc.Movement.iAirStuck, iAirstuck, ARRAYSIZE(iAirstuck));
		ImGui::Hotkey("##key", &Options.Misc.Movement.iAirStuckKey, ImVec2(150, 20));
		ImGui::Checkbox("Walkbot", &Options.Misc.Movement.bWalkBot);
		ImGui::NewLine();
		ImGui::SameLine(28);
		ImGui::Checkbox("Bunnyhop", &Options.Misc.Movement.bBunnyhop);
		ImGui::Text("Distance to Wall");
		ImGui::SliderFloat("%##float", &Options.Misc.Movement.flDistance, 0, 500, "%.1f", (1.0f));
		ImGui::Text("Degrees Turn");
		ImGui::SliderInt("%##int", &Options.Misc.Movement.iTurn, -180, 180, "%.1f");
		ImGui::Separator();
	}ImGui::EndChild();*/
	ImGui::SameLine();
	ImGui::BeginChild("cfg", ImVec2(190, 398), true);
	{
		ImGui::Text("Presets");
		ImGui::Combo("##gjfbjvfnvjfdhguyuretrsfvsdfvsdfg", &Options.Misc.Movement.Configsss, Configsss, ARRAYSIZE(Configsss));
		if (ImGui::Button("Save"))
			Config->Save();
		ImGui::SameLine();
		if (ImGui::Button("Load"))
			Config->Load();
	}ImGui::EndChild();
}

void DrawColorsTab()
{
	auto& style = ImGui::GetStyle();
	BtnNormal();

	style.ItemSpacing = ImVec2(0, 4);
	style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.f);

	style.WindowPadding = ImVec2(8, 8);
	style.ItemSpacing = ImVec2(4, 4);
	/*ImGui::BeginChild("COLORS", ImVec2(772, 434), true);
	{

		ImGui::Text("Box - color");
		ImGui::SliderFloat("R ", &Options.Ragebot.Accuracy.BoxR, 0.f, 255.f, "%.f");
		ImGui::SliderFloat("G ", &Options.Ragebot.Accuracy.BoxG, 0.f, 255.f, "%.f");
		ImGui::SliderFloat("B ", &Options.Ragebot.Accuracy.BoxB, 0.f, 255.f, "%.f");
		ImGui::Separator();
		ImGui::Text("Ammo - color");
		ImGui::SliderFloat("R ", &Options.Col.AmmoR, 0.f, 255.f, "%.f");
		ImGui::SliderFloat("G ", &Options.Col.AmmoG, 0.f, 255.f, "%.f");
		ImGui::SliderFloat("B ", &Options.Col.AmmoB, 0.f, 255.f, "%.f");
		ImGui::Separator();

	}ImGui::EndChild();*/

	ImGui::BeginChild("Box - Color", ImVec2(190, 100), true);
	{
		ImGui::Text("Box - Color");
		ImGui::ColorEdit3("##ESP", Options.Visuals.Esp.EspBox.MEME, 1 << 7);

	}ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("Ammo - Color", ImVec2(190, 100), true);
	{
		ImGui::Text("Ammo - Color");
		ImGui::ColorEdit3("##ESP", Options.Visuals.Esp.EspBox.MEME1, 1 << 7);

	}ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("Dropped Weapons", ImVec2(190, 100), true);
	{
		ImGui::Text("Dropped Weapons");
		ImGui::ColorEdit3("##ESP", Options.Visuals.Esp.EspBox.MEME2, 1 << 7);

	}ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("BOMB - Color", ImVec2(190, 100), true);
	{
		ImGui::Text("Bomb");
		ImGui::ColorEdit3("##ESP", Options.Visuals.Esp.EspBox.MEME3, 1 << 7);

	}ImGui::EndChild();
	ImGui::BeginChild("Model Chams", ImVec2(190, 100), true);
	{
		ImGui::Text("Chams Visible");
		ImGui::ColorEdit3("##ESP", Options.Visuals.Esp.EspBox.MEME4, 1 << 7);

	}ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("Chams InVisible", ImVec2(190, 100), true);
	{
		ImGui::Text("Chams InVisible");
		ImGui::ColorEdit3("##ESP", Options.Visuals.Esp.EspBox.MEME5, 1 << 7);

	}ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("Hands\Weapon Chams InVisible - Color", ImVec2(190, 100), true);
	{
		ImGui::Text("Hands\Weapon");
		ImGui::ColorEdit3("##ESP", Options.Visuals.Esp.EspBox.MEME6, 1 << 7);

	}ImGui::EndChild();
}
void Skins()
{
	auto& style = ImGui::GetStyle();
	BtnNormal();

	style.ItemSpacing = ImVec2(0, 4);
	style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.f);

	style.WindowPadding = ImVec2(8, 8);
	style.ItemSpacing = ImVec2(4, 4);
	/*ImGui::BeginChild("COLORS", ImVec2(772, 434), true);
	{

	ImGui::Text("Box - color");
	ImGui::SliderFloat("R ", &Options.Ragebot.Accuracy.BoxR, 0.f, 255.f, "%.f");
	ImGui::SliderFloat("G ", &Options.Ragebot.Accuracy.BoxG, 0.f, 255.f, "%.f");
	ImGui::SliderFloat("B ", &Options.Ragebot.Accuracy.BoxB, 0.f, 255.f, "%.f");
	ImGui::Separator();
	ImGui::Text("Ammo - color");
	ImGui::SliderFloat("R ", &Options.Col.AmmoR, 0.f, 255.f, "%.f");
	ImGui::SliderFloat("G ", &Options.Col.AmmoG, 0.f, 255.f, "%.f");
	ImGui::SliderFloat("B ", &Options.Col.AmmoB, 0.f, 255.f, "%.f");
	ImGui::Separator();

	}ImGui::EndChild();*/

	ImGui::BeginChild("Skin options", ImVec2(220, 280), true);
	{
		ImGui::Text("Skin options");
		if (Options.Skins.bEnableSkins)
		{
			ImGui::Checkbox("Enabled", &Options.Skins.bEnableSkins);
		}
		else
		{
			ImGui::Checkbox("Enable", &Options.Skins.bEnableSkins);

		}
		ImGui::Checkbox("StatTrak", &Options.Skins.startrak);
		if (Options.Skins.startrak)
			ImGui::InputInt(XorStr18("##gfkjdfkgjsdfkvjdsf"), &Options.Skins.stat);
	//	ImGui::Checkbox("Custom name", &Options.Skins.CustomName);
	//	if (Options.Skins.CustomName)
		//	ImGui::InputText(XorStr("Name"), Options.Skins.ChangerName, 32);



		ImGui::Text("Models - rifles");
		ImGui::Text("AK-47");
		ImGui::InputInt(XorStr("ID##sdfgsdfvsdfgsdfg"), &Options.Skins.ChangerSkin);
		ImGui::Text("M4A1-S");
		ImGui::InputInt(XorStr1("ID##dsgfhsdgbsdgsdfgsdfgsdfg"), &Options.Skins.ChangerSkin1);
		ImGui::Text("M4A4");
		ImGui::InputInt(XorStr2("ID##dfgsdfvsdfgsdfvsfdgtrewt"), &Options.Skins.ChangerSkin2);
		ImGui::Text("AUG");
		ImGui::InputInt(XorStr3("ID##dfsgsvsdfgsdfvsdfgewrt"), &Options.Skins.ChangerSkin3);
		ImGui::Text("FAMAS");
		ImGui::InputInt(XorStr4("ID##dsafwsert2345dfsf"), &Options.Skins.ChangerSkin4);
		ImGui::Text("GALIL");
		ImGui::InputInt(XorStr5("ID##fdgsdfvsdfhgteyewrtqwrefsxv"), &Options.Skins.ChangerSkin5);
		ImGui::Text("SG553");
		ImGui::InputInt(XorStr6("ID##sdfgsvsdfgsdfhsdrtewrtqw"), &Options.Skins.ChangerSkin6);
		ImGui::Text("Models - snipers");
		ImGui::Text("AWP");
		ImGui::InputInt(XorStr7("ID##fgsdfvtrhukmrtyh"), &Options.Skins.ChangerSkin7);
		ImGui::Text("SSG08");
		ImGui::InputInt(XorStr8("ID##gklfmryyfvsrfg"), &Options.Skins.ChangerSkin8);
		ImGui::Text("SCAR-20");
		ImGui::InputInt(XorStr9("ID##gjfnfhvyjrejfmnbdg"), &Options.Skins.ChangerSkin9);
		ImGui::Text("G3SG1");
		ImGui::InputInt(XorStr10("ID##bdsghgdsuhtryqwecxzxc"), &Options.Skins.ChangerSkin10);
		ImGui::Text("Models - pistols");
		ImGui::Text("GLOCK");
		ImGui::InputInt(XorStr11("ID##cxcxcxsadsdf"), &Options.Skins.ChangerSkin11);
		ImGui::Text("USP-S");
		ImGui::InputInt(XorStr12("ID##fcvxcvcxvxcv"), &Options.Skins.ChangerSkin12);
		ImGui::Text("Deagle");
		ImGui::InputInt(XorStr13("ID##fbvvbxcvndfg"), &Options.Skins.ChangerSkin13);
		ImGui::Text("DUALS");
		ImGui::InputInt(XorStr14("ID##ghbdghjsdyhweyt"), &Options.Skins.ChangerSkin14);
		ImGui::Text("REVOLVER");
		ImGui::InputInt(XorStr15("ID##fbllgkpptifgvdfbv"), &Options.Skins.ChangerSkin15);
		ImGui::Text("FIVE-SEVEN");
		ImGui::InputInt(XorStr16("ID##jgfnfhifutmb"), &Options.Skins.ChangerSkin16);
		ImGui::Text("P2000");
		ImGui::InputInt(XorStr17("ID##gmgbhjjsdfghbsdghf"), &Options.Skins.ChangerSkin17);
		ImGui::Text("P250");
		ImGui::InputInt(XorStr18("ID##kodgjidfgufdghfgdfg"), &Options.Skins.ChangerSkin18);
		//ImGui::InputText(XorStr("Name"), Options.Skins.ChangerName, 32);
		//ImGui::InputInt(XorStr("Skin"), &Options.Skins.ChangerSkin);


	}ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("Glove options", ImVec2(220, 280), true);
	{
		ImGui::Text("Glove options");
		if (Options.Skins.GloveOver)
		{
			ImGui::Checkbox("Enabled", &Options.Skins.GloveOver);
		}
		else
		{
			ImGui::Checkbox("Enable", &Options.Skins.GloveOver);

		}

		ImGui::Combo("##dfgvsdfgewrtsfdgsdfg", &Options.Skins.Glovemodel, GloveModel, ARRAYSIZE(GloveModel));
		ImGui::Combo("##glovejkfdksdfgsdfvsdfgsdfg", &Options.Skins.Gloveskin, Gloveskin, ARRAYSIZE(Gloveskin));


	}ImGui::EndChild();
	ImGui::BeginChild("Knife options", ImVec2(220, 280), true);
	{
		ImGui::Text("Knife options");
		if (Options.Skins.knife)
		{
			ImGui::Checkbox("Enabled", &Options.Skins.knife);
		}
		else
		{
			ImGui::Checkbox("Enable", &Options.Skins.knife);

		}
		ImGui::Combo("##dfgvsdfgewrtsfdgsdfg", &Options.Skins.Knifemodels, Knife, ARRAYSIZE(Knife));
		ImGui::InputInt(XorStr18("ID##bgdshewrtsfdgsdfg"), &Options.Skins.ChangerSkin19);


	}ImGui::EndChild();

}
void LegitBot()
{
	auto& style = ImGui::GetStyle();
	BtnNormal();

	style.ItemSpacing = ImVec2(0, 4);
	style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.f);

	style.WindowPadding = ImVec2(8, 8);
	style.ItemSpacing = ImVec2(4, 4);
	/*ImGui::BeginChild("COLORS", ImVec2(772, 434), true);
	{

	ImGui::Text("Box - color");
	ImGui::SliderFloat("R ", &Options.Ragebot.Accuracy.BoxR, 0.f, 255.f, "%.f");
	ImGui::SliderFloat("G ", &Options.Ragebot.Accuracy.BoxG, 0.f, 255.f, "%.f");
	ImGui::SliderFloat("B ", &Options.Ragebot.Accuracy.BoxB, 0.f, 255.f, "%.f");
	ImGui::Separator();
	ImGui::Text("Ammo - color");
	ImGui::SliderFloat("R ", &Options.Col.AmmoR, 0.f, 255.f, "%.f");
	ImGui::SliderFloat("G ", &Options.Col.AmmoG, 0.f, 255.f, "%.f");
	ImGui::SliderFloat("B ", &Options.Col.AmmoB, 0.f, 255.f, "%.f");
	ImGui::Separator();

	}ImGui::EndChild();*/

	ImGui::BeginChild("Skin options", ImVec2(220, 280), true);
	{
		ImGui::Text("Legitbot");
		ImGui::Checkbox("No Recoil", &Options.Skins.NoRecoil);
		ImGui::Checkbox("Legit Backtrack (5 ticks)", &Options.Ragebot.Aimbot.LegitBacktrack);


	}ImGui::EndChild();

}

HRESULT __stdcall Hooked_EndScene(IDirect3DDevice9* pDevice)
{
	static ConVar* convar;
	//VMTD3D.ReHook();

	if (!Global::MenuInit)
	{
		Gui_Init(pDevice);
		convar = Interfaces.g_ICVars->FindVar("cl_mouseenable");
	}
	ImGui::GetIO().MouseDrawCursor = Global::MenuOpen;

	ImGui_ImplDX9_NewFrame();
	auto& style = ImGui::GetStyle();
	int W, H;
	Interfaces.pEngine->GetScreenSize(W, H);

	DefineTab();

	if (Global::MenuOpen)
	{
		if (style.Alpha > 1.f)
			style.Alpha = 1.f;
		else if (style.Alpha != 1.f)
			style.Alpha += 0.01f;

		style.WindowMinSize = ImVec2(0, 0);
		ImGui::PushFont(Global::fDefault);
		int w;
		int h;
		ImVec2 mainWindowPos; // wir brauchen die main window pos um nen vernünftigen border zu drawen

		Interfaces.pEngine->GetScreenSize(w, h);

		//Erstmal drawen wir das main menu




		// Als Border machen wir ein 3-lagigen: farbe-grau



		/*ImGui::Begin("Hurensohn Visual Studio", &Global::MenuOpen, ImVec2(250, 250), 0.98f, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
		{
			mainWindowPos = ImGui::GetWindowPos();

			if (Global::MenuTab == 0)
				DrawRageTab();
			if (Global::MenuTab == 1)
				DrawVisualsTab();
			if (Global::MenuTab == 2)
				DrawMiscTab();
			if (Global::MenuTab == 3)
			DrawColorsTab();
			if (Global::MenuTab == 4)
				Skins();
		}ImGui::End();


		// So jetzt die tabs


		if (Global::MenuTab == 0)
		ImGui::SetNextWindowPos(ImVec2(mainWindowPos.x + 250, mainWindowPos.y - 80));
		else
	    ImGui::SetNextWindowPos(ImVec2(mainWindowPos.x + 250, mainWindowPos.y - 50));

		style.WindowPadding = ImVec2(0, 0);
		style.ItemSpacing = ImVec2(4, 0);
		ImGui::SetNextWindowSize(ImVec2(788, 42));
		ImGui::Begin("##tabarea", &Global::MenuOpen, ImVec2(788, 42), 0.f, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
		{
			if (Global::MenuTab == 0) BtnActive(); else BtnNormal();
			if (ImGui::Button("Ragebot", ImVec2(70, 30))) Global::MenuTab = 0;
			ImGui::SameLine();
			if (Global::MenuTab == 1) BtnActive(); else BtnNormal();
			if (ImGui::Button("Visuals", ImVec2(70, 30))) Global::MenuTab = 1;
			ImGui::SameLine();
			if (Global::MenuTab == 2) BtnActive(); else BtnNormal();
			if (ImGui::Button("Misc", ImVec2(70, 30))) Global::MenuTab = 2;
			ImGui::SameLine();
			if (Global::MenuTab == 3) BtnActive(); else BtnNormal();
			if (ImGui::Button("Colors", ImVec2(70, 30))) Global::MenuTab = 3;
			ImGui::SameLine();
			if (Global::MenuTab == 4) BtnActive(); else BtnNormal();
			if (ImGui::Button("Skins", ImVec2(70, 30))) Global::MenuTab = 4;
			// lila



		}ImGui::End();
		ImGui::SetNextWindowPos(ImVec2(mainWindowPos.x + 250, mainWindowPos.y - 40));
		style.WindowPadding = ImVec2(0, 0);
		style.ItemSpacing = ImVec2(4, 0);
		ImGui::SetNextWindowSize(ImVec2(788, 42));
		ImGui::Begin("##tabarea2", &Global::MenuOpen, ImVec2(788, 42), 0.f, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
		{
			if (Global::MenuTab == 0)
				if (ImGui::Button("Aimbot", ImVec2(70, 30))) Global::RageSubTab = 0;
				ImGui::SameLine();
			if (Global::MenuTab == 0)
				if (ImGui::Button("Anti-aim", ImVec2(70, 30))) Global::RageSubTab = 1;
			// lila


		}ImGui::End();*/

		ImGui::SetNextWindowSize(ImVec2(900, 400));
		ImGui::Begin("Main Menu", &Global::MenuOpen, ImVec2(900, 400), 1.f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders |ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
		{	

			ImGui::Columns(2, nullptr, false);
			ImGui::SetColumnOffset(1, 230);
			style.ItemSpacing = ImVec2(0.f, 0.f);
			ImGui::BeginChild("##tabs", ImVec2(300, 400), false);
			{			

				style.Colors[ImGuiCol_Button] = Global::MenuTab == 0 ? ImColor(44, 43, 44) : ImColor(18, 18, 18, 255);
				TextColor(rage);
				if (ImGui::Button("Ragebot", ImVec2(200, 45))) Global::MenuTab = 0;
				if (Global::MenuTab == 0)
				style.Colors[ImGuiCol_Button] = Global::RageSubTab == 0 ? ImColor(44, 43, 44) : ImColor(18, 18, 18, 255);
				if (Global::MenuTab == 0)
				if (ImGui::Button("Aimbot", ImVec2(200, 45))) Global::RageSubTab = 0;
				if (Global::MenuTab == 0)
					style.Colors[ImGuiCol_Button] = Global::RageSubTab == 1 ? ImColor(44, 43, 44) : ImColor(18, 18, 18, 255);
				if (Global::MenuTab == 0)
					if (ImGui::Button("Anti aimbot", ImVec2(200, 45))) Global::RageSubTab = 1;
				TextColor(legit);

				style.Colors[ImGuiCol_Button] = Global::MenuTab == 5 ? ImColor(44, 43, 44) : ImColor(18, 18, 18, 255);
				if (ImGui::Button("Legitbot", ImVec2(200, 45))) Global::MenuTab = 5;
				TextColor(visuals);

				style.Colors[ImGuiCol_Button] = Global::MenuTab == 1 ? ImColor(44, 43, 44) : ImColor(18, 18, 18, 255);
				if (ImGui::Button("Visuals", ImVec2(200, 45))) Global::MenuTab = 1;
				TextColor(misc);

				style.Colors[ImGuiCol_Button] = Global::MenuTab == 2 ? ImColor(44, 43, 44) : ImColor(18, 18, 18, 255);
				if (ImGui::Button("Misc", ImVec2(200, 45))) Global::MenuTab = 2;
				TextColor(colors);

				style.Colors[ImGuiCol_Button] = Global::MenuTab == 3 ? ImColor(44, 43, 44) : ImColor(18, 18, 18, 255);
				if (ImGui::Button("Colors", ImVec2(200, 45))) Global::MenuTab = 3;
				TextColor(skins);

				style.Colors[ImGuiCol_Button] = Global::MenuTab == 4 ? ImColor(44, 43, 44) : ImColor(18, 18, 18, 255);
				if (ImGui::Button("Skins", ImVec2(200, 45))) Global::MenuTab = 4;
			}ImGui::EndChild();

			ImGui::NextColumn();

	
			ImGui::BeginChild("##main", ImVec2(652, 400), true);
			{
				switch (Global::MenuTab)
				{
				case 0: //aimbot
					if (Global::MenuTab == 0)
						DrawRageTab();
				case 1: //aimbot
					if (Global::MenuTab == 1)
						DrawVisualsTab();
				case 2: //aimbot
					if (Global::MenuTab == 2)
						DrawMiscTab();
				case 3: //aimbot
					if (Global::MenuTab == 3)
						DrawColorsTab();
				case 4: //aimbot
					if (Global::MenuTab == 4)
						Skins();
				case 5: //aimbot
					if (Global::MenuTab == 5)
						LegitBot();
				}
			}ImGui::EndChild();
		}
		ImGui::End();

		if (Global::MenuTab == 0)
		{
			if (Global::curLil > 0)
				Global::curLil -= 2;
		}
		else if (Global::MenuTab == 1)
		{
			if (Global::curLil > 132)
				Global::curLil -= 2;
			else if (Global::curLil < 132)
				Global::curLil += 2;
		}
		else if (Global::MenuTab == 2)
		{
			if (Global::curLil > 264)
				Global::curLil -= 2;
			else if (Global::curLil < 264)
				Global::curLil += 2;
		}
		else if (Global::MenuTab == 3)
		{
			if (Global::curLil > 396)
				Global::curLil -= 2;
			else if (Global::curLil < 396)
				Global::curLil += 2;
		}



		//background

	}
	else
	{
		if (style.Alpha < 0.f)
			style.Alpha = 0.f;
		else if (style.Alpha != 0.f)
			style.Alpha -= 0.01f;
	}
	if (Options.Visuals.Infobox.Stats)
	{
		style.Alpha = 1.f;
		ImGui::PushFont(Global::fDefault);
		int lbyred = 0;
		int lbygreen = 0;
		style.WindowMinSize = ImVec2(210, 40);
		style.WindowPadding = ImVec2(8, 8);
		style.ItemSpacing = ImVec2(4, 4);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.95f);
		ImGui::Begin("Player Stats", &Options.Visuals.Infobox.bInfoBox, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders);
		{
			if (Interfaces.pEngine->IsInGame() && Interfaces.pEngine->IsConnected() && Hacks.LocalPlayer->isAlive())
			{
				int Kills = Hacks.LocalPlayer->Kills();
				int Kills1 = Hacks.LocalPlayer->Asist();
				int Kills2 = Hacks.LocalPlayer->death();

					ImGui::TextColored(ImVec4(0.97, 0.97, 0, 1), "Kills : %i", Kills);
					ImGui::TextColored(ImVec4(0.97, 0.97, 0, 1), "Assists : %i", Kills1);
					ImGui::TextColored(ImVec4(0.97, 0.97, 0, 1), "Deaths : %i", Kills2);



			}
		}ImGui::End();
	}

	if (Options.Visuals.Infobox.bInfoBox)
	{
		style.Alpha = 1.f;
		ImGui::PushFont(Global::fDefault);
		int lbyred = 0;
		int lbygreen = 0;
		style.WindowMinSize = ImVec2(210, 40);
		style.WindowPadding = ImVec2(8, 8);
		style.ItemSpacing = ImVec2(4, 4);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.95f);
		ImGui::Begin("Infobox", &Options.Visuals.Infobox.bInfoBox, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders);
		{
			if (Interfaces.pEngine->IsInGame() && Interfaces.pEngine->IsConnected() && Hacks.LocalPlayer->isAlive())
			{
				if (!(Hacks.CurrentCmd->viewangles.y - Hacks.LocalPlayer->pelvisangs() >= -35 && Hacks.CurrentCmd->viewangles.y - Hacks.LocalPlayer->pelvisangs() <= 35))
				{
					lbygreen = 1;
					lbyred = 0;
				}
				else
				{
					lbyred = 1;
					lbygreen = 0;
				}

				if (Options.Visuals.Infobox.bInfoVelocity)
				{
					int velocity = Hacks.LocalPlayer->GetVecVelocity().Length2D();
					ImGui::TextColored(ImVec4(0.97, 0.97, 0, 1), "Velocity: %i", velocity);
				}

				/*if (Options.Visuals.Infobox.bInfoAngles)
				{
					int anglereal = Global::curReal;
					int anglefake = Global::curFake;
					int lby = Hacks.LocalPlayer->pelvisangs();
					int pitch = Hacks.LocalPlayer->GetEyeAngles().x;
					bool sendpacket = Hacks.SendPacket;

					ImGui::Text("LBY:"); ImGui::SameLine();
					ImGui::TextColored(ImVec4(0, 0.97, 0, 1), "%i", lby);
					ImGui::SameLine();

					ImGui::Text("Real:"); ImGui::SameLine();
					ImGui::TextColored(ImVec4(0, 0.97, 0, 1), "%i", anglereal);
					ImGui::SameLine();

					ImGui::Text("Fake:"); ImGui::SameLine();
					ImGui::TextColored(ImVec4(0, 0.97, 0, 1), "%i", anglefake);

					ImGui::Text("Pitch:"); ImGui::SameLine();
					ImGui::TextColored(ImVec4(0, 0.97, 0, 1), "%i", pitch);

					ImGui::Text("Packet Status: ");
					ImGui::SameLine();
					if (sendpacket)
						ImGui::TextColored(ImVec4(0.97, 0, 0, 1.f), "Sending");
					else
						ImGui::TextColored(ImVec4(0, 0.97, 0, 1.f), "Choking");

				}*/

			}
		}ImGui::End();
	}


	if (Options.Visuals.Esp.EspBox.Radar)
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec2 oldPadding = style.WindowPadding;
		float oldAlpha = style.Colors[ImGuiCol_WindowBg].w;
		style.WindowPadding = ImVec2(0, 0);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.95f);
		ImGui::Begin(XorStr("Radar"), &Options.Visuals.Esp.EspBox.Radar, ImVec2(200, 200), 0.4F, ImGuiWindowFlags_NoCollapse| ImGuiWindowFlags_ShowBorders);
		{
			ImVec2 siz = ImGui::GetWindowSize();
			ImVec2 pos = ImGui::GetWindowPos();

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

						bool viewCheck = false;
						Vector EntityPos = MEME(pBaseEntity->GetAbsOrigin(), LocalPos, pos.x, pos.y, siz.x, siz.y, ang.y, 2, &viewCheck);

						//if (false)
						//isVisibled = false;
						//ImU32 clr = (bIsEnemy ? (isVisibled ? Color::LightGreen() : Color::Blue()) : Color::White()).GetU32();
						//	ImU32 clr = /*bIsEnemy ? (isVisibled ? *///Color::White().GetU32();// : Color::Black()) : Color::Blue()).GetU32();

						//if (false)
						//	continue;

						int s = 4;
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
	}
	/*if (Options.Visuals.VisualsMisc.bSpecList)
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
		int specs = 0;
		std::string spect = "";
		ImGui::PushFont(Global::fDefault);
		if (Interfaces.pEngine->IsInGame() && Interfaces.pEngine->IsConnected())
		{
			int localindex = Interfaces.pEngine->GetLocalPlayer();
			if (Hacks.LocalPlayer)
			{
				for (int i = 0; i < Interfaces.pEngine->GetMaxClients(); i++)
				{
					CBaseEntity* pEntity = Interfaces.pEntList->GetClientEntity(i);
					if (!pEntity)
						continue;
					if (pEntity->GetHealth() > 0)
						continue;
					if (pEntity == Hacks.LocalPlayer)
						continue;
					if (pEntity->IsDormant())
						continue;
					if (pEntity->GetObserverTarget() != Hacks.LocalPlayer)
						continue;

					player_info_t pInfo;
					Interfaces.pEngine->GetPlayerInfo(pEntity->GetIndex(), &pInfo);
					if (pInfo.ishltv)
						continue;

					spect += pInfo.name;
					spect += "\n";
					specs++;

				}

				style.WindowMinSize = ImVec2(210, 40);
				style.WindowPadding = ImVec2(8, 8);
				style.ItemSpacing = ImVec2(4, 4);
				style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.95f);
				ImGui::Begin("", &Options.Visuals.Infobox.bInfoBox, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
				{
                    ImGui::Text("Spectators");
					ImVec2 siz = ImGui::CalcTextSize(spect.c_str());
					ImGui::SetWindowSize(ImVec2(200, 20 + siz.y));
					ImGui::Text(spect.c_str());
				}ImGui::End();
			}
		}
	}*/

	/*if (Options.Visuals.Infobox.bPlayerBox)
	{
		style.Alpha = 1.f;
		ImGui::PushFont(Global::fDefault);
		int lbyred = 0;
		int lbygreen = 0;
		style.WindowMinSize = ImVec2(210, 40);
		style.WindowPadding = ImVec2(8, 8);
		style.ItemSpacing = ImVec2(4, 4);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.95f);
		if (Interfaces.pEngine->IsInGame() && Interfaces.pEngine->IsConnected() && Hacks.LocalPlayer->isAlive())
			if (ImGui::Begin("Playerbox", &Options.Visuals.Infobox.bPlayerBox, ImVec2(200, 250), 1.0f, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoResize))
			{
				ImDrawList* windowDrawList = ImGui::GetWindowDrawList();

				ImVec2 siz = ImGui::GetWindowSize();
				ImVec2 pos = ImGui::GetWindowPos();
				CBaseCombatWeapon* LocalWeapon;

				float accuracy = Hacks.LocalPlayer->GetActiveBaseCombatWeapon->UpdateAccuracyPenalty() * 3000.f;
				if (accuracy > 179)
					accuracy = 179;

				int iSpeed = 0;
				Vector vSpeed = Hacks.LocalPlayer->GetVecVelocity();
				iSpeed = (int)vSpeed.Length2D();

				int iSpeed2 = 0;
				Vector vSpeed2 = Hacks.LocalPlayer->GetVecVelocity();
				iSpeed2 = (int)vSpeed2.Length2D() - 300;

				float flPunch = 0.0f;
				Vector vRecoil = Hacks.LocalPlayer->GetPunchAngle();
				flPunch = (float)vRecoil.Length2D();

				if (iSpeed < 300)
					windowDrawList->AddText(ImVec2(pos.x + 25, pos.y + 1 + 217), Color(255, 255, 255, 255).GetU32(), "Speed");
				if (iSpeed > 300)
					windowDrawList->AddText(ImVec2(pos.x + 25, pos.y + 1 + 217), Color(255, 0, 0, 255).GetU32(), "Boost");
				windowDrawList->AddText(ImVec2(pos.x + 85, pos.y + 1 + 217), Color(255, 255, 255, 255).GetU32(), "Recoil");
				windowDrawList->AddText(ImVec2(pos.x + 145, pos.y + 1 + 217), Color(255, 255, 255, 255).GetU32(), "Spread");

				windowDrawList->AddRect(ImVec2(pos.x + 147, pos.y + 35), ImVec2(pos.x + 147 + 25, pos.y + 35 + 180), Color(0, 0, 0, 255).GetU32());
				windowDrawList->AddRect(ImVec2(pos.x + 87, pos.y + 35), ImVec2(pos.x + 87 + 25, pos.y + 35 + 180), Color(0, 0, 0, 255).GetU32());
				windowDrawList->AddRect(ImVec2(pos.x + 28, pos.y + 35), ImVec2(pos.x + 28 + 25, pos.y + 35 + 180), Color(0, 0, 0, 255).GetU32());

				if (iSpeed < 300)
					windowDrawList->AddRectFilled(ImVec2(pos.x + 28, pos.y + 35 + 180 - (iSpeed * 0.6)), ImVec2(pos.x + 28 + 25, pos.y + 35 + 180), Color(255, 255, 255, 255).GetU32());

				if (iSpeed > 300)
					windowDrawList->AddRectFilled(ImVec2(pos.x + 28, pos.y + 35 + 180 - (iSpeed2 * 0.2)), ImVec2(pos.x + 28 + 25, pos.y + 35 + 180), Color(255, 0, 0, 255).GetU32());

				windowDrawList->AddRectFilled(ImVec2(pos.x + 147, pos.y + 35 + 180 - accuracy), ImVec2(pos.x + 147 + 25, pos.y + 35 + 180), Color(255, 255, 255, 255).GetU32());
				windowDrawList->AddRectFilled(ImVec2(pos.x + 87, pos.y + 35 + 180 - flPunch * 30), ImVec2(pos.x + 87 + 25, pos.y + 35 + 180), Color(255, 255, 255, 255).GetU32());
				ImGui::End();
			}
	}*/

	ImGui::Render();
	return Hacks.oEndScene(pDevice);
}


HRESULT __stdcall Hooked_Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresParam)
{
	if (!Global::MenuInit)
		return Hacks.oResetScene(pDevice, pPresParam);

	ImGui_ImplDX9_InvalidateDeviceObjects();

	return Hacks.oResetScene(pDevice, pPresParam);

	ImGui_ImplDX9_CreateDeviceObjects();

	return Hacks.oResetScene(pDevice, pPresParam);
}
