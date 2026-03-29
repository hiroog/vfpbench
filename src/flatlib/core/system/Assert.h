// flatlib 2015 Hiroyuki Ogaaswara
// vim:ts=4 sw=4:

#pragma once

#include	<flatlib/core/system/Platform.h>


//-----------------------------------------------------------------------------
#if FL_RETAIL
//-----------------------------------------------------------------------------

#define	FL_ABORT()		flatlib::system::Assert_Abort()
#define	FL_ASSERT(...)
#define	FL_ASSERT_ALIGN(...)
 
//-----------------------------------------------------------------------------
#else
//-----------------------------------------------------------------------------

#define	FL_ABORT()		flatlib::system::Assert_Abort()
#define	FL_ASSERT(exp)	flatlib::system::Assert_Expr((exp), #exp, __FILE__, __LINE__)
#define	FL_ASSERT_ALIGN(ptr,align)	flatlib::system::Assert_Align( reinterpret_cast<const void*>(ptr), align, #ptr, __FILE__, __LINE__)

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------

namespace flatlib {
	namespace system {
		void	Assert_Abort();
		void	Assert_Expr( bool result, const char* expr_text, const char* file_name, unsigned int line );
		void	Assert_Align( const void* ptr, unsigned int align, char* ptr_text, const char* file_name, unsigned int line );
	}
}

