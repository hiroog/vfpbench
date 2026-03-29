// flatlib 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	"flatlib/core/thread/ThreadBase.h"
#include	"flatlib/core/thread/ThreadInstance.h"
#include	"flatlib/core/platform/WindowsAPI.h"

#if FL_OS_WINDOWS
# include	<process.h>
#elif FL_OS_UNIX
#else
# error "unknown platforms"
#endif


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace thread {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#if FL_OS_WINDOWS

ThreadInstance::ThreadInstance() : ThreadHandle( 0 )
{
}

#if 0
ThreadInstance::~ThreadInstance()
{
	Join();
}
#endif


void	ThreadInstance::Close()
{
	if( ThreadHandle != 0 ){
		CloseHandle( reinterpret_cast<HANDLE>(ThreadHandle) );
		ThreadHandle= 0;
	}
}


unsigned int	ThreadInstance::Join()
{
	if( ThreadHandle != 0 ){
		DWORD	exit_code= 0;
		WaitForSingleObject( reinterpret_cast<HANDLE>(ThreadHandle), INFINITE );
		GetExitCodeThread( reinterpret_cast<HANDLE>(ThreadHandle), &exit_code );
		Close();
		return	exit_code;
	}
	return	0;
}


unsigned int __stdcall	ThreadInstance::ExecFunction( void* arg )
{
	ThreadInstance*	This= reinterpret_cast<ThreadInstance*>( arg );
	This->Run();
	_endthreadex( 0 );
	return	0;
}


bool	ThreadInstance::Start()
{
	FL_ASSERT( ThreadHandle == 0 );
	uintptr_t	result= _beginthreadex(
			nullptr,
			0,
			ExecFunction,
			this,
			0,
			nullptr
		);
	if( result != 0 ){
		ThreadHandle= result;
		return	true;
	}
	return	false;
}

#endif


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#if FL_OS_UNIX

ThreadInstance::ThreadInstance() : Initialized( false )
{
}


#if 0
ThreadInstance::~ThreadInstance()
{
	Join();
}
#endif


void	ThreadInstance::Close()
{
}


unsigned int	ThreadInstance::Join()
{
	if( Initialized ){
		void*	exit_code= nullptr;
		int	result= pthread_join( ThreadHandle, &exit_code );
		FL_ASSERT( result == 0 );
		(void)result;
		Close();
		Initialized= false;
		return	static_cast<unsigned int>(reinterpret_cast<intptr_t>(exit_code));
	}
	return	0;
}


void*	ThreadInstance::ExecFunction( void* arg )
{
	ThreadInstance*	This= reinterpret_cast<ThreadInstance*>( arg );
	This->Run();
	pthread_exit( nullptr );
	return	nullptr;
}


bool	ThreadInstance::Start()
{
	FL_ASSERT( !Initialized );
	//pthread_attr_t	attr;
	//pthread_attr_init( &attr );
	int	result= pthread_create(
				&ThreadHandle,
				nullptr,
				ExecFunction,
				this
			);
	if( result == 0 ){
		Initialized= true;
		return	true;
	}
	return	false;
}

#endif


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

