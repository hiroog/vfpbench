// flatlib 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	"flatlib/core/memory/MemoryBase.h"
#include	"flatlib/core/memory/MemoryBuffer.h"
#include	"flatlib/core/memory/MemoryAllocator.h"
#include	"flatlib/core/memory/MemoryAddress.h"


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace memory {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


void	MemoryBuffer::Finalize()
{
	FL_MEMORY::ZFree( iBuffer );
}


void	MemoryBuffer::MemCopy( const void* memory, size_t size, size_t offset )
{
	FL_ASSERT( iBuffer != nullptr );
	FL_ASSERT( memory != nullptr );
	FL_ASSERT( BufferSize >= size + offset );

	memory::MemCopy( reinterpret_cast<void*>( reinterpret_cast<uintptr_t>(iBuffer) + offset ), memory, size );
	DataSize= size + offset;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

