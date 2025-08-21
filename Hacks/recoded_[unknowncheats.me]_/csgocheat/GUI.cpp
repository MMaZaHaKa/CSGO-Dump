#include "GUI.h"
#include <array>
#include <cmath>
static Color Red() { return Color(255, 0, 0); }
static Color Green() { return Color(0, 255, 0); }
static Color Blue() { return Color(0, 0, 255); }
static Color LightBlue() { return Color(100, 100, 255); }
static Color Grey() { return Color(128, 128, 128); }
static Color DarkGrey() { return Color(45, 45, 45); }
static Color Black() { return Color(0, 0, 0); }
static Color White() { return Color(255, 255, 255); }
static Color Purple() { return Color(220, 0, 220); }
static Color Error() { return Color(255, 0, 220); }
namespace Draw
{
	inline void Line(int x0, int y0, int x1, int y1, Color col)
	{
		Interfaces.pSurface->DrawSetColor(col);
		Interfaces.pSurface->DrawLine(x0, y0, x1, y1);
	}

	inline void Line(Vector3D start_pos, Vector3D end_pos, Color col)
	{
		Line(start_pos.x, start_pos.y, end_pos.x, end_pos.y, col);
	}

	inline void TexturedPolygon(int n, Vertex_t2* vertice, Color col)
	{
		static int texture_id = Interfaces.pSurface->CreateNewTextureID(true);
		static unsigned char buf[4] = { 255, 255, 255, 255 };
		Interfaces.pSurface->DrawSetTextureRGBA(texture_id, buf, 1, 1);
		Interfaces.pSurface->DrawSetColor(col);
		Interfaces.pSurface->DrawSetTexture(texture_id);
		Interfaces.pSurface->DrawTexturedPolygon(n, vertice);
	}

	inline void Circle(Vector3D position, float points, float radius, Color color)
	{
		float step = (float)M_PI * 2.0f / points;

		for (float a = 0; a < (M_PI * 2.0f); a += step)
		{
			Vector3D start(radius * cosf(a) + position.x, radius * sinf(a) + position.y);
			Vector3D end(radius * cosf(a + step) + position.x, radius * sinf(a + step) + position.y);
			Line(start, end, color);
		}
	}

	inline void Circle(int x, int y, float points, float radius, Color color)
	{
		Circle(Vector3D(x, y), points, radius, color);
	}

	inline void FilledCircle(Vector3D position, float points, float radius, Color color)
	{
		std::vector<Vertex_t2> vertices;
		float step = (float)M_PI * 2.0f / points;

		for (float a = 0; a < (M_PI * 2.0f); a += step)
			vertices.push_back(Vertex_t2(Vector3D(radius * cosf(a) + position.x, radius * sinf(a) + position.y)));

		TexturedPolygon((int)points, vertices.data(), color);
	}

	inline void FilledCircle(int x, int y, float points, float radius, Color color)
	{
		FilledCircle(Vector3D(x, y), points, radius, color);
	}

	inline void Circle3D(Vector position, float points, float radius, Color color)
	{
		float step = (float)M_PI * 2.0f / points;

		std::vector<Vector> points3d;
		for (float a = 0; a < (M_PI * 2.0f); a += step)
		{
			Vector start(radius * cosf(a) + position.x, radius * sinf(a) + position.y, position.z);
			Vector end(radius * cosf(a + step) + position.x, radius * sinf(a + step) + position.y, position.z);

			Vector start2d, end2d;
			if (Interfaces.g_pDebugOverlay->ScreenPosition(start, start2d) || Interfaces.g_pDebugOverlay->ScreenPosition(end, end2d))
				return;

			Line(Vector3D(start2d.x, start2d.y), Vector3D(end2d.x, end2d.y), color);
		}
	}

	inline void FilledRectangle(int x0, int y0, int x1, int y1, Color col)
	{
		Interfaces.pSurface->DrawSetColor(col);
		Interfaces.pSurface->DrawFilledRect(x0, y0, x1, y1);
	}

	inline void FilledRectangle(Vector3D start_pos, Vector3D end_pos, Color col)
	{
		FilledRectangle(start_pos.x, start_pos.y, end_pos.x, end_pos.y, col);
	}

	inline void Rectangle(int x0, int y0, int x1, int y1, Color col)
	{
		Interfaces.pSurface->DrawSetColor(col);
		Interfaces.pSurface->DrawOutlinedRect(x0, y0, x1, y1);
	}

	inline void Rectangle(Vector3D start_pos, Vector3D end_pos, Color col)
	{
		Rectangle(start_pos.x, start_pos.y, end_pos.x, end_pos.y, col);
	}

	inline void FilledGradientRectangle(int x0, int y0, int x1, int y1, Color top, Color bottom)
	{
		Interfaces.pSurface->DrawSetColor(top);
		Interfaces.pSurface->DrawFilledRectFade(x0, y0, x1, y1, 255, 255, false);

		Interfaces.pSurface->DrawSetColor(bottom);
		Interfaces.pSurface->DrawFilledRectFade(x0, y0, x1, y1, 0, 255, false);
	}

	inline void FilledGradientRectangle(Vector3D pos, Vector3D size, Color top, Color bottom)
	{
		FilledGradientRectangle(pos.x, pos.y, pos.x + size.x, pos.y + size.y, top, bottom);
	}



	inline void PolyLine(int* px, int* py, int num_points, Color col)
	{
		Interfaces.pSurface->DrawSetColor(col);
		Interfaces.pSurface->DrawPolyLine(px, py, num_points);
	}

	inline void PolyLine(Vertex_t2* vertice, int num_points, Color col)
	{
		static int* points_x = new int[num_points];
		static int* points_y = new int[num_points];
		for (int i = 0; i < num_points; i++)
		{
			points_x[i] = static_cast<int>(vertice[i].m_Position.x);
			points_y[i] = static_cast<int>(vertice[i].m_Position.y);
		}
		PolyLine(points_x, points_y, num_points, col);
	}

	inline void Text(int x, int y, const wchar_t* text, vgui::HFont font, Color color, bool centeredx, bool centeredy)
	{
		if (centeredx || centeredy)
		{
			int wide, tall;
			Interfaces.pSurface->GetTextSize(font, text, wide, tall);
			if (centeredx)
				x -= wide / 2;
			if (centeredy)
				y -= tall / 2;
		}

		Interfaces.pSurface->DrawSetTextPos(x, y);
		Interfaces.pSurface->DrawSetTextFont(font);
		Interfaces.pSurface->DrawSetTextColor(color);
		Interfaces.pSurface->DrawPrintText(text, wcslen(text));
	}

	inline void Text2(int x, int y, const wchar_t* text, vgui::HFont font, Color color, bool centered = false)
	{
		Text(x, y, text, font, color, centered, centered);
	}

	inline Vector3D GetTextSize(const wchar_t* text, vgui::HFont font)
	{
		int x_res, y_res;
		Interfaces.pSurface->GetTextSize(font, text, x_res, y_res);
		return Vector3D(x_res, y_res);
	}
}
void Font::Create()
{
	Overlay = Interfaces.pSurface->CreateFont_();
	MenuText = Interfaces.pSurface->CreateFont_();
	ESP = Interfaces.pSurface->CreateFont_();

	Interfaces.pSurface->SetFontGlyphSet(Overlay, "Tahoma", 14, 900, 0, 0, FONTFLAG_ANTIALIAS);
	Interfaces.pSurface->SetFontGlyphSet(MenuText, "Tahoma", 12, 550, 0, 0, FONTFLAG_ANTIALIAS);
	Interfaces.pSurface->SetFontGlyphSet(ESP, "Arial", 12, 700, 0, 0, FONTFLAG_DROPSHADOW);
}
//--------------------------------------------------------------------------------
inline bool InBox(int rx1, int ry1, int rx2, int ry2, int px, int py)
{
	return px > rx1 && px < rx2 && py > ry1 && py < ry2;
}
//--------------------------------------------------------------------------------
inline bool InBox(Vector3D r1, Vector3D r2, int px, int py)
{
	return InBox(r1.x, r1.y, r2.x, r2.y, px, py);
}
Vector3D::Vector3D(void)
{}

Vector3D::Vector3D(vec_t X, vec_t Y)
{
	x = X; y = Y;
}

Vector3D::Vector3D(vec_t* clr)
{
	x = clr[0]; y = clr[1];
}

//-----------------------------------------------------------------------------
// initialization
//-----------------------------------------------------------------------------

void Vector3D::Init(vec_t ix, vec_t iy)
{
	x = ix; y = iy;
}

void Vector3D::Random(float minVal, float maxVal)
{
	x = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
	y = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
}

void Vector2DClear(Vector3D& a)
{
	a.x = a.y = 0.0f;
}

//-----------------------------------------------------------------------------
// assignment
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Array access
//-----------------------------------------------------------------------------

vec_t& Vector3D::operator[](int i)
{
	return ((vec_t*)this)[i];
}

vec_t Vector3D::operator[](int i) const
{
	return ((vec_t*)this)[i];
}

//-----------------------------------------------------------------------------
// Base address...
//-----------------------------------------------------------------------------

vec_t* Vector3D::Base()
{
	return (vec_t*)this;
}

vec_t const* Vector3D::Base() const
{
	return (vec_t const*)this;
}

//-----------------------------------------------------------------------------
// IsValid?
//-----------------------------------------------------------------------------

bool Vector3D::IsValid() const
{
	return !isinf(x) && !isinf(y);
}

//-----------------------------------------------------------------------------
// comparison
//-----------------------------------------------------------------------------

bool Vector3D::operator==(const Vector3D& src) const
{
	return (src.x == x) && (src.y == y);
}

bool Vector3D::operator!=(const Vector3D& src) const
{
	return (src.x != x) || (src.y != y);
}


//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------

void Vector2DCopy(const Vector3D& src, Vector3D& dst)
{
	dst.x = src.x;
	dst.y = src.y;
}

void Vector3D::CopyToArray(float* rgfl) const
{
	rgfl[0] = x; rgfl[1] = y;
}

//-----------------------------------------------------------------------------
// standard Math operations
//-----------------------------------------------------------------------------

void Vector3D::Negate()
{
	x = -x; y = -y;
}

void Vector2DAdd(const Vector3D& a, const Vector3D& b, Vector3D& c)
{
	c.x = a.x + b.x;
	c.y = a.y + b.y;
}

void Vector2DSubtract(const Vector3D& a, const Vector3D& b, Vector3D& c)
{
	c.x = a.x - b.x;
	c.y = a.y - b.y;
}

void Vector2DMultiply(const Vector3D& a, vec_t b, Vector3D& c)
{
	c.x = a.x * b;
	c.y = a.y * b;
}

void Vector2DMultiply(const Vector3D& a, const Vector3D& b, Vector3D& c)
{
	c.x = a.x * b.x;
	c.y = a.y * b.y;
}


void Vector2DDivide(const Vector3D& a, vec_t b, Vector3D& c)
{
	vec_t oob = 1.0f / b;
	c.x = a.x * oob;
	c.y = a.y * oob;
}

void Vector2DDivide(const Vector3D& a, const Vector3D& b, Vector3D& c)
{
	c.x = a.x / b.x;
	c.y = a.y / b.y;
}

void Vector2DMA(const Vector3D& start, float s, const Vector3D& dir, Vector3D& result)
{
	result.x = start.x + s * dir.x;
	result.y = start.y + s * dir.y;
}

// FIXME: Remove
// For backwards compatability
void Vector3D::MulAdd(const Vector3D& a, const Vector3D& b, float scalar)
{
	x = a.x + b.x * scalar;
	y = a.y + b.y * scalar;
}

void Vector2DLerp(const Vector3D& src1, const Vector3D& src2, vec_t t, Vector3D& dest)
{
	dest[0] = src1[0] + (src2[0] - src1[0]) * t;
	dest[1] = src1[1] + (src2[1] - src1[1]) * t;
}

//-----------------------------------------------------------------------------
// dot, cross
//-----------------------------------------------------------------------------
vec_t DotProduct2D(const Vector3D& a, const Vector3D& b)
{
	return(a.x*b.x + a.y*b.y);
}

// for backwards compatability
vec_t Vector3D::Dot(const Vector3D& vOther) const
{
	return DotProduct2D(*this, vOther);
}

vec_t Vector2DNormalize(Vector3D& v)
{
	vec_t l = v.Length();
	if (l != 0.0f)
	{
		v /= l;
	}
	else
	{
		v.x = v.y = 0.0f;
	}
	return l;
}

//-----------------------------------------------------------------------------
// length
//-----------------------------------------------------------------------------
vec_t Vector2DLength(const Vector3D& v)
{
	return (vec_t)sqrt(v.x*v.x + v.y*v.y);
}

vec_t Vector3D::NormalizeInPlace()
{
	return Vector2DNormalize(*this);
}

bool Vector3D::IsLengthGreaterThan(float val) const
{
	return LengthSqr() > val*val;
}

bool Vector3D::IsLengthLessThan(float val) const
{
	return LengthSqr() < val*val;
}

vec_t Vector3D::Length(void) const
{
	return Vector2DLength(*this);
}


void Vector2DMin(const Vector3D &a, const Vector3D &b, Vector3D &result)
{
	result.x = (a.x < b.x) ? a.x : b.x;
	result.y = (a.y < b.y) ? a.y : b.y;
}


void Vector2DMax(const Vector3D &a, const Vector3D &b, Vector3D &result)
{
	result.x = (a.x > b.x) ? a.x : b.x;
	result.y = (a.y > b.y) ? a.y : b.y;
}

//-----------------------------------------------------------------------------
// Computes the closest point to vecTarget no farther than flMaxDist from vecStart
//-----------------------------------------------------------------------------
void ComputeClosestPoint2D(const Vector3D& vecStart, float flMaxDist, const Vector3D& vecTarget, Vector3D *pResult)
{
	Vector3D vecDelta;
	Vector2DSubtract(vecTarget, vecStart, vecDelta);
	float flDistSqr = vecDelta.LengthSqr();
	if (flDistSqr <= flMaxDist * flMaxDist)
	{
		*pResult = vecTarget;
	}
	else
	{
		vecDelta /= sqrt(flDistSqr);
		Vector2DMA(vecStart, flMaxDist, vecDelta, *pResult);
	}
}

//-----------------------------------------------------------------------------
// Returns a Vector2D with the min or max in X, Y, and Z.
//-----------------------------------------------------------------------------

Vector3D Vector3D::Min(const Vector3D &vOther) const
{
	return Vector3D(x < vOther.x ? x : vOther.x, y < vOther.y ? y : vOther.y);
}

Vector3D Vector3D::Max(const Vector3D &vOther) const
{
	return Vector3D(x > vOther.x ? x : vOther.x, y > vOther.y ? y : vOther.y);
}


//-----------------------------------------------------------------------------
// arithmetic operations
//-----------------------------------------------------------------------------

Vector3D Vector3D::operator-(void) const
{
	return Vector3D(-x, -y);
}

Vector3D Vector3D::operator+(const Vector3D& v) const
{
	Vector3D res;
	Vector2DAdd(*this, v, res);
	return res;
}

Vector3D Vector3D::operator-(const Vector3D& v) const
{
	Vector3D res;
	Vector2DSubtract(*this, v, res);
	return res;
}

Vector3D Vector3D::operator*(float fl) const
{
	Vector3D res;
	Vector2DMultiply(*this, fl, res);
	return res;
}

Vector3D Vector3D::operator*(const Vector3D& v) const
{
	Vector3D res;
	Vector2DMultiply(*this, v, res);
	return res;
}

Vector3D Vector3D::operator/(float fl) const
{
	Vector3D res;
	Vector2DDivide(*this, fl, res);
	return res;
}

Vector3D Vector3D::operator/(const Vector3D& v) const
{
	Vector3D res;
	Vector2DDivide(*this, v, res);
	return res;
}

Vector3D operator*(float fl, const Vector3D& v)
{
	return v * fl;
}
InputSys::~InputSys()
{
	if (m_ulOldWndProc)
		SetWindowLongPtr(m_hTargetWindow, GWLP_WNDPROC, m_ulOldWndProc);
	m_ulOldWndProc = 0;
}

void InputSys::Initialize()
{
	D3DDEVICE_CREATION_PARAMETERS params;

	if (FAILED(Interfaces.D3->GetCreationParameters(&params)))
		throw std::runtime_error("[InputSys] GetCreationParameters failed.");

	m_hTargetWindow = params.hFocusWindow;
	m_ulOldWndProc = SetWindowLongPtr(m_hTargetWindow, GWLP_WNDPROC, (LONG_PTR)WndProc);

	if (!m_ulOldWndProc)
		throw std::runtime_error("[InputSys] SetWindowLongPtr failed.");
}

LRESULT __stdcall InputSys::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Get().ProcessMessage(msg, wParam, lParam);

	if (Menu::Get().IsVisible())
		return true;

	return CallWindowProcW((WNDPROC)Get().m_ulOldWndProc, hWnd, msg, wParam, lParam);
}

bool InputSys::ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_LBUTTONDBLCLK:
	case WM_XBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDOWN:
	case WM_XBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_LBUTTONUP:
	case WM_XBUTTONUP:
		return ProcessMouseMessage(uMsg, wParam, lParam);
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		return ProcessKeybdMessage(uMsg, wParam, lParam);
	case WM_CHAR:
		Menu::Get().HandleInput(wParam);
	default:
		return false;
	}
}

bool InputSys::ProcessMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	auto key = VK_LBUTTON;
	auto state = KeyState::None;
	switch (uMsg)
	{
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
		state = uMsg == WM_MBUTTONUP ? KeyState::Up : KeyState::Down;
		key = VK_MBUTTON;
		break;
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
		state = uMsg == WM_RBUTTONUP ? KeyState::Up : KeyState::Down;
		key = VK_RBUTTON;
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		state = uMsg == WM_LBUTTONUP ? KeyState::Up : KeyState::Down;
		key = VK_LBUTTON;
		break;
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
		state = uMsg == WM_XBUTTONUP ? KeyState::Up : KeyState::Down;
		key = (HIWORD(wParam) == XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2);
		break;
	default:
		return false;
	}

	if (state == KeyState::Up && m_iKeyMap[int(key)] == KeyState::Down)
	{
		m_iKeyMap[int(key)] = KeyState::Pressed;

		auto& hotkey_callback = m_Hotkeys[key];

		if (hotkey_callback)
			hotkey_callback();

	}
	else
	{
		m_iKeyMap[int(key)] = state;
	}

	return true;
}

bool InputSys::ProcessKeybdMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	auto key = wParam;
	auto state = KeyState::None;

	switch (uMsg)
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		state = KeyState::Down;
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		state = KeyState::Up;
		break;
	default:
		return false;
	}

	if (state == KeyState::Up && m_iKeyMap[int(key)] == KeyState::Down)
	{
		m_iKeyMap[int(key)] = KeyState::Pressed;

		auto& hotkey_callback = m_Hotkeys[key];

		if (hotkey_callback)
			hotkey_callback();

	}
	else
	{
		m_iKeyMap[int(key)] = state;
	}

	return true;
}
KeyState InputSys::GetKeyState(std::uint32_t vk)
{
	return m_iKeyMap[vk];
}
bool InputSys::IsKeyDown(std::uint32_t vk)
{
	return m_iKeyMap[vk] == KeyState::Down;
}
bool InputSys::WasKeyPressed(std::uint32_t vk)
{
	if (m_iKeyMap[vk] == KeyState::Pressed)
	{
		m_iKeyMap[vk] = KeyState::Up;
		return true;
	}
	return false;
}

void InputSys::RegisterHotkey(std::uint32_t vk, std::function<void(void)> f)
{
	m_Hotkeys[vk] = f;
}
void InputSys::RemoveHotkey(std::uint32_t vk)
{
	m_Hotkeys[vk] = nullptr;
}
CObject::CObject(int x, int y, int width, int height, int layer)
	: m_x(x), m_y(y), m_width(width), m_height(height), m_layer(layer)
{}

//--------------------------------------------------------------------------------

void CObjectContainer::AddControl(CObject * obj)
{
	Objects.push_back(obj);
}

//--------------------------------------------------------------------------------

void CForm::Paint()
{
	for (auto &w : Windows)
	{
		w.Paint();
	}
}

void CForm::Click()
{
	for (auto &w : Windows)
	{
		w.Click();
	}
}

void CForm::HandleInput(WPARAM vk)
{
	for (auto &w : Windows)
	{
		w.HandleInput(vk);
	}
}

//--------------------------------------------------------------------------------

CWindow::CWindow(int x, int y, int width, int height, bool* open)
	: CObject(x, y, width, height), m_open(open)
{}

void CWindow::Paint(int t_x, int t_y, float* controlheight)
{
	if (m_open != nullptr && !*m_open) return;

	if (m_dragging && !InputSys::Get().IsKeyDown(VK_LBUTTON))
		m_dragging = false;

	int cur_x, cur_y;
	Interfaces.pSurface->SurfaceGetCursorPos(cur_x, cur_y);

	if (m_dragging && InputSys::Get().IsKeyDown(VK_LBUTTON) && InputSys::Get().GetKeyState(VK_LBUTTON) != KeyState::Pressed)
	{
		m_x = cur_x - m_dragoffsetx;
		m_y = cur_y - m_dragoffsety;
	}

	Vector3D tl(t_x + m_x, t_y + m_y);
	Vector3D br(tl.x + m_width, tl.y + m_height);

	if (InputSys::Get().GetKeyState(VK_LBUTTON) == KeyState::Pressed)
		if (InBox(tl.x, tl.y - 16, br.x, tl.y, cur_x, cur_y))
		{
			m_dragging = true;
			m_dragoffsetx = cur_x - m_x;
			m_dragoffsety = cur_y - m_y;
		}

	Draw::FilledRectangle(tl.x, tl.y - 16, br.x, br.y, Color(100, 0, 0, 135));
	Draw::Rectangle(tl.x, tl.y - 16, br.x, br.y, Color(50, 5, 5));
	Draw::Rectangle(tl.x + 1, tl.y - 15, br.x - 1, br.y - 1, Color(200, 10, 10));
	tl += 6;
	br -= 6;
	Draw::FilledGradientRectangle(tl.x, tl.y, br.x, br.y, Color(238, 238, 238), Color(210, 210, 210));
	Draw::Rectangle(tl, br, Color(50, 5, 5));
	Draw::Rectangle(tl.x - 1, tl.y - 1, br.x + 1, br.y + 1, Color(200, 10, 10));

	Draw::Text2(tl.x + 3, tl.y - 17, L"FAKEWARE for Counter-Strike: Global Offensive", Font::Get().MenuText, Color::White());

	m_controlheight = 0;
	for (auto o : Objects)
	{
		o->Paint(tl.x + 5, tl.y + 5, &m_controlheight);
	}
}

void CWindow::Click()
{
	if (m_open != nullptr && !*m_open) return;

	for (auto o : Objects)
	{
		o->Click();
	}
}

void CWindow::HandleInput(WPARAM vk)
{
	if (m_open != nullptr && !*m_open) return;

	for (auto o : Objects)
	{
		o->HandleInput(vk);
	}
}

//--------------------------------------------------------------------------------

CChild::CChild(int x, int y, int width, std::wstring title)
	: CObject(x, y, width), m_title(title)
{}

void CChild::Paint(int t_x, int t_y, float* controlheight)
{
	Vector3D tl(t_x + m_x, t_y + m_y + 3);
	Vector3D br(t_x + m_x + m_width, t_y + m_y + m_controlheight + 15);

	auto textSize = Draw::GetTextSize(m_title.data(), Font::Get().MenuText);

	// background
	Draw::FilledGradientRectangle(tl.x, tl.y, br.x, br.y, Color(210, 210, 210), Color(238, 238, 238));

	const Color color(180, 180, 180);

	// top
	Draw::Line(tl.x, tl.y, tl.x + 8, tl.y, color);
	Draw::Line(tl.x + 18 + textSize.x, tl.y, br.x, tl.y, color);
	// right
	Draw::Line(br.x, tl.y, br.x, br.y, color);
	//bottom
	Draw::Line(tl.x, br.y, br.x, br.y, color);
	// left
	Draw::Line(tl.x, tl.y, tl.x, br.y, color);

	Draw::Text(tl.x + 14, tl.y, m_title.data(), Font::Get().MenuText, Color::Black(), false, true);

	m_controlheight = 0;
	for (auto o : Objects)
	{
		o->Paint(tl.x + 8, tl.y + 8, &m_controlheight);
	}
}

void CChild::Click()
{
	for (auto o : Objects)
	{
		o->Click();
	}
}

void CChild::HandleInput(WPARAM vk)
{
	for (auto o : Objects)
	{
		o->HandleInput(vk);
	}
}

//--------------------------------------------------------------------------------

CSpacing::CSpacing(int height)
	: m_height(height)
{}

void CSpacing::Paint(int t_x, int t_y, float* controlheight)
{
	if (controlheight == nullptr) return;
	*controlheight += m_height;
}


//--------------------------------------------------------------------------------

CLabel::CLabel(std::wstring text, vgui::HFont font, Color color, bool centered)
	: m_text(text), m_font(font), m_color(color), m_centered(centered)
{}

void CLabel::Paint(int t_x, int t_y, float* controlheight)
{
	if (controlheight == nullptr) return;
	t_y += *controlheight;
	Draw::Text2(t_x + m_x, t_y + m_y, m_text.data(), m_font, m_color, m_centered);
	*controlheight += Draw::GetTextSize(m_text.data(), m_font).y;
}

//--------------------------------------------------------------------------------

CCheckBox::CCheckBox(std::wstring label, bool* value)
	: m_label(label), m_val(value)
{}

void CCheckBox::Paint(int t_x, int t_y, float* controlheight)
{
	if (controlheight == nullptr) return;

	const auto size = 12;

	t_y += *controlheight;

	Vector3D tl(t_x + m_x, t_y + m_y);
	Vector3D br(t_x + m_x + size, t_y + m_y + size);

	int cur_x, cur_y;
	Interfaces.pSurface->SurfaceGetCursorPos(cur_x, cur_y);

	m_hover = InBox(tl.x, tl.y, br.x, br.y, cur_x, cur_y);

	Color border_top, border_bottom, top, bottom;

	if (m_hover)
	{
		border_top = Color(160);
		border_bottom = Color(109);
		top = Color(234);
		bottom = Color(156);
	} else
	{
		border_top = Color(187);
		border_bottom = Color(84);
		top = Color::White();
		bottom = Color(216);
	}

	Draw::FilledGradientRectangle(tl.x, tl.y, br.x, br.y, border_top, border_bottom);
	Draw::FilledGradientRectangle(tl.x + 1, tl.y + 1, br.x - 1, br.y - 1, top, bottom);
	if (m_val != nullptr && *m_val)
		Draw::Text2(tl.x + (size / 2) + 1, tl.y + (size / 2), L"✔", Font::Get().MenuText, Color::Red(), true);
	Draw::Text2(br.x + 4, tl.y - 1, m_label.data(), Font::Get().MenuText, Color::Black());
	*controlheight += size + MENU_PADDING;
}

void CCheckBox::Click()
{
	if (m_val != nullptr && m_hover)
		*m_val = !*m_val;
}

//--------------------------------------------------------------------------------

template<typename T>
inline CSlider<T>::CSlider(std::wstring label, T min, T max, T * value)
	: m_label(label), m_min(min), m_max(max), m_val(value)
{}

template<typename T>
inline void CSlider<T>::Paint(int t_x, int t_y, float* controlheight)
{
	if (controlheight == nullptr) return;
	t_y += *controlheight;

	const auto width = 80;
	const auto height = 14;

	Vector3D tl(t_x + m_x, t_y + m_y);
	Vector3D br(t_x + m_x + width, t_y + m_y + height);

	int cur_x, cur_y;
	g_VGuiSurface->SurfaceGetCursorPos(cur_x, cur_y);

	m_hover = Math::InBox(tl.x - 4, tl.y, br.x + 4, br.y, cur_x, cur_y);

	int valueX = tl.x + ((*m_val - m_min) * width / (m_max - m_min));

	if (m_val != nullptr && m_hover && InputSys::Get().IsKeyDown(VK_LBUTTON))
		*m_val = (cur_x - tl.x) * (m_max - m_min) / width;

	if (*m_val < m_min) *m_val = m_min;
	if (*m_val > m_max) *m_val = m_max;

	Draw::FilledRectangle(tl.x, tl.y + 3, br.x, br.y - 3, Color(237));
	Draw::FilledRectangle(tl.x, tl.y + 3, valueX, br.y - 3, Color(180, 0, 0));
	Draw::Rectangle(tl.x, tl.y + 3, br.x, br.y - 3, Color(141));

	Draw::FilledGradientRectangle(valueX - 4, tl.y + 1, valueX + 4, br.y - 1, Color(208), Color(125));
	Draw::FilledGradientRectangle(valueX - 3, tl.y + 2, valueX + 3, br.y - 2, Color::White, Color(187));

	wchar_t buffer[30];
	swprintf(buffer, 30, L"%s (%s)", m_label.data(), Utilities::to_wstring(*m_val, 2).data());
	Draw::Text(br.x + 9, tl.y, buffer, Font::Get().MenuText, Color::Black);
	*controlheight += height + MENU_PADDING;
}

template class CSlider<int>;
template class CSlider<float>;
template class CSlider<double>;

//--------------------------------------------------------------------------------

CButton::CButton(int width, int height, std::wstring label, std::function<void()> cb)
	: CObject(0, 0, width, height), m_label(label), m_cb(cb)
{}

void CButton::Paint(int t_x, int t_y, float* controlheight)
{
	if (controlheight == nullptr) return;
	t_y += *controlheight;

	Vector3D tl(t_x + m_x, t_y + m_y);
	Vector3D br(t_x + m_x + m_width, t_y + m_y + m_height);

	int cur_x, cur_y;
	Interfaces.pSurface->SurfaceGetCursorPos(cur_x, cur_y);

	m_hover = InBox(tl.x, tl.y, br.x, br.y, cur_x, cur_y);

	Color border_top, border_bottom, top, bottom;

	if (m_hover && InputSys::Get().IsKeyDown(VK_LBUTTON))
	{
		border_top = Color(140);
		border_bottom = Color(188);
		top = Color(160);
		bottom = Color(247);
	} else if (m_hover)
	{
		border_top = Color(191);
		border_bottom = Color(144);
		top = Color(250);
		bottom = Color(178);
	} else
	{
		border_top = Color(187);
		border_bottom = Color(144);
		top = Color(251);
		bottom = Color(207);
	}

	Draw::FilledGradientRectangle(tl.x, tl.y, br.x, br.y, border_top, border_bottom);
	Draw::FilledGradientRectangle(tl.x + 1, tl.y + 1, br.x - 1, br.y - 1, top, bottom);

	Draw::Text2(tl.x + (m_width / 2), tl.y + (m_height / 2), m_label.data(), Font::Get().MenuText, Color::Black(), true);
	*controlheight += m_height + MENU_PADDING;
}

void CButton::Click()
{
	if (m_hover)
		m_cb();
}

//--------------------------------------------------------------------------------

CTextBox::CTextBox(int width, int height, std::wstring placeholder, std::wstring* value)
	: CObject(0, 0, width, height), m_placeholder(placeholder), m_val(value)
{}

void CTextBox::Paint(int t_x, int t_y, float* controlheight)
{
	if (controlheight == nullptr) return;
	t_y += *controlheight;

	m_tl = Vector3D(t_x, t_y);
	m_br = Vector3D(t_x + m_width, t_y + m_height);

	int cur_x, cur_y;
	Interfaces.pSurface->SurfaceGetCursorPos(cur_x, cur_y);

	m_hover = InBox(m_tl.x, m_tl.y, m_br.x, m_br.y, cur_x, cur_y);

	Color border_top, border_bottom;

	if (m_hover)
	{
		border_top = Color(160);
		border_bottom = Color(109);
	} else
	{
		border_top = Color(187);
		border_bottom = Color(84);
	}

	Draw::FilledGradientRectangle(m_tl.x, m_tl.y, m_br.x, m_br.y, border_top, border_bottom);
	Draw::FilledRectangle(m_tl.x + 1, m_tl.y + 1, m_br.x - 1, m_br.y - 1, Color(248));

	if (m_val != nullptr && m_val->length() > 0)
	{
		Draw::Text(m_tl.x + 4, m_tl.y + (m_height / 2), m_val->data(), Font::Get().MenuText, Color::Black(), false, true);

		if (m_selected && (int)(Interfaces.pGlobalVars->realtime * 10) % 2 == 0)
		{
			auto TextSize = Draw::GetTextSize(m_val->data(), Font::Get().MenuText);
			auto tl = m_tl + Vector3D(TextSize.x, 0);
			auto br = m_br + Vector3D(TextSize.x, 0);

			Draw::FilledRectangle(tl.x + 4, tl.y + 3, tl.x + 6, br.y - 3, Color::Black());
		}
	} else if (!m_selected)
	{
		Draw::Text(m_tl.x + 4, m_tl.y + (m_height / 2), m_placeholder.data(), Font::Get().MenuText, Color(100), false, true);
	} else if (m_selected && (int)(Interfaces.pGlobalVars
		->realtime * 2) % 2 == 0)
	{
		Draw::FilledRectangle(m_tl.x + 4, m_tl.y + 3, m_tl.x + 6, m_br.y - 3, Color::Black());
	}
	*controlheight += m_height + MENU_PADDING;
}

void CTextBox::Click()
{
	m_selected = m_hover;
}

void CTextBox::HandleInput(WPARAM vk)
{
	if (m_val != nullptr && m_selected)
	{
		switch (vk)
		{
		case VK_BACK:  // backspace.
			if (m_val->size() > 0)
				m_val->pop_back();
			break;
		case VK_TAB:  // tab.
			break;
		case 0x0A:  // linefeed.
			break;
		case VK_RETURN:  // carriage return.
			m_selected = false;
			break;
		case VK_ESCAPE:  // escape.
			break;
		case VK_SPACE:  // space.
			*m_val += L" ";
			break;

		default:
			*m_val += (wchar_t)vk;
		}
	}
}

//--------------------------------------------------------------------------------

CCombo::CCombo(int* value, std::vector<std::wstring> labels)
	: m_val(value), m_labels(labels)
{}

void CCombo::Paint(int t_x, int t_y, float * controlheight)
{
	if (controlheight == nullptr) return;
	t_y += *controlheight;

	const auto width = 80;
	const auto height = 14;

	// root vectors
	Vector3D r_tl(t_x + m_x, t_y + m_y);
	Vector3D r_br(t_x + m_x + width, t_y + m_y + height);

	int cur_x, cur_y;
	Interfaces.pSurface->SurfaceGetCursorPos(cur_x, cur_y);

	Draw::FilledRectangle(r_tl, r_br, MENU_COLOR_CONTROL - 30);
	Draw::Rectangle(r_tl, r_br, MENU_COLOR_CONTROL_BORDER);

	if (m_val != nullptr)
	{
		{ // Left button
			Vector3D tl(r_tl.x, r_tl.y);
			Vector3D br(r_tl.x + height, r_br.y);
			m_lhover = InBox(tl, br, cur_x, cur_y);
			Color color;
			if (m_lhover && InputSys::Get().IsKeyDown(VK_LBUTTON)) color = MENU_COLOR_CONTROL_ACTIVE;
			else if (m_lhover) color = MENU_COLOR_CONTROL_HOVER;
			else color = MENU_COLOR_CONTROL;
			Draw::FilledRectangle(tl, br, color);
			Draw::Rectangle(tl, br, MENU_COLOR_CONTROL_BORDER);
			Draw::Text2(tl.x + (height / 2), tl.y + (height / 2), L"◄", Font::Get().MenuText, Color::White(), true);
		}

		{ // Right button
			Vector3D tl(r_tl.x + width - height, r_tl.y);
			Vector3D br(r_tl.x + width, r_br.y);
			m_rhover = InBox(tl, br, cur_x, cur_y);
			Color color;
			if (m_rhover && InputSys::Get().IsKeyDown(VK_LBUTTON)) color = MENU_COLOR_CONTROL_ACTIVE;
			else if (m_rhover) color = MENU_COLOR_CONTROL_HOVER;
			else color = MENU_COLOR_CONTROL;
			Draw::FilledRectangle(tl, br, color);
			Draw::Rectangle(tl, br, MENU_COLOR_CONTROL_BORDER);
			Draw::Text2(tl.x + (height / 2) + 1, tl.y + (height / 2), L"►", Font::Get().MenuText, Color::White(), true);
		}

		Draw::Text2(r_tl.x + (width / 2), r_tl.y + (height / 2), m_labels.at(*m_val).data(), Font::Get().MenuText, Color::White(), true);
	}

	*controlheight += height + MENU_PADDING;
}

void CCombo::Click()
{
	if (m_val == nullptr) return;
	if (m_lhover && *m_val > 0) *m_val -= 1;
	else if (m_rhover && *m_val >= 0 && (size_t)*m_val < m_labels.size() - 1) *m_val += 1;

}

//--------------------------------------------------------------------------------
