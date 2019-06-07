// 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef	FLATLIB_PLATFORMSTRING_H_
#define	FLATLIB_PLATFORMSTRING_H_

#include	<string.h>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#if flOS_UNIX

#include	<stdarg.h>

int strcpy_s( char* str, size_t bsize, const char* ptr );
int strcat_s( char* str, size_t bsize, const char* ptr );
int strncpy_s( char* str, size_t bsize, const char* ptr, size_t csize );
int _stricmp( const char* str, const char* ptr );
int _strcmpi( const char* str, const char* ptr );
int _vscprintf( const char* format, va_list ap );
int vsprintf_s( char* str, size_t size, const char* format, va_list ap );
int sprintf_s( char* str, size_t size, const char* format ... );

#endif

namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace memory {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void	MemCopy( void* dest, const void* src, size_t copy_size );

template<typename T>
inline void	MemClear( T& var )
{
	memset( &var, 0, sizeof(T) );
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

#endif

