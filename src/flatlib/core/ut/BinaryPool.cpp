// flatlib 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	"flatlib/core/ut/UTBase.h"
#include	"flatlib/core/ut/BinaryPool.h"
#include	"flatlib/core/memory/MemoryAllocator.h"


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace ut {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

BinaryPool::BinaryPool( SizeType init_size ) :
	iBuffer( nullptr ),
	DataSize( 0 ),
	BufferSize( init_size )
{
}

BinaryPool::BinaryPool( BinaryPool&& src ) :
	iBuffer( src.iBuffer ),
	DataSize( src.DataSize ),
	BufferSize( src.BufferSize )
{
	src.iBuffer= nullptr;
	src.DataSize= 0;
	src.BufferSize= 0;
}

BinaryPool&	BinaryPool::operator=( BinaryPool&& src )
{
	Finalize();
	iBuffer= src.iBuffer;
	DataSize= src.DataSize;
	BufferSize= src.BufferSize;
	src.iBuffer= nullptr;
	src.DataSize= 0;
	src.BufferSize= 0;
	return	*this;
}

BinaryPool::~BinaryPool()
{
	Finalize();
}

void	BinaryPool::Finalize()
{
	FL_MEMORY::ZFree( iBuffer );
}

void	BinaryPool::Reset( SizeType init_size )
{
	Finalize();
	DataSize= 0;
	BufferSize= init_size;
}

void	BinaryPool::Realloc( SizeType new_size )
{
	auto*	new_buffer= FL_MEMORY::AllocByte<unsigned char>( new_size );
	if( iBuffer ){
		memory::MemCopy( new_buffer, iBuffer, DataSize );
		Finalize();
	}
	iBuffer= new_buffer;
	BufferSize= new_size;
}

BinaryPool::SizeType	BinaryPool::AllocOffset( SizeType byte_size, unsigned int align_size )
{
	SizeType	index= DataSize;
	SizeType	data_size= memory::AlignSize<SizeType>( DataSize, align_size ) + byte_size;
	if( !iBuffer || data_size > BufferSize ){
		SizeType	buf_size= BufferSize;
		do{
			buf_size<<= 1;
		}while( data_size > buf_size );
		Realloc( buf_size );
	}
	DataSize= data_size;
	return	index;
}

void*	BinaryPool::AllocMemory( SizeType byte_size, unsigned int align_size )
{
	auto	index= AllocOffset( byte_size, align_size );
	return	iBuffer + index;
}

void	BinaryPool::Truncate( SizeType size )
{
	if( size <= BufferSize ){
		DataSize= size;;
	}
}

void	BinaryPool::SubstSize( SizeType size )
{
	DataSize-= size;;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

