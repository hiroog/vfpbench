// flatlib 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	"flatlib/core/memory/MemoryAddress.h"
#include	"flatlib/core/platform/WinString.h"


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace memory {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


void	MemCopy( void* __restrict dest, const void* __restrict src, size_t size )
{
	FL_ASSERT( reinterpret_cast<uintptr_t>( dest ) + size <= reinterpret_cast<uintptr_t>( src )
			|| reinterpret_cast<uintptr_t>( dest ) >= reinterpret_cast<uintptr_t>( src ) + size );
	memcpy( dest, src, size );
}


void	MemMove( void* dest, const void* src, size_t size )
{
	memmove( dest, src, size );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}


