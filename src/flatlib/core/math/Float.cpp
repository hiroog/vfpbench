// flatlib 2015 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	"flatlib/core/math/MathBase.h"
#include	"flatlib/core/math/Float.h"
#include	"flatlib/core/math/MathSIMD.h"
#include	"flatlib/core/math/Vect4.h"


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace math {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template<typename INTT, typename FT>
int  GetExponentT( INTT ivalue )
{
	INTT	e= (ivalue & FT::EXPONENT_BIT_MASK)>>FT::FRACTION_BIT_COUNT;
	return	static_cast<int>(e) - static_cast<int>(FT::EXPONENT_BIAS);
}


template<typename T,typename FT>
bool	IsNanT( T a )
{
	auto	ival= AsInt( a );
	auto	frac= ival & FT::FRACTION_BIT_MASK;
	auto	exp= (ival & FT::EXPONENT_BIT_MASK) >> FT::EXPONENT_BIT_COUNT;
	return	exp == FT::EXPONENT_NAN && frac != 0;
}


bool	IsNan( float a )
{
	return	IsNanT<float,TypeFloat32>( a );
}


bool	IsNan( double a )
{
	return	IsNanT<double,TypeFloat64>( a );
}


template<typename T,typename FT>
bool	IsValidT( T a )
{
	auto	ival= AsInt( a );
	if( ival == FT::P_INFINITY_BIT ){
		return	false;
	}else if( ival == FT::N_INFINITY_BIT ){
		return	false;
	}else if( IsNan( a ) ){
		return	false;
	}
	return	true;
}

bool	IsValid( float a )
{
	return	IsValidT<float,TypeFloat32>( a );
}

bool	IsValid( double a )
{
	return	IsValidT<double,TypeFloat64>( a );
}


template<typename T,typename FT>
bool	DumpValidT( T a, const char* text )
{
	auto	ival= AsInt( a );
	auto	frac= ival & FT::FRACTION_BIT_MASK;
	auto	exp= (ival & FT::EXPONENT_BIT_MASK) >> FT::EXPONENT_BIT_COUNT;
	if( ival == FT::P_INFINITY_BIT ){
		FL_ERROR( "math: %s +INFINITY\n", text );
		return	false;
	}else if( ival == FT::N_INFINITY_BIT ){
		FL_ERROR( "math: %s -INFINITY\n", text );
		return	false;
	}else if( exp == FT::EXPONENT_NAN && frac != 0 ){
		FL_ERROR( "math: %s NaN\n", text );
		return	false;
	}else if( exp == 0 && frac != 0 ){
		FL_ERROR( "math: %s un-normalized\n", text );
		return	false;
	}
	return	true;
}

bool	DumpValid( float a, const char* text )
{
	return	DumpValidT<float,TypeFloat32>( a, text );
}

bool	DumpValid( double a, const char* text )
{
	return	DumpValidT<double,TypeFloat64>( a, text );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

uint32_t	FP32toFP16_soft( float fp32 )
{ // s10e5
	uint32_t	fbit= AsInt( fp32 );
	if( !fbit ){
		return	0;
	}
	int32_t		e=((fbit & 0x7f800000)>>23) - 127 + 15;
	if( e <= 0 ){
		return	0;
	}
	if( e > 31 ){
		e= 31;
	}
	uint32_t	s=  fbit & 0x80000000;
	uint32_t	f=  fbit & 0x007fffff;
	return	((s>>16)&0x8000) | ((e<<10) & 0x7c00) | ((f>>13) & 0x03ff);
}


float	FP16toFP32_soft( uint32_t fp16 )
{
	if( !fp16 ){
		return	0.0f;
	}
	uint32_t	s=  fp16 & 0x8000;
	int32_t		e=((fp16 & 0x7c00) >>10) - 15 + 127;
	uint32_t	f=  fp16 & 0x03ff;
	uint32_t	fval= (s<<16) | ((e <<23)&0x7f800000) | (f<<13);
	return	AsFloat( fval );
}

#if FL_CPU_F16C

uint32_t	FP32toFP16( float fp32 )
{
	__m128	src= _mm_set_ss( fp32 );
	__m128i	dest= _mm_cvtps_ph( src, _MM_FROUND_TO_NEAREST_INT );
	return	_mm_cvtsi128_si32( dest );
}

float	FP16toFP32( uint32_t fp16 )
{
	__m128i	src= _mm_cvtsi32_si128( fp16 );
	__m128	dest= _mm_cvtph_ps( src );
	return	_mm_cvtss_f32( dest );
}

#elif FL_CPU_SIMD_NEON && FL_CPU_PTR64

uint32_t	FP32toFP16( float fp32 )
{
#if FL_CC_VC
	__n128	src= { 0 };
	src.n128_f32[0]= fp32;
	__n64	dest= vcvt_f16_f32( src );
	return	dest.n64_u16[0];
#else
	Vect4	src{ fp32, 0, 0, 0 };
	union {
		float16x4_t	fp16x4;
		uint16_t	u16[4];
	}	dest;
	dest.fp16x4= vcvt_f16_f32( src.vXmm );
	return	dest.u16[0];
#endif
}

float	FP16toFP32( uint32_t fp16 )
{
#if FL_CC_VC
	__n64	src= { 0 };
	src.n64_u16[0]= fp16;
	__n128	dest= vcvt_f32_f16( src );
	return	dest.n128_f32[0];
#else
	union {
		float16x4_t	fp16x4;
		uint16_t	u16[4];
	}	src;
	src.u16[0]= fp16;
	Vect4	dest;
	dest.vXmm= vcvt_f32_f16( src.fp16x4 );
	return	dest.x;
#endif
}

#else

uint32_t	FP32toFP16( float fp32 )
{
	return	FP32toFP16_soft( fp32 );
}

float	FP16toFP32( uint32_t fp16 )
{
	return	FP16toFP32_soft( fp16 );
}

#endif



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


float	Sqrt( float n )
{
#if 1
	if( n == 0.0f ){
		return	0.0f;
	}
#else
	if( IsZero( n, 1e-6f ) ){
		return	0.0f;
	}
#endif
#if FL_CPU_SIMD_SSE

	if( true ){
        static const __m128 CONST_05= _mm_set_ss( 0.5f );
        static const __m128 CONST_15= _mm_set_ss( 1.5f );

		__m128	r0;
		__m128	r1;
		__m128	r2;
		r0= _mm_set_ss( n );
		r1= _mm_rsqrt_ss( r0 );
		r2= r0;

		r0= _mm_mul_ss( r0, r1 );
		r0= _mm_mul_ss( r0, r1 );
		r0= _mm_mul_ss( r0, r1 );
		r0= _mm_mul_ss( r0, CONST_05 );

		r1= _mm_mul_ss( r1, CONST_15 );
		r1= _mm_sub_ss( r1, r0 );
		r1= _mm_mul_ss( r1, r2 );

		return	_mm_cvtss_f32( r1 );

	}else{
		__m128	val= _mm_set_ss( n );
		return	_mm_cvtss_f32( _mm_mul_ss( _mm_rsqrt_ss( val ), val ) );
	}

#else
	return	sqrtf( n );
#endif
}



float 	InvSqrt( float n )
{
#if 1
	if( n == 0.0f ){
		return	1.0f;
	}
#else
	if( IsZero( n ) ){
		return	1.0f;
	}
#endif
#if FL_CPU_SIMD_SSE

	if( true ){
        static const __m128 CONST_05= _mm_set_ss( 0.5f );
        static const __m128 CONST_15= _mm_set_ss( 1.5f );

		__m128	r0;
		__m128	r1;
		//float	ret;
		r0= _mm_set_ss( n );
		r1= _mm_rsqrt_ss( r0 );	

		r0= _mm_mul_ss( r0, r1 );
		r0= _mm_mul_ss( r0, r1 );
		r0= _mm_mul_ss( r0, r1 );
		r0= _mm_mul_ss( r0, CONST_05 );

		r1= _mm_mul_ss( r1, CONST_15 );
		r1= _mm_sub_ss( r1, r0 );

		return	_mm_cvtss_f32( r1 );
		//_mm_store_ss( &ret, r1 );
		//return	ret;

	}else{
		__m128	val= _mm_set_ss( n );
		return	_mm_cvtss_f32( _mm_rsqrt_ss( val ) );

		//float	ret;
		//_mm_store_ss( &ret, _mm_rsqrt_ss( val ) );
		//return	ret;
	}

#else
	return	1.0f/sqrtf( n );
#endif
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//constexpr float	ARGMAX_FLOAT_MIN_VALUE	= FLOAT_MIN;
constexpr float	ARGMAX_FLOAT_MIN_VALUE	= -1.0e30f;

float	Max( const float* ptr, unsigned int size )
{
	float	max_val= ARGMAX_FLOAT_MIN_VALUE;
	for( unsigned int si= 0 ; si< size ; si++ ){
		float	val= *ptr++;
		if( val > max_val ){
			max_val= val;
		}
	}
	return	max_val;
}

float	MaxWithMask( const float* ptr, unsigned int size, const uint8_t* mask )
{
	float	max_val= ARGMAX_FLOAT_MIN_VALUE;
	for( unsigned int si= 0 ; si< size ; si++ ){
		float	val= *ptr++;
		if( *mask++ ){
			if( val > max_val ){
				max_val= val;
			}
		}
	}
	return	max_val;
}

float	MaxWithMask( const float* ptr, unsigned int size, const float* mask )
{
	float	max_val= ARGMAX_FLOAT_MIN_VALUE;
	for( unsigned int si= 0 ; si< size ; si++ ){
		float	val= *ptr++ * *mask++;
		if( val > max_val ){
			max_val= val;
		}
	}
	return	max_val;
}


unsigned int	ArgMax( const float* ptr, unsigned int size )
{
	float	max_val= ARGMAX_FLOAT_MIN_VALUE;
	unsigned int	index= 0;
	for( unsigned int si= 0 ; si< size ; si++ ){
		float	val= *ptr++;
		if( val > max_val ){
			max_val= val;
			index= si;
		}
	}
	return	index;
}

unsigned int	ArgMaxAndValue( const float* ptr, unsigned int size, float& value )
{
	float	max_val= ARGMAX_FLOAT_MIN_VALUE;
	unsigned int	index= 0;
	for( unsigned int si= 0 ; si< size ; si++ ){
		float	val= *ptr++;
		if( val > max_val ){
			max_val= val;
			index= si;
		}
	}
	value= max_val;
	return	index;
}

unsigned int	ArgMaxWithMask( const float* ptr, unsigned int size, const uint8_t* mask )
{
	float	max_val= ARGMAX_FLOAT_MIN_VALUE;
	unsigned int	index= 0;
	for( unsigned int si= 0 ; si< size ; si++ ){
		float	val= *ptr++;
		if( *mask++ ){
			if( val > max_val ){
				max_val= val;
				index= si;
			}
		}
	}
	return	index;
}

unsigned int	ArgMaxWithMask( const float* ptr, unsigned int size, const float* mask )
{
	float	max_val= ARGMAX_FLOAT_MIN_VALUE;
	unsigned int	index= 0;
	for( unsigned int si= 0 ; si< size ; si++ ){
		float	val= *ptr++ * *mask++;
		if( val > max_val ){
			max_val= val;
			index= si;
		}
	}
	return	index;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}



