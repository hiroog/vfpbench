// flatlib 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#pragma once

#include	<flatlib/core/math/Float.h>
#include	<flatlib/core/math/Vect4.h>
#include	<flatlib/core/system/Assert.h>


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace math {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

class	Matrix3;


class alignas(16) Matrix4 {
public:
	struct {
		float	_11, _21, _31, _41;
		float	_12, _22, _32, _42;
		float	_13, _23, _33, _43;
		float	_14, _24, _34, _44;
	};
	static const Matrix4	Identity;
public:
	Matrix4()
	{
	}
	Matrix4(
			float a, float b, float c, float d,
			float e, float f, float g, float h,
			float i, float j, float k, float l,
			float m, float n, float o, float p ) :
			_11( a ), _21( b ), _31( c ), _41( d ),
			_12( e ), _22( f ), _32( g ), _42( h ),
			_13( i ), _23( j ), _33( k ), _43( l ),
			_14( m ), _24( n ), _34( o ), _44( p )
	{
	}
	Matrix4( const Matrix4& src )= default;
	Matrix4&	operator=( const Matrix4& )= default;
	~Matrix4()
	{
	}
	const Vect4&	v4( unsigned int index ) const
	{
		FL_ASSERT( index < 4 );
		return	reinterpret_cast<const Vect4*>( &_11 )[ index ];
	}
	Vect4&	v4( unsigned int index )
	{
		FL_ASSERT( index < 4 );
		return	reinterpret_cast<Vect4*>( &_11 )[ index ];
	}
	const Matrix3&	m3() const
	{
		return	*reinterpret_cast<const Matrix3*>( &_11 );
	}
	Matrix3&	m3()
	{
		return	*reinterpret_cast<Matrix3*>( &_11 );
	}
	void	SetIdentity()
	{
		*this= Identity;
	}
	void	Mul( const Matrix4& src1, const Matrix4& src2 );
	void	Mul( const Matrix4& src )
	{
		Mul( *this, src );
	}
	void	SetProjection( float aspect, float n, float f, float fov );
	void	SetOrtho( float left, float top, float right, float bottom, float n, float f );
	void	SetScreenSpace( float width, float height );
	void	Normalize();
	void	Dump( const char* text= nullptr ) const;
	bool	DumpValid( const char* text= nullptr) const;
	bool	IsValid() const;
};









//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

