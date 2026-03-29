// flatlib 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#pragma once

#include	<flatlib/core/memory/MemoryAddress.h>


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace memory {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


/*!
	@code
		memory::MemoryBuffer	buffer;
		memory::AllocBuffer( buffer, 1024 );
	@endcode
*/
class MemoryBuffer {
public:
private:
	void*	iBuffer;
	size_t	DataSize;
	size_t	BufferSize;
public:
	MemoryBuffer() : iBuffer( nullptr ), DataSize( 0 ), BufferSize( 0 )
	{
	}
	~MemoryBuffer()
	{
		Finalize();
	}
	void	Finalize();
	void	Detach()
	{
		iBuffer= nullptr;
	}
	void*	GetBuffer() const
	{
		return	iBuffer;
	}
	size_t	GetDataSize() const
	{
		return	DataSize;
	}
	size_t	GetBufferSize() const
	{
		return	BufferSize;
	}
	void	SetBuffer( void* buffer )
	{
		iBuffer= buffer;
	}
	void	SetDataSize( size_t size )
	{
		DataSize= size;
	}
	void	SetBufferSize( size_t size )
	{
		BufferSize= size;
	}
	template<typename T>
	T*	Map( size_t offset= 0 ) const
	{
		return	reinterpret_cast<T*>( memory::OffsetAddress( iBuffer, offset ) );
	}
	void	MemCopy( const void* memory, size_t size, size_t offset= 0 );
};




//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}


