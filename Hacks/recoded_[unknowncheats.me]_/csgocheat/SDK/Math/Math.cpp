#include "../../stdafx.h"
#define M_PI	3.14159265358979323846f
CMath g_Math;

VOID CMath::sinCos( float radians, PFLOAT sine, PFLOAT cosine )
{
	__asm
	{
		fld dword ptr[radians]
		fsincos
		mov edx, dword ptr[cosine]
		mov eax, dword ptr[sine]
		fstp dword ptr[edx]
		fstp dword ptr[eax]
	}
}

void CMath::angleVectors( const Vector& angles, Vector& forward )
{
	Assert(s_bMathlibInitialized);
	Assert(forward);

	float sp, sy, cp, cy;

	sy = sin( DEG2RAD(angles[1]) );
	cy = cos( DEG2RAD(angles[1]) );

	sp = sin( DEG2RAD(angles[0]) );
	cp = cos( DEG2RAD(angles[0]) );

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}

float CMath::sseSqrt( float x )
{
	float root = 0.0f;

	__asm
	{
		sqrtss xmm0, x
		movss root, xmm0
	}

	return root;
}

Vector CMath::RotatePoint( Vector p, Vector c, float ang )
{
	float angleInRadians = ang * ( M_PI / 180.f );
	float cosTheta = cos( angleInRadians );
	float sinTheta = sin( angleInRadians );
	return Vector(
		( cosTheta * ( p.x - c.x ) -
			sinTheta * ( p.y - c.y ) + c.x ),
		( sinTheta * ( p.x - c.x ) +
			cosTheta * ( p.y - c.y ) + c.y ),
		0 );
}


void CMath::vectorAngles( float* forward, float* angles )
{
	if( forward[ 1 ] == 0.0f && forward[ 0 ] == 0.0f )
	{
		angles[ 0 ] = ( forward[ 2 ] > 0.0f ) ? 270.0f : 90.0f;
		angles[ 1 ] = 0.0f;
	}
	else
	{
		float len2d = sseSqrt( square(forward[0]) + square(forward[1]) );

		angles[ 0 ] = RAD2DEG(atan2f(-forward[2], len2d));
		angles[ 1 ] = RAD2DEG(atan2f(forward[1], forward[0]));

		if( angles[ 0 ] < 0.0f )
			angles[ 0 ] += 360.0f;
		if( angles[ 1 ] < 0.0f )
			angles[ 1 ] += 360.0f;
	}
}

void CMath::vectorAnglesVec( Vector forward, Vector& angles )
{
	if( forward[ 1 ] == 0.0f && forward[ 0 ] == 0.0f )
	{
		angles[ 0 ] = ( forward[ 2 ] > 0.0f ) ? 270.0f : 90.0f;
		angles[ 1 ] = 0.0f;
	}
	else
	{
		float len2d = sseSqrt( square(forward[0]) + square(forward[1]) );

		angles[ 0 ] = RAD2DEG(atan2f(-forward[2], len2d));
		angles[ 1 ] = RAD2DEG(atan2f(forward[1], forward[0]));

		if( angles[ 0 ] < 0.0f )
			angles[ 0 ] += 360.0f;
		if( angles[ 1 ] < 0.0f )
			angles[ 1 ] += 360.0f;
	}
}

void CMath::VectorAngles3D( Vector& vecForward, Vector& vecAngles )
{
	Vector vecView;
	if( vecForward[ 1 ] == 0.f && vecForward[ 0 ] == 0.f )
	{
		vecView[ 0 ] = 0.f;
		vecView[ 1 ] = 0.f;
	}
	else
	{
		vecView[ 1 ] = atan2( vecForward[ 1 ], vecForward[ 0 ] ) * 180.f / M_PI;

		if( vecView[ 1 ] < 0.f )
			vecView[ 1 ] += 360;

		vecView[ 2 ] = sqrt( vecForward[ 0 ] * vecForward[ 0 ] + vecForward[ 1 ] * vecForward[ 1 ] );

		vecView[ 0 ] = atan2( vecForward[ 2 ], vecView[ 2 ] ) * 180.f / M_PI;
	}

	vecAngles[ 0 ] = -vecView[ 0 ];
	vecAngles[ 1 ] = vecView[ 1 ];
}

float deg2rad( float deg )
{
	return ( deg * M_PI / 180 );
}

FORCEINLINE float CMath::DotProduct( const Vector& a, const Vector& b )
{
	return ( a.x * b.x + a.y * b.y + a.z * b.z );
}

void CMath::AngleMatrix( Vector& angles, matrix3x4_t& matrix )
{
	float angle;
	float sr, sp, sy, cr, cp, cy;

	angle = angles.x * ( 6.283185f / 360 );
	sp = sin( angle );
	cp = cos( angle );

	angle = angles.y * ( 6.283185f / 360 );
	sy = sin( angle );
	cy = cos( angle );

	angle = angles.z * ( 6.283185f / 360 );
	sr = sin( angle );
	cr = cos( angle );

	// matrix = (YAW * PITCH) * ROLL
	matrix[ 0 ][ 0 ] = cp * cy;
	matrix[ 1 ][ 0 ] = cp * sy;

	matrix[ 2 ][ 0 ] = -sp;
	matrix[ 0 ][ 1 ] = sr * sp * cy + cr * -sy;

	matrix[ 1 ][ 1 ] = sr * sp * sy + cr * cy;
	matrix[ 2 ][ 1 ] = sr * cp;

	matrix[ 0 ][ 2 ] = ( cr * sp * cy + -sr * -sy );
	matrix[ 1 ][ 2 ] = ( cr * sp * sy + -sr * cy );

	matrix[ 2 ][ 2 ] = cr * cp;

	matrix[ 0 ][ 3 ] = 0.0;
	matrix[ 1 ][ 3 ] = 0.0;
	matrix[ 2 ][ 3 ] = 0.0;
}

void CMath::VectorTransform( const Vector in1, matrix3x4_t in2, Vector& out )
{
	out[ 0 ] = DotProduct( in1, Vector( in2[ 0 ][ 0 ], in2[ 0 ][ 1 ], in2[ 0 ][ 2 ] ) ) + in2[ 0 ][ 3 ];
	out[ 1 ] = DotProduct( in1, Vector( in2[ 1 ][ 0 ], in2[ 1 ][ 1 ], in2[ 1 ][ 2 ] ) ) + in2[ 1 ][ 3 ];
	out[ 2 ] = DotProduct( in1, Vector( in2[ 2 ][ 0 ], in2[ 2 ][ 1 ], in2[ 2 ][ 2 ] ) ) + in2[ 2 ][ 3 ];
}

void CMath::VectorTransform2( const Vector in1, float in2[3][4], Vector& out )
{
	out[ 0 ] = DotProduct( in1, Vector( in2[ 0 ][ 0 ], in2[ 0 ][ 1 ], in2[ 0 ][ 2 ] ) ) + in2[ 0 ][ 3 ];
	out[ 1 ] = DotProduct( in1, Vector( in2[ 1 ][ 0 ], in2[ 1 ][ 1 ], in2[ 1 ][ 2 ] ) ) + in2[ 1 ][ 3 ];
	out[ 2 ] = DotProduct( in1, Vector( in2[ 2 ][ 0 ], in2[ 2 ][ 1 ], in2[ 2 ][ 2 ] ) ) + in2[ 2 ][ 3 ];
}

void CMath::Normalize( Vector& vIn, Vector& vOut )
{
	float flLen = vIn.Length();
	if( flLen == 0 )
	{
		vOut.Init( 0, 0, 1 );
		return;
	}
	flLen = 1 / flLen;
	vOut.Init( vIn.x * flLen, vIn.y * flLen, vIn.z * flLen );
}
RECT GetViewport()
{
	RECT Viewport = { 0, 0, 0, 0 };
	int w, h;
	Interfaces.pEngine->GetScreenSize(w, h);
	Viewport.right = w; Viewport.bottom = h;
	return Viewport;
}
 void CMath::VectorAngles1(const Vector& forward, QAngle &angles) {
	float    tmp, yaw, pitch;

	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if (forward[2] > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
		if (yaw < 0)
			yaw += 360;

		tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
		if (pitch < 0)
			pitch += 360;
	}

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}

bool CMath::WorldToScreen(Vector &in, Vector &out)
{
	const VMatrix& worldToScreen = Interfaces.pEngine->WorldToScreenMatrix(); //Gracb the world to screen matrix from CEngineClient::WorldToScreenMatrix

	float w = worldToScreen[3][0] * in[0] + worldToScreen[3][1] * in[1] + worldToScreen[3][2] * in[2] + worldToScreen[3][3]; //Calculate the angle in compareson to the player's camera.
	out.z = 0; //Screen doesn't have a 3rd dimension.

	if (w > 0.001) //If the object is within view.
	{
		RECT ScreenSize = GetViewport();
		float fl1DBw = 1 / w; //Divide 1 by the angle.
		out.x = (ScreenSize.right / 2) + (0.5f * ((worldToScreen[0][0] * in[0] + worldToScreen[0][1] * in[1] + worldToScreen[0][2] * in[2] + worldToScreen[0][3]) * fl1DBw) * ScreenSize.right + 0.5f); //Get the X dimension and push it in to the Vector.
		out.y = (ScreenSize.bottom / 2) - (0.5f * ((worldToScreen[1][0] * in[0] + worldToScreen[1][1] * in[1] + worldToScreen[1][2] * in[2] + worldToScreen[1][3]) * fl1DBw) * ScreenSize.bottom + 0.5f); //Get the Y dimension and push it in to the Vector.
		return true;
	}

	return false;
}

CMath* CMath::Singleton( void )
{
	static CMath* pMath = nullptr;

	if( !pMath )
		pMath = new CMath();

	return pMath;
}


VOID FORCEINLINE __SPL__( VOID )
{
}
