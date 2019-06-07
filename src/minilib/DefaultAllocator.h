// 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef	FLATLIB_UTIL_DEFAULTALLOCATOR_H_
#define	FLATLIB_UTIL_DEFAULTALLOCATOR_H_

#include	<minilib/MemoryAllocator.h>


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace util {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template<typename T, size_t Alignment= flatlib::memory::GetAlignment<T>()>
struct DefaultAllocator {
	static T* AllocBuffer( size_t count )
	{
		return	reinterpret_cast<T*>( memory::MemoryAlloc( sizeof(T) * count, Alignment ) );
	}
	static void FreeBuffer( T* data )
	{
		memory::MemoryFree( data );
	}
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

#endif

