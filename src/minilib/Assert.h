// 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef FLATLIB_ASSERT_H_
#define FLATLIB_ASSERT_H_

#if defined(flENABLE_ASSERT) && flENABLE_ASSERT
# undef		NDEBUG
#endif

#include	<minilib/Platform.h>
#include	<assert.h>

#if FL_OS_ANDROID
# include	<android/log.h>
#endif

namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#ifndef	flENABLE_ASSERT
# define	flENABLE_ASSERT		FL_DEBUG
#endif


#if flENABLE_ASSERT
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


//--------------------------------------
#if FL_OS_ANDROID
//--------------------------------------

inline static void flatlib_assert( const char* text, const char* file, int line )
{
    __android_log_assert( text, "flatlib", "Assertion failure (%s) %s line: %d\n", text, file, line );
}
# define	FL_ASSERT(n)		if(!(n)){::flatlib::flatlib_assert( #n, __FILE__, __LINE__ );}

//--------------------------------------
#else
//--------------------------------------

# define	FL_ASSERT(n)		assert(n)

//--------------------------------------
#endif
//--------------------------------------

#if FL_OS_WINDOWS
# define	flABORT()		DebugBreak()
#else
# define	flABORT()		FL_ASSERT(0)
#endif

#define	flSTATIC_ASSERT(n,msg)	static_assert(n,msg)

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#else
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#define	FL_ASSERT(n)
#define	flSTATIC_ASSERT(n,msg)
#define	flABORT()

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#endif


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

#endif

