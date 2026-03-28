// flatlib 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	"flatlib/core/CoreBase.h"
#include	"flatlib/core/platform/WinString.h"
#include	"flatlib/core/system/Assert.h"
#include	<stdio.h>

namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#if !FL_OS_WINDOWS

int sprintf_s( char* str, size_t size, const char* format, ... )
{
	FL_ASSERT( size >= 1 );
#if !FL_RETAIL
	memset( str, 0x1c, size );
#endif
	va_list	ap;
	va_start( ap, format );
	int	result= ::vsnprintf( str, size, format, ap );
	va_end( ap );
	str[size-1]= '\0';
	return	result;
}


int vsprintf_s( char* str, size_t size, const char* format, va_list ap )
{
	FL_ASSERT( size >= 1 );
#if !FL_RETAIL
	memset( str, 0x2c, size );
#endif
	int	result= ::vsnprintf( str, size, format, ap );
	str[size-1]= '\0';
	return	result;
}


int _vscprintf( const char* format, va_list ap )
{
	return	::vsnprintf( nullptr, 0, format, ap );
}


//-----------------------------------------------------------------------------

void	strcpy_s( char* str, size_t size, const char* src )
{
	FL_ASSERT( strlen( src ) < size );
	FL_ASSERT( size >= 1 );
#if !FL_RETAIL
	memset( str, 0x3c, size );
#endif
	strncpy( str, src, size );
	str[size-1]= '\0';
}


void	strncpy_s( char* str, size_t size, const char* src, size_t src_size )
{
	FL_ASSERT( src_size < size );
	FL_ASSERT( size >= 1 );
#if !FL_RETAIL
	memset( str, 0x3c, size );
#endif
	strncpy( str, src, src_size );
	str[size-1]= '\0';
}

//-----------------------------------------------------------------------------

void	strcat_s( char* str, size_t bsize, const char* ptr )
{
	FL_ASSERT( strlen(str) + strlen(ptr) < bsize );
	::strcat( str, ptr );
	str[bsize-1]= '\0';
}


#endif


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

char*	str_sprintf_s( char* str, const char* end_str, const char* format, ... )
{
	va_list	ap;
	va_start( ap, format );
	size_t	result= vsprintf_s( str, end_str - str, format, ap );
	va_end( ap );
	FL_ASSERT( result == strlen(str) );
	return	str + result;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}



