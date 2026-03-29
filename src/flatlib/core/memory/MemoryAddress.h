// flatlib 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#pragma once

#include	<flatlib/core/system/Assert.h>
#include	<flatlib/core/system/Types.h>
#include	<string.h>

namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace memory {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


template<typename T>
inline T* AlignAddress( const T* addr, size_t align_size= FL_DEFAULT_ALIGN_SIZE )
{
	size_t	addr_mask= align_size -1;
	return	reinterpret_cast<T*>( (reinterpret_cast<uintptr_t>(addr) + addr_mask) & ~addr_mask);
}


template<typename T,typename TS>
inline T AlignSize( T size, TS align_size )
{
	T	mask= static_cast<T>(align_size) -1;
	return	(size + mask) & ~mask;
}
template<typename T>
inline T AlignSize( T size )
{
	return	AlignSize( size, static_cast<T>(FL_DEFAULT_ALIGN_SIZE) );
}



//-----------------------------------------------------------------------------

template<typename T>
inline void	MemClear( T& object )
{
	//static_assert(std::is_trivially_copyable_v<T>);
	memset( &object, 0, sizeof(object) );
}

//-----------------------------------------------------------------------------


void	MemCopy( void* __restrict dest, const void* __restrict src, size_t size );
void	MemMove( void* dest, const void* src, size_t size );


//-----------------------------------------------------------------------------

template<typename PT,typename TS>
inline PT*	OffsetAddress( PT* addr, TS offset )
{
	return	reinterpret_cast<PT*>( reinterpret_cast<uintptr_t>(addr) + offset );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}


