// flatlib 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#pragma once

#include	<flatlib/core/memory/MemoryAddress.h>


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace ut {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

class BinaryPool {
public:
	typedef	size_t	SizeType;
protected:
	unsigned char*	iBuffer;
	SizeType		DataSize;
	SizeType		BufferSize;
protected:
	void	Realloc( SizeType new_size );
public:
	BinaryPool( const BinaryPool& )= delete;
	BinaryPool( BinaryPool&& );
	BinaryPool& operator=( const BinaryPool& )= delete;
	BinaryPool& operator=( BinaryPool&& );
	explicit BinaryPool( SizeType init_size= 512 );
	~BinaryPool();
	void	Finalize();
	void	Reset( SizeType init_size= 512 );
	SizeType	AllocOffset( SizeType byte_size, unsigned int align_size= 1 );
	void*		AllocMemory( SizeType byte_size, unsigned int align_size= 1 );
	template<typename T>
	T*	AllocByte( SizeType byte_size )
	{
		return	reinterpret_cast<T*>( AllocMemory( byte_size, alignof(T) ) );
	}
	void*		GetBuffer() const
	{
		return	iBuffer;
	}
	SizeType	GetDataSize() const
	{
		return	DataSize;
	}
	SizeType	GetBufferSize() const
	{
		return	BufferSize;
	}
	template<typename T>
	T*	Map( SizeType offset= 0 ) const
	{
		return	reinterpret_cast<T*>( memory::OffsetAddress( iBuffer, offset ) );
	}
	void	Truncate( SizeType size );
	void	SubstSize( SizeType size );
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}


