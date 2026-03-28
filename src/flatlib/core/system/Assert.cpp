// flatlib 2015 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	"flatlib/core/CoreBase.h"
#include	"flatlib/core/system/Assert.h"
#include	"flatlib/core/system/ConsoleLog.h"

#if FL_OS_WINDOWS
#  include	"flatlib/core/platform/WindowsAPI.h"
#  include	<assert.h>
#elif FL_OS_ANDROID
#  include	<android/log.h>
# else
#  include	<assert.h>
#endif


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace system {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


void	Assert_Abort()
{
#if FL_OS_WINDOWS
	assert( 0 );
	DebugBreak();
#elif FL_CC_CLANG_GCC
	__builtin_trap();
#else
	assert( 0 );
#endif
}


void	Assert_Expr( bool result, const char* expr_text, const char* file_name, unsigned int line )
{
	if( !result ){
#if FL_OS_ANDROID
		__android_log_assert( expr_text, "flatlib", "Assertion failed: %s %s %d\n", expr_text, file_name, line );
#else
		FL_PRINT( "Assertion failed: %s %s %d\n", expr_text, file_name, line );
		Assert_Abort();
#endif
	}
}


void	Assert_Align( const void* ptr, unsigned int align, const char* expr_text, const char* file_name, unsigned int line )
{
	if( reinterpret_cast<uintptr_t>(ptr) & (align-1) ){
#if FL_OS_ANDROID
		__android_log_assert( expr_text, "flatlib", "Assertion failed: Align=%d %s %s %d\n", align, expr_text, file_name, line );
#else
		FL_PRINT( "Assertion failed: Align=%d %s %s %d\n", align, expr_text, file_name, line );
		Assert_Abort();
#endif
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

