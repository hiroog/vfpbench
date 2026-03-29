// flatlib 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	"flatlib/core/text/TextBase.h"
#include	"flatlib/core/text/TextPool.h"
#include	"flatlib/core/platform/WinString.h"
#include	"flatlib/core/memory/MemoryAllocator.h"

#define	USE_BUFFER_COPY	0

namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace text {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

TextPool::TextPool() : TextPool( 512 )
{
}

TextPool::TextPool( SizeType size ) : Pool( size ), FormatBuffer( nullptr )
{
}

TextPool::TextPool( TextPool&& src )
{
	Pool= FL_Move( src.Pool );
	FormatBuffer= src.FormatBuffer;
	src.FormatBuffer= nullptr;
}

TextPool&	TextPool::operator=( TextPool&& src )
{
	Finalize();
	Pool= FL_Move( src.Pool );
	FormatBuffer= src.FormatBuffer;
	src.FormatBuffer= nullptr;
	return	*this;
}

TextPool::~TextPool()
{
	Finalize();
}

void	TextPool::Finalize()
{
	FL_MEMORY::ZFree( FormatBuffer );
}

void	TextPool::AddChar( char ch )
{
	*Pool.AllocByte<char>( 1 )= ch;
}

void	TextPool::AddText( const char* ptr )
{
	SizeType	size= static_cast<SizeType>( strlen( ptr ) );
	memory::MemCopy( Pool.AllocByte<char>( size ), ptr, size );
}

void	TextPool::AddFormatV( const char* format, va_list ap )
{
#if USE_BUFFER_COPY
	const int	LINE_BUFFER_SIZE= 1024 * 64;
	if( !FormatBuffer ){
		FormatBuffer= FL_MEMORY::AllocByte<char>( LINE_BUFFER_SIZE );
	}
	vsprintf_s( FormatBuffer, LINE_BUFFER_SIZE-1, format, ap );
	FormatBuffer[LINE_BUFFER_SIZE-1]= '\0';
	AddText( FormatBuffer );
#else
	int	length= 0;
	{
		va_list	cap;
		va_copy( cap, ap );
		length= _vscprintf( format, cap ) + 1;
		va_end( cap );
	}
	if( length ){
		auto*	ptr= Pool.AllocByte<char>( length );
		vsprintf_s( ptr, length, format, ap );
		Pool.SubstSize( 1 );
	}
#endif
}

void	TextPool::AddFormat( const char* format ... )
{
	va_list	ap;
	va_start( ap, format );
	AddFormatV( format, ap );
	va_end( ap );
}

const char*	TextPool::GetText() const
{
	return	Pool.Map<const char>();
}

void*	TextPool::GetBuffer() const
{
	return	Pool.GetBuffer();
}

TextPool::SizeType	TextPool::GetBufferSize() const
{
	return	Pool.GetBufferSize();
}

TextPool::SizeType	TextPool::GetDataSize() const
{
	return	Pool.GetDataSize();
}

void	TextPool::Truncate( SizeType size )
{
	Pool.Truncate( size );
}

void	TextPool::SubstSize( SizeType size )
{
	Pool.SubstSize( size );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

