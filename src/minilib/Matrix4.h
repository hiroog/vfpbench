// 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef FLATLIB_MATH_MATRIX4_H_
#define FLATLIB_MATH_MATRIX4_H_

#include	<minilib/Math.h>

namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace math {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

class alignas(16) Matrix4 {
public:
	alignas(32) static Matrix4	_Identity;
	union {
		float	m[4][4];
		struct {
			float	_11, _21, _31, _41;
			float	_12, _22, _32, _42;
			float	_13, _23, _33, _43;
			float	_14, _24, _34, _44;
		};
		struct {
			flFloat32x4	vXmm0;
			flFloat32x4	vXmm1;
			flFloat32x4	vXmm2;
			flFloat32x4	vXmm3;
		};
	};
public:
	Matrix4()
	{
	}
	Matrix4( const Matrix4& mat0 )
	{
		Copy( mat0 );
	}
	Matrix4(
			float a11, float a12, float a13, float a14,
			float a21, float a22, float a23, float a24,
			float a31, float a32, float a33, float a34,
			float a41, float a42, float a43, float a44 ) :
		_11( a11 ), _21( a21 ), _31( a31 ), _41( a41 ),
		_12( a12 ), _22( a22 ), _32( a32 ), _42( a42 ),
		_13( a13 ), _23( a23 ), _33( a33 ), _43( a43 ),
		_14( a14 ), _24( a24 ), _34( a34 ), _44( a44 )
	{
	}
	~Matrix4()
	{
	}
	void 	SetIdentity()
	{
		Copy( _Identity );
	}
	void 	Copy( const Matrix4& mat0 )
	{
		memcpy( m, mat0.m, sizeof(Matrix4) );
	}
	void 	CopyUA( const Matrix4& mat0 )
	{
		Copy( mat0 );
	}

	void 	MulCopy_4_4_FPU( const Matrix4& mat0, const Matrix4& mat1 );
	void 	MulCopy_4_4_SIMD_I( const Matrix4& mat0, const Matrix4& mat1 );

	void 	Mul( const Matrix4& mat0 )
	{
#if FL_CPU_SIMD
		MulCopy_4_4_SIMD_I( *this, mat0 );
#else
		MulCopy_4_4_FPU( *this, mat0 );
#endif
	}

	void  	MulCopy( const Matrix4& mat0, const Matrix4& mat1 )
	{
		MulCopy_4_4_FPU( mat0, mat1 );
	}


	void 	RotationX_CS( float cs, float ss );
	void 	RotationY_CS( float cs, float ss );
	void 	RotationZ_CS( float cs, float ss );


	void 	RotationX( float t )
	{
		RotationX_CS( math::Cos(t), math::Sin(t) );
	}
	void 	RotationY( float t )
	{
		RotationY_CS( math::Cos(t), math::Sin(t) );
	}
	void 	RotationZ( float t )
	{
		RotationZ_CS( math::Cos(t), math::Sin(t) );
	}



	void 	Translation_FPU( float x, float y, float z )
	{
		_41+= x*_11 + y*_21 + z*_31;
		_42+= x*_12 + y*_22 + z*_32;
		_43+= x*_13 + y*_23 + z*_33;
		_44+= x*_14 + y*_24 + z*_34;
	}

	void 	Translation( float x, float y, float z )
	{
		Translation_FPU( x, y, z );
	}


	void 	TransposeCopy_FPU( const Matrix4& mat0 );
	void 	TransposeCopy( const Matrix4& mat0 )
	{
		TransposeCopy_FPU( mat0 );
	}
	void 	TransposeCopyU_( const Matrix4& mat0 )
	{
		TransposeCopy_FPU( mat0 );
	}


	void 	Dump( const char* msg= nullptr ) const;

};


#if FL_CPU_SIMD_SSE
inline void	Matrix4::MulCopy_4_4_SIMD_I( const Matrix4& p2, const Matrix4& p1 )
{
#if 1

	__m128	xmm0;
	__m128	xmm1;

	__m128	xmm4= p1.vXmm0;						// _11_21_31_41
	xmm0= _mm_set_ps1( p2._11 );
	xmm1= _mm_mul_ps( xmm0, xmm4 );

	__m128	xmm5= p1.vXmm1;						// _12_22_32_42
	xmm0= _mm_set_ps1( p2._21 );
	xmm0= _mm_mul_ps( xmm0, xmm5 );
	xmm1= _mm_add_ps( xmm1, xmm0 );

	__m128	xmm6= p1.vXmm2;						// _13_23_33_43
	xmm0= _mm_set_ps1( p2._31 );
	xmm0= _mm_mul_ps( xmm0, xmm6 );
	xmm1= _mm_add_ps( xmm1, xmm0 );

	__m128	xmm7= p1.vXmm3;						// _14_24_34_44
	xmm0= _mm_set_ps1( p2._41 );
	xmm0= _mm_mul_ps( xmm0, xmm7 );
	xmm1= _mm_add_ps( xmm1, xmm0 );

	vXmm0= xmm1;



	xmm0= _mm_set_ps1( p2._12 );
	xmm1= _mm_mul_ps( xmm0, xmm4 );

	xmm0= _mm_set_ps1( p2._22 );
	xmm0= _mm_mul_ps( xmm0, xmm5 );
	xmm1= _mm_add_ps( xmm1, xmm0 );

	xmm0= _mm_set_ps1( p2._32 );
	xmm0= _mm_mul_ps( xmm0, xmm6 );
	xmm1= _mm_add_ps( xmm1, xmm0 );

	xmm0= _mm_set_ps1( p2._42 );
	xmm0= _mm_mul_ps( xmm0, xmm7 );
	xmm1= _mm_add_ps( xmm1, xmm0 );

	vXmm1= xmm1;



	xmm0= _mm_set_ps1( p2._13 );
	xmm1= _mm_mul_ps( xmm0, xmm4 );

	xmm0= _mm_set_ps1( p2._23 );
	xmm0= _mm_mul_ps( xmm0, xmm5 );
	xmm1= _mm_add_ps( xmm1, xmm0 );

	xmm0= _mm_set_ps1( p2._33 );
	xmm0= _mm_mul_ps( xmm0, xmm6 );
	xmm1= _mm_add_ps( xmm1, xmm0 );

	xmm0= _mm_set_ps1( p2._43 );
	xmm0= _mm_mul_ps( xmm0, xmm7 );
	xmm1= _mm_add_ps( xmm1, xmm0 );

	vXmm2= xmm1;



	xmm0= _mm_set_ps1( p2._14 );
	xmm1= _mm_mul_ps( xmm0, xmm4 );

	xmm0= _mm_set_ps1( p2._24 );
	xmm0= _mm_mul_ps( xmm0, xmm5 );
	xmm1= _mm_add_ps( xmm1, xmm0 );

	xmm0= _mm_set_ps1( p2._34 );
	xmm0= _mm_mul_ps( xmm0, xmm6 );
	xmm1= _mm_add_ps( xmm1, xmm0 );

	xmm0= _mm_set_ps1( p2._44 );
	xmm0= _mm_mul_ps( xmm0, xmm7 );
	xmm1= _mm_add_ps( xmm1, xmm0 );

	vXmm3= xmm1;

#else

	__m128	xmm0;
	__m128	xmm1;
	__m128	xmm2;
	__m128	xmm3;
	__m128	xmm4;
	__m128	xmm5;
	__m128	xmm6;
	__m128	xmm7;
	__m128	xmm8;
	__m128	xmm9;
	__m128	xmm10;
	__m128	xmm11;


	xmm0= p2.vXmm0;
	xmm1= p2.vXmm1;
	xmm2= p2.vXmm2;
	xmm3= p2.vXmm3;

	xmm4= xmm0;
	xmm4= _mm_unpacklo_ps( xmm4, xmm2 );
	xmm5= xmm1;
	xmm5= _mm_unpacklo_ps( xmm5, xmm3 );

	xmm0= _mm_unpackhi_ps( xmm0, xmm2 );
	xmm1= _mm_unpackhi_ps( xmm1, xmm3 );

	xmm2= xmm4;
	xmm2= _mm_unpacklo_ps( xmm2, xmm5 ); // 0000
	xmm4= _mm_unpackhi_ps( xmm4, xmm5 ); // 1111

	xmm3= xmm0;
	xmm3= _mm_unpacklo_ps( xmm3, xmm1 ); // 2222
	xmm0= _mm_unpackhi_ps( xmm0, xmm1 ); // 3333


	xmm8=  _mm_load_ps( &p1._11 );
	xmm9=  _mm_load_ps( &p1._12 );
	xmm10= _mm_load_ps( &p1._13 );
	xmm11= _mm_load_ps( &p1._14 );


	xmm1= xmm8;
	xmm1= _mm_mul_ps( xmm1, xmm2 );
	xmm5= xmm9;
	xmm5= _mm_mul_ps( xmm5, xmm2 );
	xmm1= _mm_hadd_ps( xmm1, xmm5 );
	xmm6= xmm10;
	xmm6= _mm_mul_ps( xmm6, xmm2 );
	xmm7= xmm11;
	xmm7= _mm_mul_ps( xmm7, xmm2 );
	xmm6= _mm_hadd_ps( xmm6, xmm7 );
	xmm1= _mm_hadd_ps( xmm1, xmm6 );
	vXmm0= xmm1;


	xmm1= xmm8;
	xmm1= _mm_mul_ps( xmm1, xmm4 );
	xmm5= xmm9;
	xmm5= _mm_mul_ps( xmm5, xmm4 );
	xmm1= _mm_hadd_ps( xmm1, xmm5 );
	xmm6= xmm10;
	xmm6= _mm_mul_ps( xmm6, xmm4 );
	xmm7= xmm11;
	xmm7= _mm_mul_ps( xmm7, xmm4 );
	xmm6= _mm_hadd_ps( xmm6, xmm7 );
	xmm1= _mm_hadd_ps( xmm1, xmm6 );
	vXmm1= xmm1;


	xmm1= xmm8;
	xmm1= _mm_mul_ps( xmm1, xmm3 );
	xmm5= xmm9;
	xmm5= _mm_mul_ps( xmm5, xmm3 );
	xmm1= _mm_hadd_ps( xmm1, xmm5 );
	xmm6= xmm10;
	xmm6= _mm_mul_ps( xmm6, xmm3 );
	xmm7= xmm11;
	xmm7= _mm_mul_ps( xmm7, xmm3 );
	xmm6= _mm_hadd_ps( xmm6, xmm7 );
	xmm1= _mm_hadd_ps( xmm1, xmm6 );
	vXmm2= xmm1;


	xmm8= _mm_mul_ps( xmm8, xmm0 );
	xmm9= _mm_mul_ps( xmm9, xmm0 );
	xmm1= _mm_hadd_ps( xmm8, xmm9 );
	xmm10= _mm_mul_ps( xmm10, xmm0 );
	xmm11= _mm_mul_ps( xmm11, xmm0 );
	xmm10= _mm_hadd_ps( xmm10, xmm11 );
	xmm1= _mm_hadd_ps( xmm1, xmm10 );
	vXmm3= xmm1;


/*
	68step

	12x4 = 48 + 16 = 64step

	x64 
	x86 70step
*/

#endif
}
#endif




//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

#endif
