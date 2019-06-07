// 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef	FLATLIB_MEMORY_MEMORYALLOCATOR_H_
#define	FLATLIB_MEMORY_MEMORYALLOCATOR_H_

#include	<minilib/CoreLib.h>

namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace memory {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void*	MemoryAlloc( size_t size, size_t alignment );
void	MemoryFree( void* ptr );
size_t	GetAllocCount();
size_t	GetAllocSize();

template<typename T>
inline constexpr size_t	GetAlignment()
{
	return	alignof(T) < sizeof(void*) ? sizeof(void*) : alignof(T);
}

template<typename T,typename... ARG>
inline static T*	New( ARG&&... arg )
{
	return	new( MemoryAlloc( sizeof(T), GetAlignment<T>() ) ) T( FL_Forward<ARG>(arg)... );
}

template<typename T>
inline void	Delete( T* ptr )
{
	if( ptr ){
		ptr->~T();
		MemoryFree( ptr );
	}
}

template<typename T>
inline void	SafeDelete( T*& ptr )
{
	Delete( ptr );
	ptr= nullptr;
}

template<typename T>
inline T*	NewArray( size_t count )
{
	if( count ){
		T*	array= reinterpret_cast<T*>(MemoryAlloc( sizeof(T) * count, GetAlignment<T>() ));
		for( size_t ai= 0 ; ai< count ; ai++ ){
			new( &array[ai] ) T;
		}
		return	array;
	}
	return	nullptr;
}

template<typename T>
inline void	DeleteArray( T* ptr, size_t count )
{
	if( ptr ){
		for( size_t ai= 0 ; ai< count ; ai++ ){
			ptr[ai].~T();
		}
		MemoryFree( ptr );
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

#endif


