// flatlib 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#pragma once

#include	<flatlib/core/memory/MemoryBase.h>
#include	<flatlib/core/thread/CriticalSection.h>
#include	<flatlib/core/thread/AtomicValue.h>
#include	<flatlib/core/memory/MemoryBuffer.h>
#include	<new>


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace memory {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

class MemoryBuffer;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

/*
	FL_MEMORY::New<T>( ... )
	FL_MEMORY::NewArray<T>( array_count )
	FL_MEMORY::AllocByte<T>( byte_size )
	FL_MEMORY::AllocType<T>( array_count )
	FL_MEMORY::Free_( ptr )
	FL_MEMORY::Delete_( ptr )
	FL_MEMORY::DeleteArray_( ptr, array_count )
	FL_MEMORY::AutoRelease_( ptr )
	FL_MEMORY::AddRef( ptr )
	FL_MEMORY::ZFree( ptr )
	FL_MEMORY::ZDelete( ptr )
	FL_MEMORY::ZDeleteArray( ptr, array_count )
	FL_MEMORY::ZAutoRelease( ptr )

	memory::MemoryBuffer	buffer;
	memory::AllocBuffer( buffer, size )
*/

class BaseHeapAllocator {
public:
	virtual void*	Alloc( size_t );
	virtual void	Free( void* );
};


class MemoryAllocator {
public:
	static MemoryAllocator		Allocator;
	static BaseHeapAllocator	HeapAllocator;
	struct MemoryFence {
		size_t 			Size;
#if !FL_RETAIL
		const char*		FileName;
		unsigned int	Line;
#endif
		unsigned int	Array;	// 0=alloc, 1=new, 2>=new[]
		thread::AtomicValue<int>	Reference;
		unsigned int	CheckCode;
		unsigned int	Offset;
	};
	struct BottomFence {
		unsigned char	CheckCode[4];
	};
public:
	thread::CriticalSection	LockData;
	BaseHeapAllocator*		iHeapAllocator= &HeapAllocator;
	size_t			AllocSize;
	unsigned int	AllocCount;
private:
	static MemoryFence*	GetFence( void* ptr );
	static void*		GetMemoryAddress( MemoryFence* fence );
	static BottomFence*	GetBottomFence( void* ptr, MemoryFence* fence );
	//static const BottomFence*	GetBottomFence( const void* ptr, const MemoryFence* fence );
	static bool			ValidateMemory( void* ptr, MemoryFence* fence, const char* cmd, const char* file, unsigned int line );
	void*	AllocNL( size_t byte_size, unsigned int align_size= FL_DEFAULT_ALIGN_SIZE, unsigned int array_count= 0, const char* file_name= nullptr, unsigned int line= 0 );
public:
	MemoryAllocator();
	~MemoryAllocator();
	void	Finalize();
	void	Lock();
	void	Unlock();
	void*	Alloc( size_t byte_size, unsigned int align_size= FL_DEFAULT_ALIGN_SIZE, unsigned int array_count= 0, const char* file_name= nullptr, unsigned int line= 0 );
	void	FreeNL( void* ptr, unsigned int array_count= 0, const char* file_name= nullptr, unsigned int line= 0 );
	void	Free( void* ptr, unsigned int array_count= 0, const char* file_name= nullptr, unsigned int line= 0 );
	static const MemoryFence*	GetFence( const void* ptr );
	void	DumpStatus();
	int		AddRefNL( void* ptr, const char* file_name= nullptr, unsigned int line= 0 );
	int		ReleaseNL( void* ptr, const char* file_name= nullptr, unsigned int line= 0 );
	int		AddReferenceNL( const void* ptr, int value, const char* file_name= nullptr, unsigned int line= 0 );
	int		GetReferenceNL( const void* ptr, const char* file_name= nullptr, unsigned int line= 0 );
	size_t	GetTotalSize() const
	{
		return	AllocSize;
	}
	unsigned int	GetTotalCount() const
	{
		return	AllocCount;
	}
	BaseHeapAllocator*	SetHeapAllocator( BaseHeapAllocator* heap );
};


inline MemoryAllocator& RAllocator()
{
	return	MemoryAllocator::Allocator;
}

//-----------------------------------------------------------------------------

template<typename T>
struct ArrayAllocator {
	inline static T*	NewArray( size_t array_count, const char* file_name= nullptr, unsigned int line= 0 )
	{
		T*	ptr= reinterpret_cast<T*>( RAllocator().Alloc( sizeof(T) * array_count, alignof(T), static_cast<unsigned int>(array_count), file_name, line ) );
		for( size_t si= 0 ; si< array_count ; si++ ){
			new( &ptr[si] ) T();
		}
		return	ptr;
	}
	inline static void	DeleteArray( T* ptr, size_t array_count, const char* file_name= nullptr, unsigned int line= 0 )
	{
		if( ptr ){
			//const auto*	fence= MemoryAllocator::GetFence( reinterpret_cast<const void*>(ptr) );
			//array_count= fence->Array;
			for( size_t si= 0 ; si< array_count ; si++ ){
				ptr[si].~T();
			}
			RAllocator().Free( ptr, static_cast<unsigned int>(array_count), file_name, line );
		}
	}
};

template<typename T>
struct ArrayAllocator<T*> {

	inline static T**	NewArray( size_t array_count, const char* file_name= nullptr, unsigned int line= 0 )
	{
		T**	ptr= reinterpret_cast<T**>( RAllocator().Alloc( sizeof(T*) * array_count, alignof(T), static_cast<unsigned int>(array_count), file_name, line ) );
		return	ptr;
	}

	inline static void	DeleteArray( T** ptr, size_t array_count, const char* file_name= nullptr, unsigned int line= 0 )
	{
		if( ptr ){
			RAllocator().Free( ptr, static_cast<unsigned int>(array_count), file_name, line );
		}
	}
};


#if 0
template<typename T>
struct ArrayAllocatorNL {
	inline static void	DeleteArray( T* ptr, size_t array_count, const char* file_name= nullptr, unsigned int line= 0 )
	{
		for( size_t si= 0 ; si< array_count ; si++ ){
			ptr[si].~T();
		}
		RAllocator().FreeNL( ptr, static_cast<unsigned int>(array_count), file_name, line );
	}
};

template<typename T>
struct ArrayAllocatorNL<T*> {
	inline static void	DeleteArray( T** ptr, size_t array_count, const char* file_name= nullptr, unsigned int line= 0 )
	{
		RAllocator().FreeNL( ptr, static_cast<unsigned int>(array_count), file_name, line );
	}
};
#endif



//-----------------------------------------------------------------------------

struct DebugAllocator {
	const char*		File;
	unsigned int	Line;

	DebugAllocator( const char* file_name, unsigned int line ) : File( file_name ), Line( line )
	{
	}

	template<typename T>
	inline T*	AllocByte( size_t byte_size, unsigned int align_size= FL_DEFAULT_ALIGN_SIZE )
	{
		return	reinterpret_cast<T*>( RAllocator().Alloc( byte_size, align_size, 0, File, Line ) );
	}

	template<typename T>
	inline T*	AllocType( size_t array_count, unsigned int align_size= alignof(T) )
	{
		return	reinterpret_cast<T*>( RAllocator().Alloc( sizeof(T)*array_count, align_size, 0, File, Line ) );
	}

	inline void		Free_( void* ptr )
	{
		RAllocator().Free( ptr, 0, File, Line );
	}

	template<typename T, typename... A0>
	inline T*	New( A0&&... a0 )
	{
		return	new( RAllocator().Alloc( sizeof(T), alignof(T), 1, File, Line ) ) T( FL_Forward<A0>(a0)... );
	}

	template<typename T>
	inline void	Delete_( T* ptr )
	{
		if( ptr ){
			ptr->~T();
			RAllocator().Free( reinterpret_cast<void*>(ptr), 1, File, Line );
		}
	}

	template<typename T>
	inline T*	NewArray( size_t array_count )
	{
		return	ArrayAllocator<T>::NewArray( array_count, File, Line );
	}

	template<typename T>
	inline void	DeleteArray_( T* ptr, size_t array_count )
	{
		ArrayAllocator<T>::DeleteArray( ptr, array_count, File, Line );
	}

	inline int	AddRef( void* ptr )
	{
		return	RAllocator().AddRefNL( ptr, File, Line );
	}

	template<typename T>
	inline bool	AutoRelease_( T* ptr )
	{
		if( ptr ){
			if( RAllocator().ReleaseNL( ptr, File, Line ) > 0 ){
				return	false;
			}
			const auto*	fence= MemoryAllocator::GetFence( reinterpret_cast<const void*>(ptr) );
			switch( fence->Array ){
			case 1:
				ptr->~T();
			case 0:
				RAllocator().Free( ptr, fence->Array, File, Line );
				break;
			default:
				ArrayAllocator<T>::DeleteArray( ptr, fence->Array, File, Line );
				break;
			}
			return	true;
		}
		return	false;
	}

	template<typename T>
	inline void	ZAutoRelease( T*& ptr )
	{
		if( ptr ){
			AutoRelease_( ptr );
			ptr= nullptr;
		}
	}
	template<typename T>
	inline void	ZFree( T*& ptr )
	{
		if( ptr ){
			Free_( ptr );
			ptr= nullptr;
		}
	}
	template<typename T>
	inline void	ZDelete( T*& ptr )
	{
		if( ptr ){
			Delete_( ptr );
			ptr= nullptr;
		}
	}
	template<typename T>
	inline void	ZDeleteArray( T*& ptr, size_t array_count )
	{
		if( ptr ){
			DeleteArray_( ptr, array_count );
			ptr= nullptr;
		}
	}
};


//-----------------------------------------------------------------------------

namespace ReleaseAllocator {

	template<typename T>
	inline static T*	AllocByte( size_t byte_size, unsigned int align_size= FL_DEFAULT_ALIGN_SIZE )
	{
		return	reinterpret_cast<T*>( RAllocator().Alloc( byte_size, align_size ) );
	}

	template<typename T>
	inline static T*	AllocType( size_t array_count, unsigned int align_size= alignof(T) )
	{
		return	reinterpret_cast<T*>( RAllocator().Alloc( sizeof(T)*array_count, align_size ) );
	}

	inline static void	Free_( void* ptr )
	{
		RAllocator().Free( ptr );
	}

	template<typename T, typename... A0>
	inline static T*	New( A0&&... a0 )
	{
		return	new( RAllocator().Alloc( sizeof(T), alignof(T), 1 ) ) T( FL_Forward<A0>(a0)... );
	}

	template<typename T>
	inline static void	Delete_( T* ptr )
	{
		if( ptr ){
			ptr->~T();
			RAllocator().Free( reinterpret_cast<void*>(ptr), 1 );
		}
	}

	template<typename T>
	inline static T*	NewArray( size_t array_count )
	{
		return	ArrayAllocator<T>::NewArray( array_count );
	}

	template<typename T>
	inline static void	DeleteArray_( T* ptr, size_t array_count )
	{
		ArrayAllocator<T>::DeleteArray( ptr, array_count );
	}

	inline int	AddRef( void* ptr )
	{
		return	RAllocator().AddRefNL( ptr );
	}

	template<typename T>
	inline bool	AutoRelease_( T* ptr )
	{
		if( ptr ){
			if( RAllocator().ReleaseNL( ptr ) > 0 ){
				return	false;
			}
			const auto*	fence= MemoryAllocator::GetFence( reinterpret_cast<const void*>(ptr) );
			switch( fence->Array ){
			case 1:
				ptr->~T();
			case 0:
				RAllocator().Free( ptr, fence->Array );
				break;
			default:
				ArrayAllocator<T>::DeleteArray( ptr, fence->Array );
				break;
			}
			return	true;
		}
		return	false;
	}

	template<typename T>
	inline void	ZAutoRelease( T*& ptr )
	{
		if( ptr ){
			AutoRelease_( ptr );
			ptr= nullptr;
		}
	}
	template<typename T>
	inline void	ZFree( T*& ptr )
	{
		if( ptr ){
			Free_( ptr );
			ptr= nullptr;
		}
	}
	template<typename T>
	inline void	ZDelete( T*& ptr )
	{
		if( ptr ){
			Delete_( ptr );
			ptr= nullptr;
		}
	}
	template<typename T>
	inline void	ZDeleteArray( T*& ptr, size_t array_count )
	{
		if( ptr ){
			DeleteArray_( ptr, array_count );
			ptr= nullptr;
		}
	}
}


//-----------------------------------------------------------------------------

void	AllocBuffer( MemoryBuffer& buffer, size_t byte_size, unsigned int align_size= FL_DEFAULT_ALIGN_SIZE );


//-----------------------------------------------------------------------------

#if FL_RETAIL
# define		FL_MEMORY		flatlib::memory::ReleaseAllocator
#else
# define		FL_MEMORY		flatlib::memory::DebugAllocator( __FILE__, __LINE__ ).DebugAllocator
#endif


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}


