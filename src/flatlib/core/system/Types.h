// flatlib 2016 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#pragma once

#include	<flatlib/core/system/Platform.h>
#include	<stdint.h>
#include	<stddef.h>
#include	<stdarg.h>


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


#if FL_CC_VC
# if _MSC_VER <= 1800 // VS2013
#  define	alignas(n)	__declspec(align(n))
#  define	alignof(n)	__alignof(n)
# endif
//#  define	FL_RESTRICT		__restrict
# if FL_CPU_PTR64
typedef	long long			ssize_t;
# else
typedef	int					ssize_t;
# endif
#endif

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

enum : unsigned int {
	FL_DEFAULT_ALIGN_SIZE	=	sizeof(void*),
};

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

#if 0
template<typename T>
inline void FL_Swap( T& dest, T& src )
{
	T	tmp= static_cast<T&&>(dest);
	dest= static_cast<T&&>(src);
	src= static_cast<T&&>(tmp);
}
template<typename T>
inline void FL_POD_Swap( T& dest, T& src )
{
	T	tmp= dest;
	dest= src;
	src= tmp;
}
#endif

template<typename T>
inline void	ZRelease( T*& ptr )
{
	if( ptr ){
		ptr->Release();
		ptr= nullptr;
	}
}

#if defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard)
# define	FL_ATTR_NODISCARD	[[nodiscard]]
#endif

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

