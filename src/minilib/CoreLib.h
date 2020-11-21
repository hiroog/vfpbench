// 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef FLATLIB_CORELIB_H_
#define FLATLIB_CORELIB_H_

#include	<minilib/Platform.h>

#if FL_OS_WINDOWS
# include	<windows.h>
#endif

#if FL_OS_DARWIN
# include	<TargetConditionals.h>
#endif

#if FL_OS_UNIX
# include	<new>
# include	<stddef.h>
# include	<stdint.h>
# include	<stdarg.h>
#endif

namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template<typename T> struct FL_RM_REF { typedef T Type; };
template<typename T> struct FL_RM_REF<T&> { typedef T Type; };
template<typename T> struct FL_RM_REF<T&&> { typedef T Type; };
template<typename T>
inline constexpr typename FL_RM_REF<T>::Type&& FL_Move( T&& src )
{
	return	static_cast<typename FL_RM_REF<T>::Type&&>( src );
}
template<typename T>
inline constexpr T&& FL_Forward( typename FL_RM_REF<T>::Type& src )
{
	return	static_cast<T&&>( src );
}
template<typename T>
inline constexpr T&& FL_Forward( typename FL_RM_REF<T>::Type&& src )
{
	return	static_cast<T&&>( src );
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

#include	<minilib/ConsoleLog.h>
#include	<minilib/Assert.h>
#include	<minilib/PlatformString.h>

#endif

