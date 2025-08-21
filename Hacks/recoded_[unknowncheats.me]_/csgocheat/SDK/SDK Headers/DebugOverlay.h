#pragma once
#include "../../stdafx.h"

class CDebugOverlay
{
public:
	int CDebugOverlay::ScreenPosition( const Vector& vecOrigin, Vector& vecScreen )
	{
		typedef int (__thiscall* ScreenPosition_t)( PVOID, const Vector&, Vector& );
		return getvfunc< ScreenPosition_t >( this, 13 )( this, vecOrigin, vecScreen );
	}

	void CDebugOverlay::AddBoxOverlay(const Vector& origin, const Vector& mins, const Vector& max, QAngle const& orientation, int r, int g, int b, int a, float duration)
	{
		typedef void(__thiscall* AddBoxOverlay_t)(PVOID, const Vector&, const Vector&, const Vector&, QAngle const&, int, int, int, int, float);
		return getvfunc< AddBoxOverlay_t >(this, 2)(this, origin, mins, max, orientation, r, g, b, a, duration);
	}
	void CDebugOverlay::AddLineOverlay(const Vector& origin, const Vector& dest, int r, int g, int b, bool noDepthTest, float duration)
	{
		typedef void(__thiscall* AddLineOverlay_t)(PVOID, const Vector&, const Vector&, int, int, int, bool, float);
		return getvfunc< AddLineOverlay_t >(this, 5)(this, origin, dest, r, g, b, noDepthTest, duration);
	}
};
