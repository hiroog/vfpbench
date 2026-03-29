// flatlib 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	"flatlib/core/thread/ThreadBase.h"
#include	"flatlib/core/thread/Mutex.h"

#if FL_OS_WINDOWS
# include	"flatlib/core/platform/WindowsAPI.h"
#endif


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace thread {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


Mutex::Mutex()
{
#if FL_OS_WINDOWS
	LockHandle= reinterpret_cast<uintptr_t>(::CreateMutex( nullptr, false, nullptr ));
#elif FL_OS_UNIX
	int	result= pthread_mutex_init( &LockObject, nullptr );
	FL_ASSERT( result == 0 );
	(void)result;
#endif
}


Mutex::~Mutex()
{
#if FL_OS_WINDOWS
	if( LockHandle ){
		CloseHandle( reinterpret_cast<HANDLE>(LockHandle) );
		LockHandle= 0;
	}
#elif FL_OS_UNIX
	pthread_mutex_destroy( &LockObject );
#endif
}


void	Mutex::Lock()
{
#if FL_OS_WINDOWS
	WaitForSingleObject( reinterpret_cast<HANDLE>(LockHandle), INFINITE );
#elif FL_OS_UNIX
	pthread_mutex_lock( &LockObject );
#endif
}


void	Mutex::Unlock()
{
#if FL_OS_WINDOWS
	ReleaseMutex( reinterpret_cast<HANDLE>(LockHandle) );
#elif FL_OS_UNIX
	pthread_mutex_unlock( &LockObject );
#endif
}


bool	Mutex::TryLock()
{
#if FL_OS_WINDOWS
	return	WaitForSingleObject( reinterpret_cast<HANDLE>(LockHandle), 0 ) == WAIT_OBJECT_0;
#elif FL_OS_UNIX
	return	pthread_mutex_trylock( &LockObject ) == 0;
	//return	pthread_mutex_trylock( &LockObject ) != EBUSY;
#endif
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

