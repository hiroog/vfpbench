// flatlib 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	"flatlib/core/memory/MemoryBase.h"
#include	"flatlib/core/memory/MemoryAllocator.h"
#include	"flatlib/core/thread/CriticalSection.h"
#include	"flatlib/core/thread/ScopedLock.h"
#include	"flatlib/core/platform/WinString.h"

#if FL_OS_UNIX
# include	<stdlib.h>
#endif

#define	FL_USE_LEGACYALLOCATOR2		0

#if FL_USE_LEGACYALLOCATOR2
# include	"flatlib/core/memory/LegacyAllocator2.h"
#endif


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace memory {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#if FL_USE_LEGACYALLOCATOR2
class Legacy_HeapAllocator : public BaseHeapAllocator {
	struct MallocAllocator {
		enum : bool {
			AutoRaise=	true
		};
		static void	Free( void* ptr )
		{
			free( ptr );
		}
		static void*	Alloc( size_t size )
		{
			return	malloc( size );
		}
	};
	static bool	debug_func( HeapBlock::MemHeapTop* heap, HeapBlock::MemoryBlock* block, void* arg )
	{
		if( block->IsData() ){
			auto*	data_block= block->AsBlockData();
			auto*	data= data_block->GetDataAddress();
			auto*	fence= reinterpret_cast<MemoryAllocator::MemoryFence*>( data );
#if !FL_RETAIL
			FL_PRINT( "%p : %zd byte, array=%d ref=%d : %s %d\n", data, fence->Size, fence->Array, fence->Reference.Get(), fence->FileName, fence->Line );
#else
			FL_PRINT( "%p : %zd byte, array=%d ref=%d\n", data, fence->Size, fence->Array, fence->Reference.Get() );
#endif
		}
		return	true;
	}
public:
	LegacyAllocator2<MallocAllocator>	Allocator;
public:
	void*	Alloc( size_t size ) override
	{
		return	Allocator.Alloc( size );
	}
	void	Free( void* ptr ) override
	{
		Allocator.Free( ptr );
	}
	void	DumpDataBlock()
	{
		FL_PRINT( "LegacyAllocator2: DataBlock %zd %zd\n", Allocator.GetAllocSize(), Allocator.GetAllocCount() );
		Allocator.EnumMemoryBlock( debug_func, nullptr );
	}
};

static Legacy_HeapAllocator	Legacy_HeapAllocator;
#endif


//-----------------------------------------------------------------------------


void*	BaseHeapAllocator::Alloc( size_t size )
{
	return	malloc( size );
}


void	BaseHeapAllocator::Free( void* ptr )
{
	free( ptr );
}


//-----------------------------------------------------------------------------

enum : uint32_t {
	MEMORY_CHECK_CODE		=	0x90f02015,
	MEMORY_CHECK_CODE2		=	0xe9032815,
};


#define	MEMORY_BOTTOM_FENCE		"\x11\x29\xf6\xbe"
#define	MEMORY_BOTTOM_FENCE2	"\xf3\x1a\xc4\x76"


MemoryAllocator		MemoryAllocator::Allocator;
BaseHeapAllocator	MemoryAllocator::HeapAllocator;


MemoryAllocator::MemoryAllocator() : AllocSize( 0 ), AllocCount( 0 )
{
#if FL_USE_LEGACYALLOCATOR2
	SetHeapAllocator( &Legacy_HeapAllocator );
#endif
}


MemoryAllocator::~MemoryAllocator()
{
	Finalize();
}


void	MemoryAllocator::Finalize()
{
	if( GetTotalCount() != 0 ){
		DumpStatus();
		FL_ASSERT( 0 );
	}
}


void	MemoryAllocator::Lock()
{
	LockData.Lock();
}


void	MemoryAllocator::Unlock()
{
	LockData.Unlock();
}


const MemoryAllocator::MemoryFence*	MemoryAllocator::GetFence( const void* ptr )
{
	unsigned int	offset= reinterpret_cast<const unsigned int*>( ptr )[-1];
	return	reinterpret_cast<const MemoryFence*>( reinterpret_cast<uintptr_t>(ptr) - offset - sizeof(MemoryFence) );
}


MemoryAllocator::MemoryFence*	MemoryAllocator::GetFence( void* ptr )
{
	unsigned int	offset= reinterpret_cast<unsigned int*>( ptr )[-1];
	return	reinterpret_cast<MemoryFence*>( reinterpret_cast<uintptr_t>(ptr) - offset - sizeof(MemoryFence) );
}


#if 0
const MemoryAllocator::BottomFence*	MemoryAllocator::GetBottomFence( const void* ptr, const MemoryFence* fence )
{
	return	reinterpret_cast<const BottomFence*>( reinterpret_cast<uintptr_t>( ptr ) + fence->Size );
}
#endif


MemoryAllocator::BottomFence*	MemoryAllocator::GetBottomFence( void* ptr, MemoryFence* fence )
{
	return	reinterpret_cast<BottomFence*>( reinterpret_cast<uintptr_t>( ptr ) + fence->Size );
}


bool	MemoryAllocator::ValidateMemory( void* ptr, MemoryFence* fence, const char* cmd, const char* file_name, unsigned int line )
{
#if !FL_RETAIL
	if( fence->CheckCode != MEMORY_CHECK_CODE ){
		FL_ERROR( "flatlib:Memory Fence error alloc=%s %d, %s=%s %d\n", fence->FileName, fence->Line, cmd, file_name, line );
		return	false;
	}
	auto*	bottom= GetBottomFence( ptr, fence );
	if( memcmp( bottom->CheckCode, MEMORY_BOTTOM_FENCE, 4 ) != 0 ){
		FL_ERROR( "flatlib:Memory Bottom Fence error alloc=%s %d, %s=%s %d\n", fence->FileName, fence->Line, cmd, file_name, line );
		return	false;
	}
#endif
	return	true;
}


void*	MemoryAllocator::AllocNL( size_t size, unsigned int align_size, unsigned int array, const char* file_name, unsigned int line )
{
	size_t	total_size= size + sizeof(MemoryFence) + sizeof(BottomFence);
	if( align_size > FL_DEFAULT_ALIGN_SIZE ){
		total_size+= align_size;
	}
	AllocSize+= size;
	AllocCount++;
	MemoryFence*	fence= reinterpret_cast<MemoryFence*>( iHeapAllocator->Alloc( total_size ) );
	fence->Size= size;
#if !FL_RETAIL
	fence->FileName= file_name;
	fence->Line= line;
#endif
	fence->Array= array;
	fence->Reference.Set( 1 );
	fence->CheckCode= MEMORY_CHECK_CODE;
	uintptr_t	top_ptr= reinterpret_cast<uintptr_t>(fence) + sizeof(MemoryFence);
	void*		alloc_ptr= nullptr;
	if( align_size > FL_DEFAULT_ALIGN_SIZE ){
		align_size--;
		uintptr_t		aligned_ptr= (top_ptr + align_size) & ~static_cast<uintptr_t>(align_size);
		unsigned int	offset= static_cast<unsigned int>( aligned_ptr - top_ptr );
		fence->Offset= offset;
		reinterpret_cast<unsigned int*>( aligned_ptr )[-1]= offset;
		alloc_ptr= reinterpret_cast<void*>( aligned_ptr );
	}else{
		fence->Offset= 0;
		reinterpret_cast<unsigned int*>( top_ptr )[-1]= 0;
		alloc_ptr= reinterpret_cast<void*>( top_ptr );
	}
	BottomFence*	bottom= GetBottomFence( alloc_ptr, fence );
	memory::MemCopy( bottom->CheckCode, MEMORY_BOTTOM_FENCE, 4 );
	FL_VERBOSE( "Alloc %p (%lld) total=%lld count=%d offset=%d\n", alloc_ptr, static_cast<long long>(size), static_cast<long long>(AllocSize), AllocCount, fence->Offset );
	return	alloc_ptr;
}


void*	MemoryAllocator::Alloc( size_t size, unsigned int align_size, unsigned int array, const char* file_name, unsigned int line )
{
	thread::ScopedLock<thread::CriticalSection>	lock_data( LockData );
	return	AllocNL( size, align_size, array, file_name, line );
}


void	MemoryAllocator::FreeNL( void* ptr, unsigned int array, const char* file_name, unsigned int line )
{
	MemoryFence*	fence= GetFence( ptr );
#if FL_RETAIL
	FL_ASSERT( fence->CheckCode == MEMORY_CHECK_CODE );
#else
	if( fence->Array != array ){
		FL_ERROR( "flatlib:Memory Free error: Alloc/Free or New/Delete unmatch (%d,%d) alloc=%s %d, free=%s %d\n", fence->Array, array, fence->FileName, fence->Line, file_name, line );
		FL_ASSERT( 0 );
	}
	if( !ValidateMemory( ptr, fence, "Free", file_name, line ) ){
		FL_ASSERT( 0 );
	}
	if( fence->Reference.Get() > 1 ){
		FL_ERROR( "flatlib:Memory Free error: Reference invalid ref=%d array=%d alloc=%s %d, free=%s %d\n", fence->Reference.Get(), array, fence->FileName, fence->Line, file_name, line );
		FL_ASSERT( 0 );
	}
#endif
	AllocCount--;
	AllocSize-= fence->Size;
	fence->CheckCode= MEMORY_CHECK_CODE2;
	fence->Reference.Set( 0 );
	auto*	bottom= GetBottomFence( ptr, fence );
	memory::MemCopy( bottom->CheckCode, MEMORY_BOTTOM_FENCE2, 4 );
	FL_VERBOSE( "Free %p (%lld) total=%lld count=%d\n", ptr, static_cast<long long>(fence->Size), static_cast<long long>(AllocSize), AllocCount );
	iHeapAllocator->Free( fence );
}


void	MemoryAllocator::Free( void* ptr, unsigned int array, const char* file_name, unsigned int line )
{
	if( ptr ){
		thread::ScopedLock<thread::CriticalSection>	lock_data( LockData );
		FreeNL( ptr, array, file_name, line );
	}
}


int		MemoryAllocator::AddRefNL( void* ptr, const char* file_name, unsigned int line )
{
	if( ptr ){
		MemoryFence*	fence= GetFence( ptr );
#if !FL_RETAIL
		if( !ValidateMemory( ptr, fence, "AddRef", file_name, line ) ){
			FL_ASSERT( 0 );
		}
#endif
		return	fence->Reference.Increment();
	}
	return	0;
}


int		MemoryAllocator::ReleaseNL( void* ptr, const char* file_name, unsigned int line )
{
	if( ptr ){
		MemoryFence*	fence= GetFence( ptr );
#if !FL_RETAIL
		if( !ValidateMemory( ptr, fence, "Release", file_name, line ) ){
			FL_ASSERT( 0 );
		}
#endif
		return	fence->Reference.Decrement();
	}
	return	0;
}


int		MemoryAllocator::AddReferenceNL( const void* ptr, int value, const char* file_name, unsigned int line )
{
	if( ptr ){
		MemoryFence*	fence= GetFence( const_cast<void*>(ptr) );
#if !FL_RETAIL
		if( !ValidateMemory( const_cast<void*>(ptr), fence, "AddReference", file_name, line ) ){
			FL_ASSERT( 0 );
		}
#endif
		return	fence->Reference.LoadAdd( value ) + value;
	}
	return	0;
}


int		MemoryAllocator::GetReferenceNL( const void* ptr, const char* file_name, unsigned int line )
{
	if( ptr ){
		MemoryFence*	fence= GetFence( const_cast<void*>(ptr) );
#if !FL_RETAIL
		if( !ValidateMemory( const_cast<void*>(ptr), fence, "GetReference", file_name, line ) ){
			FL_ASSERT( 0 );
		}
#endif
		return	fence->Reference.Get();
	}
	return	0;
}


BaseHeapAllocator*	MemoryAllocator::SetHeapAllocator( BaseHeapAllocator* heap )
{
	auto*	prev= iHeapAllocator;
	iHeapAllocator= heap ? heap : &HeapAllocator;
	return	prev;
}


void	MemoryAllocator::DumpStatus()
{
	FL_PRINT( "MemoryAllocator: total=%lld byte, count=%d\n", static_cast<long long>(AllocSize), AllocCount );
#if FL_USE_LEGACYALLOCATOR2
	if( AllocCount ){
		Legacy_HeapAllocator.DumpDataBlock();
	}
#endif
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


void	AllocBuffer( MemoryBuffer& buffer, size_t byte_size, unsigned int align_size )
{
	buffer.Finalize();
	buffer.SetBuffer( FL_MEMORY::AllocByte<void>( byte_size, align_size ) );
	buffer.SetDataSize( 0 );
	buffer.SetBufferSize( byte_size );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

