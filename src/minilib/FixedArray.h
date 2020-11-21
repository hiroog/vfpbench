// 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef	FLATLIB_UTIL_FIXEDARRAY_H_
#define	FLATLIB_UTIL_FIXEDARRAY_H_

#include	<minilib/DefaultAllocator.h>
#include	<minilib/Assert.h>


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace ut {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template<typename T>
class FixedArray {
	typedef	unsigned int	SizeType;
	T*			Array;
	SizeType	ArraySize;
private:
	void 	Copy_Internal( const FixedArray& src )
	{
		SizeType	scount= src.GetSize();
		for( SizeType si= 0 ; si< scount ; si++ ){
			Set( si, src.Get( si ) );
		}
	}
public:
	FixedArray() : Array( nullptr ), ArraySize( 0 )
	{
	}
	explicit FixedArray( SizeType count ) : Array( nullptr ), ArraySize( 0 )
	{
		Init( count );
	}
	FixedArray( const FixedArray& src ) : Array( nullptr ), ArraySize( 0 )
	{
		Init( src.GetSize() );
		Copy_Internal( src );
	}
	FixedArray( FixedArray&& src ) : Array( src.Array ), ArraySize( src.ArraySize )
	{
		src.Array= nullptr;
		src.ArraySize= 0;
	}
	FixedArray& operator=( const FixedArray& src )
	{
		if( ArraySize != src.GetSize() ){
			Clear();
			Init( src.GetSize() );
		}
		Copy_Internal( src );
		return	*this;
	}
	FixedArray& operator=( FixedArray&& src )
	{
		if( Array != src.Array ){
			Clear();
			Array= src.Array;
			ArraySize= src.ArraySize;
			src.Array= nullptr;
			src.ArraySize= 0;
		}
		return	*this;
	}
	~FixedArray()
	{
		Clear();
	}
	void 	Clear()
	{
		if( Array ){
			memory::DeleteArray( Array, ArraySize );
			Array= nullptr;
			ArraySize= 0;
		}
	}
	void 	ClearZero()
	{
		memset( Array, 0, sizeof(T) * ArraySize );
	}
	SizeType 	GetSize() const
	{
		return	ArraySize;
	}
	FixedArray& 	Init( SizeType count )
	{
		Clear();
		flASSERT( Array == nullptr );
		ArraySize= count;
		if( count ){
			Array= memory::NewArray<T>( count );
		}
		return	*this;
	}
	const T& 	Get( SizeType index ) const
	{
		flASSERT( Array != nullptr );
		flASSERT( index >= 0 && index < ArraySize );
		return	Array[index];
	}
	void 	Set( SizeType index, const T& value )
	{
		flASSERT( Array != nullptr );
		flASSERT( index >= 0 && index < ArraySize );
		Array[index]= value;
	}
	void 	Set( SizeType index, T&& value )
	{
		flASSERT( Array != nullptr );
		flASSERT( index >= 0 && index < ArraySize );
		Array[index]= FL_Move(value);
	}
	T& operator []( SizeType index )
	{
		flASSERT( Array != nullptr );
		flASSERT( index >= 0 && index < ArraySize );
		return	Array[index];
	}
	const T& operator []( SizeType index ) const
	{
		flASSERT( Array != nullptr );
		flASSERT( index >= 0 && index < ArraySize );
		return	Array[index];
	}
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template<typename T, typename AFunc= DefaultAllocator<T> >
class FixedArrayPOD {
	typedef unsigned int	SizeType;
	T*			Array;
	SizeType	ArraySize;
public:
	FixedArrayPOD( const FixedArrayPOD& )= delete;
	FixedArrayPOD& operator=( const FixedArrayPOD& )= delete;
	FixedArrayPOD() : Array( nullptr ), ArraySize( 0 )
	{
	}
	explicit FixedArrayPOD( SizeType count ) : Array( nullptr ), ArraySize( 0 )
	{
		Init( count );
	}
	~FixedArrayPOD()
	{
		Clear();
	}
	void 	Clear()
	{
		if( Array ){
			AFunc::FreeBuffer( Array );
			Array= nullptr;
		}
		ArraySize= 0;
	}
	void 	ClearZero()
	{
		memset( Array, 0, sizeof(T) * ArraySize );
	}
	SizeType 	GetSize() const
	{
		return	ArraySize;
	}
	FixedArrayPOD& 	Init( SizeType count )
	{
		flASSERT( Array == nullptr );
		ArraySize= count;
		if( count ){
			Array= AFunc::AllocBuffer( count );
		}
		return	*this;
	}
	const T 	Get( SizeType index ) const
	{
		flASSERT( Array != nullptr );
		flASSERT( index >= 0 && index < ArraySize );
		return	Array[index];
	}
	void 	Set( SizeType index, const T& value )
	{
		flASSERT( Array != nullptr );
		flASSERT( index >= 0 && index < ArraySize );
		Array[index]= value;
	}
	T& operator []( SizeType index )
	{
		flASSERT( Array != nullptr );
		flASSERT( index >= 0 && index < ArraySize );
		return	Array[index];
	}
	const T& operator []( SizeType index ) const
	{
		flASSERT( Array != nullptr );
		flASSERT( index >= 0 && index < ArraySize );
		return	Array[index];
	}
};



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

#endif

