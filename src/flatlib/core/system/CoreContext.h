// flatlib 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#pragma once

#include	<flatlib/core/CoreBase.h>


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace memory {
	class MemoryAllocator;
}
namespace file {
	class FileSystem;
}

namespace db {
	class DBGlobal;
}

namespace system {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

class SystemInfo;
class ConsoleLog;

/*
	@code
		#include	<flatlib/core/system/CoreContext.h>
		auto*	context= flatlib::system::CreateContext();
		context->RConsoleLog().SetConsoleOutputMode( true );
		...
		flatib::ZRelease( context );
	@endcode
*/
class CoreContext {
protected:
	file::FileSystem*	iFileSystem;
	db::DBGlobal*		iDBGlobal;
	system::SystemInfo*	iSystemInfo;
public:
	CoreContext();
	virtual ~CoreContext();
	//-------------------------------------------------------------------------
	memory::MemoryAllocator&	RAllocator();
	system::ConsoleLog&			RConsoleLog();
	system::SystemInfo&			RSystemInfo();
	file::FileSystem&			RFileSystem();
	db::DBGlobal&				RDBGlobal();
	//-------------------------------------------------------------------------
	void	Release();
};


CoreContext*	CreateContext();
CoreContext&	RCore();
bool			IsInitialized();


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}


