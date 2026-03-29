// flatlib 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#pragma once

#include	<flatlib/core/system/Platform.h>

#if FL_OS_WINDOWS
# define	WIN32_LEAN_AND_MEAN
# define	NOMINMAX
# include	<windows.h>
# undef		GetUserName
# undef		CreateDirectory
#endif


