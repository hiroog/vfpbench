// flatlib 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#pragma once

#include	<flatlib/core/math/Float.h>
#include	<flatlib/core/math/MathSIMD.h>


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace math {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

class Vect3;

class alignas(16) Vect4 {
public:
	union {
#if FL_CPU_SIMD_SSE
		__m128		vXmm;
		__m128i		vXmmi;
#elif FL_CPU_SIMD_NEON
		float32x4_t	vXmm;
#else
		struct { float v[4]; }	vXmm;
#endif
		struct {
			float	x, y, z, w;
		};
	};
	static const Vect4	Zero;
public:
	Vect4()
	{
	}
	Vect4( float x_, float y_, float z_, float w_ ) : x( x_ ), y( y_ ), z( z_ ), w( w_ )
	{
	}
	Vect4( const Vect4& src ) : x( src.x ), y( src.y ), z( src.z ), w( src.w )
	{
	}
	~Vect4()
	{
	}
	void	Set( float x_, float y_, float z_, float w_ )
	{
		x= x_;
		y= y_;
		z= z_;
		w= w_;
	}
	Vect3&	xyz()
	{
		return	*reinterpret_cast<Vect3*>(&x);
	}
	const Vect3&	xyz() const
	{
		return	*reinterpret_cast<const Vect3*>(&x);
	}
	float	Dot( const Vect4& src ) const
	{
		return	x * src.x + y * src.y + z * src.z + w * src.w;
	}
	float	Magnitude() const
	{
		return	Dot( *this );
	}
	float	Length() const;
	float	ReciprocalLength() const;
	void	Normalize();
	Vect4	Max( const Vect4& src ) const;
	Vect4	Min( const Vect4& src ) const;
	bool	Equal( const Vect4& src, float precision= EQUAL_MIN_FLOAT ) const;
	Vect4	operator*( const Vect4& src ) const
	{
		return	Vect4( x * src.x, y * src.y, z * src.z, w * src.w );
	}
	Vect4	operator+( const Vect4& src ) const
	{
		return	Vect4( x + src.x, y + src.y, z + src.z, w + src.w );
	}
	Vect4	operator-( const Vect4& src ) const
	{
		return	Vect4( x - src.x, y - src.y, z - src.z, w - src.w );
	}
	Vect4	operator-() const
	{
		return	Vect4( -x, -y, -z, -w );
	}
	Vect4&	operator+=( const Vect4& src )
	{
		x+= src.x;
		y+= src.y;
		z+= src.z;
		w+= src.w;
		return	*this;
	}
	Vect4&	operator*=( const Vect4& src )
	{
		x*= src.x;
		y*= src.y;
		z*= src.z;
		w*= src.w;
		return	*this;
	}
	Vect4&	operator*=( float scale )
	{
		x*= scale;
		y*= scale;
		z*= scale;
		w*= scale;
		return	*this;
	}
	void	Dump( const char* text= nullptr ) const;
	bool	DumpValid( const char* text= nullptr) const;
	bool	IsValid() const;
};



inline Vect4	operator*( const Vect4& src1, float scale )
{
	return	Vect4( src1.x * scale, src1.y * scale, src1.z * scale, src1.w * scale );
}


inline Vect4	operator*( float scale, const Vect4& src1  )
{
	return	src1 * scale;
}








//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

