// 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef	FLATLIB_UTIL_BINARYBUFFER_H_
#define	FLATLIB_UTIL_BINARYBUFFER_H_

#include	<minilib/DefaultAllocator.h>
#include	<minilib/PlatformString.h>


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace util {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


template<typename SizeType= size_t, typename AFunc= DefaultAllocator<unsigned char,64>>
class BinaryBuffer {
private:
	unsigned char*	AllocBuffer;
	SizeType		Pointer;
	SizeType		AllocSize;
public:
	BinaryBuffer( const BinaryBuffer& )= delete;
	BinaryBuffer& operator=( const BinaryBuffer& )= delete;
	BinaryBuffer() : AllocBuffer( nullptr ), Pointer( 0 ), AllocSize( 0 )
	{
	}
	~BinaryBuffer()
	{
		Finalize();
	}
	void SetInitialSize( SizeType size )
	{
		Finalize();
		Pointer= 0;
		AllocSize= size;
		AllocBuffer= AFunc::AllocBuffer( AllocSize );
	}
	void Finalize()
	{
		if( AllocBuffer ){
			AFunc::FreeBuffer( AllocBuffer );
			AllocBuffer= nullptr;
			Pointer= 0;
			AllocSize= 0;
		}
	}
	void Clear()
	{
		Finalize();
	}
	void* Alloc( SizeType size )
	{
		if( Pointer + size > AllocSize ){
			SizeType	new_size= AllocSize ? AllocSize : 64;
			do{
				new_size<<= 1;
			}while( Pointer + size > new_size );
			FL_LOG( "Resize %zd -> %zd  req %zd ptr %zd\n", AllocSize, new_size, size, Pointer );
			unsigned char*	new_buf= AFunc::AllocBuffer( new_size );
			flASSERT( new_buf != nullptr );
			if( AllocBuffer ){
				memory::MemCopy( new_buf, AllocBuffer, Pointer );
				AFunc::FreeBuffer( AllocBuffer );
			}
			AllocBuffer= new_buf;
			AllocSize= new_size;
		}
		void*	alloc_top= reinterpret_cast<void*>( AllocBuffer + Pointer );
		Pointer+= size;
		return	alloc_top;
	}
	SizeType GetSize() const
	{
		return	Pointer;
	}
	const void* GetTop() const
	{
		return	AllocBuffer;
	}
	void* GetBinaryAddress()
	{
		return	AllocBuffer;
	}
	void DecrementPointer( SizeType size )
	{
		if( size < Pointer ){
			Pointer-= size;
		}else{
			Pointer= 0;
		}
	}
};


typedef BinaryBuffer<uint32_t>	BinaryBuffer32;


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

#endif

