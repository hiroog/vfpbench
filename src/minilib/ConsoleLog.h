// 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef CONSOLEIO_H_
#define CONSOLEIO_H_

#include	<stdarg.h>

namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

class ConsoleLog {
public:
	static void Output( const char* msg );
	static void Format( const char* format, va_list args );
	static void Log( const char* format ... );
	static void Error( const char* format ... );
	static void Print( const char* format ... );
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

#define	FL_OUTPUT	::flatlib::ConsoleLog::Output
#define	FL_ERROR	::flatlib::ConsoleLog::Error
#define	FL_LOG		::flatlib::ConsoleLog::Log
#define	FL_PRINT	::flatlib::ConsoleLog::Print

#endif

