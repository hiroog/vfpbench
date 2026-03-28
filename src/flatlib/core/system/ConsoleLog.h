// flatlib 2015 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#pragma once

//#include	<flatlib/core/CoreBase.h>
#include	<flatlib/core/system/Platform.h>
#include	<stdarg.h>


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace system {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

class ConsoleLog {
	bool	ConsoleOutputMode;
	bool	EnableVerbose;
	bool	EnableLog;
#if FL_OS_WINDOWS || FL_OS_LINUX || FL_OS_DARWIN || FL_OS_EMSCRIPTEN
	void*	FileHandle;
#endif
public:
	static ConsoleLog	Logger;
public:
	ConsoleLog();
	~ConsoleLog();
	void	SetConsoleOutputMode( bool mode );
	void	SetVerbose( bool mode )
	{
		EnableVerbose= mode;
	}
	void	SetDebugLog( bool mode )
	{
		EnableLog= mode;
	}
	void	SetLogFile( const char* base_name );
	void	CloseLogFile();
	void	LogString( const char* text );
	void	LogFormatV( const char* format, va_list ap );
	void	Verbose( const char* format ... )
	{
#if FL_DEBUG
		if( EnableVerbose ){
			va_list	ap;
			va_start( ap, format );
			LogFormatV( format, ap );
			va_end( ap );
		}
#endif
	}
	void	Log( const char* format ... );
	void	Error( const char* format ... );
	void	Print( const char* format ... );
};


inline ConsoleLog&	RConsoleLog()
{
	return	ConsoleLog::Logger;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}


#define	FL_VERBOSE(...)	flatlib::system::RConsoleLog().Verbose( __VA_ARGS__ )
#define	FL_LOG(...)		flatlib::system::RConsoleLog().Log( __VA_ARGS__ )
#define	FL_PRINT(...)	flatlib::system::RConsoleLog().Print( __VA_ARGS__ )
#define	FL_ERROR(...)	flatlib::system::RConsoleLog().Error( __VA_ARGS__ )



