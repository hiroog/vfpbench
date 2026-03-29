// flatlib 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#pragma once

#include	<flatlib/core/system/Platform.h>
#include	<string.h>
#if FL_OS_WINDOWS
# include	<stdio.h>
#endif

#if FL_OS_CS
# include	<stdio.h>
#endif

namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#if !FL_OS_WINDOWS

#if !FL_OS_CS
int		sprintf_s( char* str, size_t size, const char* format, ... );
int 	vsprintf_s( char* str, size_t size, const char* format, va_list ap );
#endif
int		_vscprintf( const char* format, va_list ap );

void	strcpy_s( char* str, size_t size, const char* src );
void	strncpy_s( char* str, size_t size, const char* src, size_t src_size );

void	strcat_s( char* str, size_t bsize, const char* src );

//-----------------------------------------------------------------------------

inline int	_strcmpi( const char* a, const char* b )
{
	return	::strcasecmp( a, b );
}

//-----------------------------------------------------------------------------

#if 0
inline long long int _strtoi64( const char* ptr, char** endptr, int base )
{
	return	::strtoll( ptr, endptr, base );
}

inline long long int _strtoui64( const char* ptr, char** endptr, int base )
{
	return	::strtoull( ptr, endptr, base );
}
#endif

#endif

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

char*	str_sprintf_s( char* str, const char* end_str, const char* format, ... );

template<typename T, typename... A0>
int	sprintf_b( T& buffer, const char* format, A0&&... a0 )
{
	return	sprintf_s( reinterpret_cast<char*>(buffer.GetBuffer()), buffer.GetBufferSize(), format, FL_Forward<A0>(a0)... );
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}



