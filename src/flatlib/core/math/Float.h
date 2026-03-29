// flatlib 2015 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#pragma once

#include	<flatlib/core/system/Types.h>
#include	<math.h>


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace math {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

constexpr float	EQUAL_MIN_FLOAT= 1.0e-4f;
constexpr float	CLAMP_MIN_FLOAT= 1.0e-8f;
constexpr float	PI_FLOAT= 3.1415926535897932f;


class TypeFloat16 {
public:
	enum : unsigned int {
		EXPONENT_BIT_MASK	=	0x7c00,
		FRACTION_BIT_MASK	=	0x03ff,
		EXPONENT_BIT_COUNT	=	5,
		FRACTION_BIT_COUNT	=	10,
		EXPONENT_BIAS		=	15,
		EXPONENT_NAN		=	0x1f,
		P_INFINITY_BIT		=	0x7c00,
		N_INFINITY_BIT		=	0xfc00,
	};
	static constexpr float	FLOAT_MAX= 65504.f;
	static constexpr float	FLOAT_MIN= -65504.f;
	static constexpr float	FLOAT_MIN_= 6.10352e-5f;
};

class TypeFloat32 {
public:
	enum : unsigned int {
		EXPONENT_BIT_MASK	=	0x7f800000u,
		FRACTION_BIT_MASK	=	0x007fffffu,
		EXPONENT_BIT_COUNT	=	8,
		FRACTION_BIT_COUNT	=	23,
		EXPONENT_BIAS		=	128,
		EXPONENT_NAN		=	0xff,
		P_INFINITY_BIT		=	0x7f800000u,
		N_INFINITY_BIT		=	0xff800000u,
	};
	static constexpr float	FLOAT_MAX= 3.402823466e38f;
	static constexpr float	FLOAT_MIN= -3.402823466e38f;
	static constexpr float	FLOAT_MIN_= 1.175494351e-38f;
};

class TypeFloat64 {
public:
	enum : unsigned long long {
		EXPONENT_BIT_MASK	=	0x7ff0000000000000ull,
		FRACTION_BIT_MASK	=	0x000fffffffffffffull,
		EXPONENT_BIT_COUNT	=	11,
		FRACTION_BIT_COUNT	=	52,
		EXPONENT_BIAS		=	1023,
		EXPONENT_NAN		=	0x3ff,
		P_INFINITY_BIT		=	0x7ff0000000000000ull,
		N_INFINITY_BIT		=	0xfff0000000000000ull,
	};
	static constexpr double	FLOAT_MAX= 1.7976931348623157e308;
	static constexpr double	FLOAT_MIN= -1.7976931348623157e308;
	static constexpr double	FLOAT_MIN_= 2.2250738585072014e-308;
};


bool	IsNan( float a );
bool	IsNan( double a );

bool	IsValid( float a );
bool	IsValid( double a );

inline bool	IsZero( float value, float precision= CLAMP_MIN_FLOAT )
{
	return	(value <= precision && value >= -precision);
}

inline float	ClampZero( float value, float precision= CLAMP_MIN_FLOAT )
{
	return	IsZero( value, precision ) ? 0.0f : value;
}

bool	DumpValid( float a, const char* text= nullptr );
bool	DumpValid( double a, const char* text= nullptr );

inline uint32_t		AsInt( float a )
{
	union ct {
		float			fval;
		uint32_t		ival;
	}	c;
	c.fval= a;
	return	c.ival;
}

inline uint64_t	AsInt( double a )
{
	union ct {
		double		fval;
		uint64_t	ival;
	}	c;
	c.fval= a;
	return	c.ival;
}

inline float	AsFloat( uint32_t a )
{
	union ct {
		float		fval;
		uint32_t	ival;
	}	c;
	c.ival= a;
	return	c.fval;
}

inline double	AsFloat( uint64_t a )
{
	union ct {
		double		fval;
		uint64_t	ival;
	}	c;
	c.ival= a;
	return	c.fval;
}

uint32_t	FP32toFP16_soft( float fp32 );
float		FP16toFP32_soft( uint32_t fp16 );
uint32_t	FP32toFP16( float fp32 );
float		FP16toFP32( uint32_t fp16 );


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

inline float	Max( float a, float b )
{
	return	a >= b ? a : b;
}

inline float	Min( float a, float b )
{
	return	a <= b ? a : b;
}

inline float	Clamp( float a, float m1, float m2 )
{
	return	Max( Min( a, m2 ), m1 );
}

float	Sqrt( float a );
float	InvSqrt( float a );

inline float	Abs( float a )
{
	return	fabsf( a );
}

inline float	Exp( float a )
{
	return	expf( a );
}

inline float	Log( float a )
{
	return	logf( a );
}

inline float	Log10( float a )
{
	return	log10f( a );
}

inline float	Pow( float a, float b )
{
	return	powf( a, b );
}

inline float	Sin( float a )
{
	return	sinf( a );
}

inline float	Cos( float a )
{
	return	cosf( a );
}

inline float	Tan( float a )
{
	return	tanf( a );
}

inline float	Atan2( float a, float b )
{
	return	atan2f( a, b );
}

inline float	Atan( float a )
{
	return	atanf( a );
}

inline float	Acos( float a )
{
	return	acosf( a );
}

inline float	Asin( float a )
{
	return	asinf( a );
}

inline float	Equal( float a, float b, float precision = EQUAL_MIN_FLOAT )
{
	float	d= (a - b);
	return	d > -EQUAL_MIN_FLOAT && d < EQUAL_MIN_FLOAT;
}


inline float	DEGtoRAD( float d )
{
	return	d * (PI_FLOAT/180.0f);
}

inline float	RADtoDEG( float r )
{
	return	r * (180.0f/PI_FLOAT);
}


inline float	Ceil( float a )
{
	return	ceilf( a );
}

inline float	Floor( float a )
{
	return	floorf( a );
}

inline float	Round( float a )
{
	return	roundf( a );
}


inline int	RoundInt( float a )
{
	return	static_cast<int>( roundf( a ) );
//	return	static_cast<int>( a + 0.5f );
}

inline float	Inverse( float a )
{
	return	1.0f/a;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

float	Max( const float* ptr, unsigned int size );
unsigned int	ArgMax( const float* ptr, unsigned int size );
unsigned int	ArgMaxAndValue( const float* ptr, unsigned int size, float& value );



float	MaxWithMask( const float* ptr, unsigned int size, const uint8_t* mask );
float	MaxWithMask( const float* ptr, unsigned int size, const float* mask );
unsigned int	ArgMaxWithMask( const float* ptr, unsigned int size, const uint8_t* mask );
unsigned int	ArgMaxWithMask( const float* ptr, unsigned int size, const float* mask );


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

