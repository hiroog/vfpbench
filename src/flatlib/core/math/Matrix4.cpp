// flatlib 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	"flatlib/core/math/MathBase.h"
//#include	"flatlib/core/math/Float.h"
//#include	"flatlib/core/math/Vect1.h"
//#include	"flatlib/core/math/Vect2.h"
//#include	"flatlib/core/math/Vect3.h"
//#include	"flatlib/core/math/Vect4.h"
//#include	"flatlib/core/math/Quat.h"
//#include	"flatlib/core/math/Matrix3.h"
#include	"flatlib/core/math/Matrix4.h"

#ifndef FL_USE_VECT4
# define	FL_USE_VECT4	1
#endif

namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace math {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#if !FL_USE_VECT4
void    Normalize4_( float* xp )
{
	float	x= xp[0];
	float	y= xp[1];
	float	z= xp[2];
	float	w= xp[3];
	float	rlen= math::InvSqrt( x*x + y*y + z*z + w*w );
    xp[0]*= rlen;
    xp[1]*= rlen;
    xp[2]*= rlen;
    xp[3]*= rlen;
}
#endif

const Matrix4	Matrix4::Identity{
	1.0f,	0.0f,	0.0f,	0.0f,
	0.0f,	1.0f,	0.0f,	0.0f,
	0.0f,	0.0f,	1.0f,	0.0f,
	0.0f,	0.0f,	0.0f,	1.0f,
};



void	Matrix4::Normalize()
{
#if FL_USE_VECT4
	v4(0).Normalize();
	v4(1).Normalize();
	v4(2).Normalize();
	v4(3).Normalize();
#else
	Normalize4_( &_11 );
	Normalize4_( &_12 );
	Normalize4_( &_13 );
	Normalize4_( &_14 );
#endif
}


void	Matrix4::Mul( const Matrix4& src1, const Matrix4& src2 )
{
	Matrix4	dest;
	dest._11= src1._11*src2._11 + src1._21*src2._12 + src1._31*src2._13 + src1._41*src2._14;
	dest._21= src1._11*src2._21 + src1._21*src2._22 + src1._31*src2._23 + src1._41*src2._24;
	dest._31= src1._11*src2._31 + src1._21*src2._32 + src1._31*src2._33 + src1._41*src2._34;
	dest._41= src1._11*src2._41 + src1._21*src2._42 + src1._31*src2._43 + src1._41*src2._44;

	dest._12= src1._12*src2._11 + src1._22*src2._12 + src1._32*src2._13 + src1._42*src2._14;
	dest._22= src1._12*src2._21 + src1._22*src2._22 + src1._32*src2._23 + src1._42*src2._24;
	dest._32= src1._12*src2._31 + src1._22*src2._32 + src1._32*src2._33 + src1._42*src2._34;
	dest._42= src1._12*src2._41 + src1._22*src2._42 + src1._32*src2._43 + src1._42*src2._44;

	dest._13= src1._13*src2._11 + src1._23*src2._12 + src1._33*src2._13 + src1._43*src2._14;
	dest._23= src1._13*src2._21 + src1._23*src2._22 + src1._33*src2._23 + src1._43*src2._24;
	dest._33= src1._13*src2._31 + src1._23*src2._32 + src1._33*src2._33 + src1._43*src2._34;
	dest._43= src1._13*src2._41 + src1._23*src2._42 + src1._33*src2._43 + src1._43*src2._44;

	dest._14= src1._14*src2._11 + src1._24*src2._12 + src1._34*src2._13 + src1._44*src2._14;
	dest._24= src1._14*src2._21 + src1._24*src2._22 + src1._34*src2._23 + src1._44*src2._24;
	dest._34= src1._14*src2._31 + src1._24*src2._32 + src1._34*src2._33 + src1._44*src2._34;
	dest._44= src1._14*src2._41 + src1._24*src2._42 + src1._34*src2._43 + src1._44*src2._44;
	*this= dest;
}


void	Matrix4::SetProjection( float aspect, float n, float f, float fov )
{
	SetIdentity();
	float	half_fov= fov * 0.5f;
	float	cot= math::Cos( half_fov ) / math::Sin( half_fov );
	_11= cot / aspect;
	_22= cot;
	float	q= f/(f-n);
	_33= q;
	_43= -q*n;
	_34= 1.0f;
	_44= 0.0f;
}


void	Matrix4::SetOrtho( float left, float top, float right, float bottom, float n, float f )
{
	SetIdentity();
	float	rwidth= 1.0f / (right - left);
	float	rheight= 1.0f / (top - bottom);
	float	rdepth= 1.0f / (f - n);

	_11= rwidth * 2.0f;
	_22= rheight * 2.0f;
	_33= rdepth;

	_41= (left + right) * rwidth;
	_42= (top + bottom) * rheight;
	_43= -n * rdepth;
}


void	Matrix4::SetScreenSpace( float width, float height )
{
	SetIdentity();
	_11=  2.0f/width;
	_22= -2.0f/height;
	_33=  1.0f;
	_41= -1.0f;
	_42=  1.0f;
	_43=  0.0f;
}


void	Matrix4::Dump( const char* text ) const
{
	FL_PRINT( "Matrix4 %s: %f %f %f %f\n", text, _11, _21, _31, _41 );
	FL_PRINT( "Matrix4 %s: %f %f %f %f\n", text, _12, _22, _32, _42 );
	FL_PRINT( "Matrix4 %s: %f %f %f %f\n", text, _13, _23, _33, _43 );
	FL_PRINT( "Matrix4 %s: %f %f %f %f\n", text, _14, _24, _34, _44 );
}

bool	Matrix4::DumpValid( const char* text ) const
{
	return	true;
}

bool	Matrix4::IsValid() const
{
	return	true;
}





//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}



