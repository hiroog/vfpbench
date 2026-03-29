// flatlib 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#pragma once

#include	<flatlib/core/memory/MemoryAllocator.h>

namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace ut {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


template<typename T, typename SIZET= unsigned int>
class FixedArray {
public:
	typedef	SIZET	SizeType;
protected:
	T*			iBuffer;
	SizeType	BufferSize;
protected:
	void	PtrClear()
	{
		iBuffer= nullptr;
		BufferSize= 0;
	}
public:
	FixedArray() : iBuffer( nullptr ), BufferSize( 0 )
	{
	}
	explicit FixedArray( SizeType size ) : iBuffer( nullptr ), BufferSize( 0 )
	{
		SetArraySize( size );
	}
	FixedArray( FixedArray<T>&& src ) : iBuffer( src.iBuffer ), BufferSize( src.BufferSize )
	{
		src.PtrClear();
	}
	FixedArray( const FixedArray<T>& src ) : iBuffer( nullptr ), BufferSize( 0 )
	{
		DeepCopy( src );
	}
	FixedArray<T>& operator=( const FixedArray<T>& src )
	{
		DeepCopy( src );
		return	*this;
	}
	~FixedArray()
	{
		Finalize();
	}
	void	Finalize()
	{
		FL_MEMORY::ZDeleteArray( iBuffer, BufferSize );
		BufferSize= 0;
	}
	FixedArray<T>& operator=( FixedArray<T>&& src )
	{
		Finalize();
		iBuffer= src.iBuffer;
		BufferSize= src.BufferSize;
		src.PtrClear();
		return	*this;
	}

	void	SetArraySize( SizeType size )
	{
		Finalize();
		iBuffer= FL_MEMORY::NewArray<T>( size );
		BufferSize= size;
	}
	const T&	operator[]( SizeType index ) const
	{
		FL_ASSERT( index < BufferSize );
		FL_ASSERT( iBuffer != nullptr );
		return	iBuffer[index];
	}
	T&	operator[]( SizeType index )
	{
		FL_ASSERT( index < BufferSize );
		FL_ASSERT( iBuffer != nullptr );
		return	iBuffer[index];
	}
	SizeType	GetBufferSize() const
	{
		return	BufferSize;
	}
	SizeType	GetDataSize() const
	{
		return	BufferSize;
	}
	void*	GetBuffer() const
	{
		return	reinterpret_cast<void*>( iBuffer );
	}
	void	DeepCopy( const FixedArray<T>& src )
	{
		Finalize();
		SetArraySize( src.BufferSize );
		SizeType	acount= src.BufferSize;
		for( SizeType ai= 0 ; ai < acount ; ai++ ){
			iBuffer[ai]= src.iBuffer[ai];
		}
	}
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}



