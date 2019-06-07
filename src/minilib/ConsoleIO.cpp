// 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	"CoreLib.h"
#include	"ConsoleIO.h"
#include	<stdio.h>

#if flOS_ANDROID
# include	<android/log.h>
#endif

namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void ConsoleIO::Output( const char* msg )
{
#if flOS_ANDROID
	__android_log_write( ANDROID_LOG_INFO, "vfpbench", msg );
#else
	printf( "%s", msg );
#endif
}

void ConsoleIO::Format( const char* format, va_list args )
{
#if flOS_ANDROID
	__android_log_vprint( ANDROID_LOG_INFO, "vfpbench", format, args );
#else
	vprintf( format, args );
#endif
}

void ConsoleIO::Log( const char* format ... )
{
	if( flDEBUG ){
		va_list	ap;
		va_start( ap, format );
		Format( format, ap );
		va_end( ap );
	}
}

void ConsoleIO::Error( const char* format ... )
{
	va_list	ap;
	va_start( ap, format );
	Format( format, ap );
	va_end( ap );
}

void ConsoleIO::Print( const char* format ... )
{
	va_list	ap;
	va_start( ap, format );
	Format( format, ap );
	va_end( ap );
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

