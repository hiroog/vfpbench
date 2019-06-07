// 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	"CoreLib.h"
#include	"MemoryAllocator.h"
#include	<stdlib.h>


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace memory {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace {
	static size_t	TotalAllocCount= 0;
	static size_t	TotalAllocSize= 0;
	enum : uint32_t {
		FENCE_BEGIN	= 0x12345678,
	};
	static const uint32_t	FENCE_END= 0x79ABCDEF;
}

struct MemoryFence {
	size_t		Size;
	uint32_t	Offset;
	uint32_t	Fence;
};

void*	MemoryAlloc( size_t size, size_t alignment )
{
	if( alignment < sizeof(void*) ){
		alignment= sizeof(void*);
	}
	uintptr_t	align_m1= alignment-1;
	size_t		total_size= size + sizeof(MemoryFence) + sizeof(uint32_t) + align_m1;
	uintptr_t	alloc_top= reinterpret_cast<uintptr_t>(malloc( total_size ));
	uintptr_t	data_top= alloc_top + sizeof(MemoryFence);
	uintptr_t	aligned_addr= (data_top + align_m1) & ~align_m1;
	uint32_t	offset= static_cast<uint32_t>( aligned_addr - alloc_top );
	auto*	fence= &reinterpret_cast<MemoryFence*>( aligned_addr )[-1];
	fence->Size= size;
	fence->Offset= offset;
	fence->Fence= FENCE_BEGIN;
	memcpy( reinterpret_cast<void*>( aligned_addr + size ), &FENCE_END, 4 );
	TotalAllocCount++;
	TotalAllocSize+= size;
	FL_LOG( "Alloc=%8zd %4zd %p\n", size, TotalAllocCount, aligned_addr );
	return	reinterpret_cast<void*>( aligned_addr );
}

void	MemoryFree( void* ptr )
{
	if( !ptr ){
		return;
	}
	auto*	fence= &reinterpret_cast<MemoryFence*>( ptr )[-1];
	TotalAllocCount--;
	TotalAllocSize-= fence->Size;
	FL_LOG( "Free=%8zd %4zd %p\n", fence->Size, TotalAllocCount, ptr );
	if( fence->Fence != FENCE_BEGIN ){
		FL_ERROR( "Memory fence-B Unmatch %p (%zd)\n", ptr, fence->Size );
		flASSERT( 0 );
	}
	if( memcmp( reinterpret_cast<void*>((reinterpret_cast<uintptr_t>( ptr ) + fence->Size)), &FENCE_END, 4 ) ){
		FL_ERROR( "Memory fence-E Unmatch %p (%zd)\n", ptr, fence->Size );
		flASSERT( 0 );
	}
	fence->Fence= 0x0;
	free( reinterpret_cast<void*>( reinterpret_cast<uintptr_t>( ptr ) - fence->Offset ) );
}

size_t	GetAllocSize()
{
	return	TotalAllocSize;
}

size_t	GetAllocCount()
{
	return	TotalAllocCount;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}


