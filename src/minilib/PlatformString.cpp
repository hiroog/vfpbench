// 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	"CoreLib.h"
#include	"PlatformString.h"

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#if FL_OS_UNIX

#include	<stdio.h>
#include	<string.h>

int strcpy_s( char* str, size_t bsize, const char* ptr )
{
	FL_ASSERT( strlen(ptr) < bsize );
	::strncpy( str, ptr, bsize );
	str[bsize-1]= '\0';
	return	0;
}


int strcat_s( char* str, size_t bsize, const char* ptr )
{
	FL_ASSERT( strlen(str) + strlen(ptr) < bsize );
	::strcat( str, ptr );
	str[bsize-1]= '\0';
	return	0;
}


int strncpy_s( char* str, size_t bsize, const char* ptr, size_t csize )
{
	FL_ASSERT( strlen(ptr) < bsize );
	FL_ASSERT( bsize >= csize );
	::strncpy( str, ptr, csize );
	str[bsize-1]= '\0';
	return	0;
}


int _stricmp( const char* str, const char* ptr )
{
	return	::strcasecmp( str, ptr );
}

int _strcmpi( const char* str, const char* ptr )
{
	return	::strcasecmp( str, ptr );
}


int _vscprintf( const char* format, va_list ap )
{
	// caution!! thread unsafe
	const int	wbufsize= 1024*32;
	static char	workbuffer[wbufsize+8];
	int	ret= ::vsnprintf( workbuffer, wbufsize, format, ap );
	FL_ASSERT( ret >= 0 );
	return	ret;
}


int vsprintf_s( char* str, size_t size, const char* format, va_list ap )
{
	int	ret= ::vsnprintf( str, size, format, ap );
	FL_ASSERT( ret >= 0 );
	FL_ASSERT( static_cast<size_t>(ret) < size );
	if( !(static_cast<size_t>(ret) < size) ){
		// !!
	}
	return	ret;
}


int sprintf_s( char* str, size_t size, const char* format ... )
{
	va_list	ap;
	va_start( ap, format );
	int	ret= vsprintf_s( str, size, format, ap );
	va_end( ap );
	return	ret;
}


#endif


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace memory {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void MemCopy( void* dest, const void* src, size_t copy_size )
{
	FL_ASSERT( reinterpret_cast<uintptr_t>( dest ) + copy_size <= reinterpret_cast<uintptr_t>( src )
		|| reinterpret_cast<uintptr_t>( dest ) >= reinterpret_cast<uintptr_t>( src ) + copy_size );
	memcpy( dest, src, copy_size );
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}
