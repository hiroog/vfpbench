// 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef FLATLIB_MATH_MATH_H_
#define FLATLIB_MATH_MATH_H_

#include	<minilib/Platform.h>
#include	<math.h>

#if flCC_VC
# include	<intrin.h>
# if flCPU_X86 || flCPU_X64
#  define	flCPU_SIMD_SSE	1
#  define	flCPU_SIMD		1
# endif
#endif

#if (flCC_GCC || flCC_CLANG) && (flCPU_X86 || flCPU_X64)
# include	<x86intrin.h>
# define	flCPU_SIMD_SSE	1
# define	flCPU_SIMD		1
#endif

#if (flCC_GCC || flCC_CLANG) && (flCPU_ARM7 || flCPU_ARM64)
# if __ARM_NEON__
#  include	<arm_neon.h>
#  if flCPU_ARM64
#   define	flCPU_SIMD_NEON64	1
#   define	flCPU_SIMD			1
#  else
#   define	flCPU_SIMD_NEON		1
#   define	flCPU_SIMD			1
#  endif
# endif
#endif

#ifndef	flCPU_SIMD_SSE
# define	flCPU_SIMD_SSE		0
#endif
#ifndef	flCPU_SIMD_AVX
# define	flCPU_SIMD_AVX		0
#endif
#ifndef	flCPU_SIMD_NEON
# define	flCPU_SIMD_NEON		0
#endif
#ifndef	flCPU_SIMD_NEON64
# define	flCPU_SIMD_NEON64	0
#endif


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace math {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


#if flCPU_SIMD_AVX
typedef	__m128		flFloat32x4;
typedef	__m256		flFloat32x8;
#elif flCPU_SIMD_SSE
typedef	__m128		flFloat32x4;
struct flFloat32x8 { __m128 v[2]; };
#elif flCPU_SIMD_NEON
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

