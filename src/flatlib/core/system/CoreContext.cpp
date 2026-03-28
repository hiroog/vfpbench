// flatlib 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	"flatlib/core/system/CoreContext.h"
#include	"flatlib/core/memory/MemoryAllocator.h"
#include	"flatlib/core/system/SystemInfo.h"

#ifndef	FL_USE_FILESYSTEM
# define	FL_USE_FILESYSTEM	1
#endif
#ifndef	FL_USE_DATABASE
# define	FL_USE_DATABASE		1
#endif

#if FL_USE_FILESYSTEM
# include	"flatlib/core/file/FileSystem.h"
#endif
#if FL_USE_DATABASE
# include	"flatlib/core/db/DBGlobal.h"
#endif

#if FL_USE_FILESYSTEM
# if FL_OS_WINDOWS
#  include	"flatlib/core/file/FileSystem_Windows.h"
# elif FL_OS_CS
#  include	"flatlib/core/file/FileSystem_Default.h"
# elif FL_OS_CS2
#  include	"flatlib/core/file/FileSystem_CS2.h"
# elif FL_OS_UNIX
#  include	"flatlib/core/file/FileSystem_Unix.h"
# else
#  include	"flatlib/core/file/FileSystem_Default.h"
# endif
#endif

namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#if !FL_USE_FILESYSTEM
namespace file {
	class FileSystem {
	};
}
#endif

#if !FL_USE_DATABASE
namespace db {
	class DBGLobal {
	};
}
#endif

namespace system {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

static CoreContext*	iCurrentContext= nullptr;

//-----------------------------------------------------------------------------

CoreContext::CoreContext() :
	iFileSystem( nullptr ),
	iDBGlobal( nullptr ),
	iSystemInfo( nullptr )
{
}

CoreContext::~CoreContext()
{
}

void	CoreContext::Release()
{
	if( this == iCurrentContext ){
		iCurrentContext= nullptr;
	}
	FL_MEMORY::Delete_( this );
}

memory::MemoryAllocator&	CoreContext::RAllocator()
{
	return	memory::RAllocator();
}

system::ConsoleLog&		CoreContext::RConsoleLog()
{
	return	system::RConsoleLog();
}

system::SystemInfo&		CoreContext::RSystemInfo()
{
	return	*iSystemInfo;
}

file::FileSystem&		CoreContext::RFileSystem()
{
	return	*iFileSystem;
}

db::DBGlobal&		CoreContext::RDBGlobal()
{
	return	*iDBGlobal;
}


//-----------------------------------------------------------------------------

class CoreContextImpl : public CoreContext {
#if FL_USE_FILESYSTEM
	file::FileSystem		FileSystemInstance;
#endif
#if FL_USE_DATABASE
	db::DBGlobal			DBGlobalInstance;
#endif
	system::SystemInfo		SystemInfoInstance;
public:
	~CoreContextImpl()
	{
		Finalize();
	}
	void	Init()
	{
#if FL_USE_FILESYSTEM
		iFileSystem= &FileSystemInstance;
#endif
#if FL_USE_DATABASE
		iDBGlobal= &DBGlobalInstance;
#endif
		iSystemInfo= &SystemInfoInstance;
#if FL_USE_FILESYSTEM
# if FL_OS_WINDOWS
		iFileSystem->SetRootFS( FL_MEMORY::New<file::winfs::FileSystem>() );
# elif FL_OS_CS
		iFileSystem->SetRootFS( FL_MEMORY::New<file::deffs::FileSystem>() );
# elif FL_OS_CS2
		iFileSystem->SetRootFS( FL_MEMORY::New<file::cs2fs::FileSystem>() );
# elif FL_OS_UNIX
		iFileSystem->SetRootFS( FL_MEMORY::New<file::unixfs::FileSystem>() );
# else
		iFileSystem->SetRootFS( FL_MEMORY::New<file::deffs::FileSystem>() );
# endif
#endif
	}
	void	Finalize()
	{
	}
};


//-----------------------------------------------------------------------------

CoreContext*	CreateContext()
{
	FL_ASSERT( iCurrentContext == nullptr );
	auto*	context= FL_MEMORY::New<CoreContextImpl>();
	context->Init();
	iCurrentContext= context;
	return	context;
}


CoreContext&	RCore()
{
	FL_ASSERT( iCurrentContext != nullptr );
	return	*iCurrentContext;
}

bool	IsInitialized()
{
	return	iCurrentContext != nullptr;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

