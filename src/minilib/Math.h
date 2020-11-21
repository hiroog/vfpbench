// 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef FLATLIB_MATH_MATH_H_
#define FLATLIB_MATH_MATH_H_

#include	<minilib/Platform.h>
#include	<math.h>

#if FL_CC_VC
# include	<intrin.h>
# if FL_CPU_X86 || FL_CPU_X64
#  define	FL_CPU_SIMD_SSE	1
#  define	FL_CPU_SIMD		1
# endif
#endif

#if (FL_CC_GCC || FL_CC_CLANG) && (FL_CPU_X86 || FL_CPU_X64)
# include	<x86intrin.h>
# define	FL_CPU_SIMD_SSE	1
# define	FL_CPU_SIMD		1
#endif

#if (FL_CC_GCC || FL_CC_CLANG) && (FL_CPU_ARM7 || FL_CPU_ARM64)
# if __ARM_NEON__
#  include	<arm_neon.h>
#  if FL_CPU_ARM64
#   define	FL_CPU_SIMD_NEON64	1
#   define	FL_CPU_SIMD			1
#  else
#   define	FL_CPU_SIMD_NEON		1
#   define	FL_CPU_SIMD			1
#  endif
# endif
#endif

#ifndef	FL_CPU_SIMD_SSE
# define	FL_CPU_SIMD_SSE		0
#endif
#ifndef	FL_CPU_SIMD_AVX
# define	FL_CPU_SIMD_AVX		0
#endif
#ifndef	FL_CPU_SIMD_NEON
# define	FL_CPU_SIMD_NEON		0
#endif
#ifndef	FL_CPU_SIMD_NEON64
# define	FL_CPU_SIMD_NEON64	0
#endif


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace math {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


#if FL_CPU_SIMD_AVX
typedef	__m128		flFloat32x4;
typedef	__m256		flFloat32x8;
#elif FL_CPU_SIMD_SSE
typedef	__m128		flFloat32x4;
struct flFloat32x8 { __m128 v[2]; };
#elif FL_CPU_SIMD_NEON
typedef	float32x4_t		flFloat32x4;
typedef	float32x4x2_t	flFloat32x8;
#else
struct flFloat32x4 { float v[4]; };
struct flFloat32x8 { float v[8]; };
#endif



constexpr inline float 	CLAMP_FLOAT()
{
	return	1e-7f;
}

constexpr inline float 	PI()
{
	return	3.141592653589793238462f;
}

inline bool 	IsZero( float value, float precis= CLAMP_FLOAT() )
{
	return	(value <= precis && value >= -precis);
}

inline bool 	Equal( float a, float b, float precis= CLAMP_FLOAT() )
{
	return	IsZero( a - b, precis );
}

inline float 	DEGtoRAD( float deg )
{
	return	deg * (PI()/180.0f);
}

inline float 	Sin( float s )
{
	return	sinf( s );
}

inline float 	Cos( float s )
{
	return	cosf( s );
}

unsigned int	FloatToHalf( float fp );
float			HalfToFloat( unsigned int hp );

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

#endif

