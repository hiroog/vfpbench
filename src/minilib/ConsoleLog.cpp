// 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	"CoreLib.h"
#include	"ConsoleLog.h"
#include	<stdio.h>

#if FL_OS_ANDROID
# include	<android/log.h>
#endif

namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void ConsoleLog::Output( const char* msg )
{
#if FL_OS_ANDROID
	__android_log_write( ANDROID_LOG_INFO, "vfpbench", msg );
#else
	printf( "%s", msg );
#endif
}

void ConsoleLog::Format( const char* format, va_list args )
{
#if FL_OS_ANDROID
	__android_log_vprint( ANDROID_LOG_INFO, "vfpbench", format, args );
#else
	vprintf( format, args );
#endif
}

void ConsoleLog::Log( const char* format ... )
{
	if( FL_DEBUG ){
		va_list	ap;
		va_start( ap, format );
		Format( format, ap );
		va_end( ap );
	}
}

void ConsoleLog::Error( const char* format ... )
{
	va_list	ap;
	va_start( ap, format );
	Format( format, ap );
	va_end( ap );
}

void ConsoleLog::Print( const char* format ... )
{
	va_list	ap;
	va_start( ap, format );
	Format( format, ap );
	va_end( ap );
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

