// flatlib 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	"flatlib/core/CoreBase.h"
#include	"flatlib/core/system/ConsoleLog.h"
#include	"flatlib/core/time/DateTime.h"
#include	"flatlib/core/platform/WinString.h"

#if FL_OS_WINDOWS
# include	"flatlib/core/platform/WindowsAPI.h"
# include	<stdio.h>
# include	<io.h>
#endif

#if FL_OS_ANDROID
# include	<android/log.h>
#endif

#if FL_OS_WINDOWS || FL_OS_LINUX || FL_OS_DARWIN || FL_OS_EMSCRIPTEN
# include	<stdio.h>
# define	ENABLE_LOG_FILE		1
#elif FL_OS_CS || FL_OS_CS2
# include	<stdio.h>
#endif

#ifndef	ENABLE_LOG_FILE
# define	ENABLE_LOG_FILE		0
#endif


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace system {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

ConsoleLog	ConsoleLog::Logger;



ConsoleLog::ConsoleLog() :
		ConsoleOutputMode( false )
		,EnableVerbose( false )
		,EnableLog( FL_DEBUG != 0 )
#if ENABLE_LOG_FILE
		,FileHandle( nullptr )
#endif
{
}


ConsoleLog::~ConsoleLog()
{
	CloseLogFile();
}


void	ConsoleLog::SetConsoleOutputMode( bool mode )
{
	ConsoleOutputMode= mode;
#if FL_OS_WINDOWS
	if( mode ){
		HANDLE	handle= (HANDLE)_get_osfhandle( _fileno(stdout) );
		DWORD	mode= 0;
		if( ::GetConsoleMode( handle, &mode ) ){
			mode|= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
			::SetConsoleMode( handle, mode );
			FL_VERBOSE( "SetConsoleMode %x\n", mode );
		}
	}
#endif
}


void	ConsoleLog::CloseLogFile()
{
#if ENABLE_LOG_FILE
	if( FileHandle ){
		fclose( reinterpret_cast<FILE*>( FileHandle ) );
		FileHandle= nullptr;
	}
#endif
}


void	ConsoleLog::SetLogFile( const char* base_name )
{
#if ENABLE_LOG_FILE
	time::DateTime	ctime;
	time::GetLocalTime( ctime );
	constexpr int	FILE_BUFFER_SIZE= 256;
	char	file_name[FILE_BUFFER_SIZE];
	sprintf_s( file_name, FILE_BUFFER_SIZE-8, "%s_%04d%02d%02d_%02d%02d%02d.log",
				base_name,
				ctime.Year,
				ctime.Month,
				ctime.Day,
				ctime.Hour,
				ctime.Minute,
				ctime.Second
			);

# if FL_OS_WINDOWS
	CloseLogFile();
	FILE*	fp= nullptr;
	if( fopen_s( &fp, file_name, "w" ) == 0 ){
		FileHandle= fp;
	}
# else
	FILE*	fp= fopen( file_name, "w" );
	FileHandle= fp;
# endif
#endif
}


void	ConsoleLog::LogString( const char* text )
{
#if FL_OS_WINDOWS
	if( ConsoleOutputMode ){
		printf( "%s", text );
	}else{
		::OutputDebugStringA( text );
	}
#elif FL_OS_ANDROID
	__android_log_write( ANDROID_LOG_INFO, "flatlib", text );
#elif FL_OS_UNIX
	printf( "%s", text );
#else
	FL_ABORT();
#endif
#if ENABLE_LOG_FILE
	if( FileHandle ){
		fputs( text, reinterpret_cast<FILE*>( FileHandle ) );
	}
#endif
}


void	ConsoleLog::LogFormatV( const char* format, va_list ap )
{
#if ENABLE_LOG_FILE
	va_list	ap2;
	va_copy( ap2, ap );
#endif
#if FL_OS_WINDOWS
	if( ConsoleOutputMode ){
		vprintf( format, ap );
	}else{
		constexpr int	FORMAT_BUFFER_SIZE= 1024 * 4;
		char		buffer[FORMAT_BUFFER_SIZE];
		vsprintf_s( buffer, FORMAT_BUFFER_SIZE, format, ap );
		::OutputDebugStringA( buffer );
	}
#elif FL_OS_ANDROID
	__android_log_vprint( ANDROID_LOG_INFO, "flatlib", format, ap );
#elif FL_OS_UNIX
	vprintf( format, ap );
#else
	FL_ABORT();
#endif
#if ENABLE_LOG_FILE
	if( FileHandle ){
		vfprintf( reinterpret_cast<FILE*>( FileHandle ), format, ap2 );
	}
#endif
}


void	ConsoleLog::Log( const char* format ... )
{
#if !FL_RETAIL
	if( EnableLog ){
		va_list	ap;
		va_start( ap, format );
		LogFormatV( format, ap );
		va_end( ap );
	}
#endif
}


void	ConsoleLog::Error( const char* format ... )
{
	va_list	ap;
	va_start( ap, format );
	LogFormatV( format, ap );
	va_end( ap );
}


void	ConsoleLog::Print( const char* format ... )
{
	va_list	ap;
	va_start( ap, format );
	LogFormatV( format, ap );
	va_end( ap );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

