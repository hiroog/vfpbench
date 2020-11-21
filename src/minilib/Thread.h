// 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef	FLATLIB_THREAD_THREAD_H_
#define	FLATLIB_THREAD_THREAD_H_

#include	<minilib/Platform.h>
#if FL_OS_UNIX
# include	<pthread.h>
#endif

namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace thread {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


class Thread {
private:
#if FL_OS_UNIX
	pthread_t	hThread;
#endif
	void	(*Func)( void* arg );
	void*	Arg;
private:
#if FL_OS_UNIX
	static void*	ThreadFunc( void* arg );
#endif
public:
	Thread() : Func( nullptr ), Arg( nullptr )
	{
	}
	~Thread()
	{
		Quit();
	}
	void 	Quit();
	int 	Run( void (*exec)( void* arg ), void* arg );
	void 	Exit();
	void 	Join();
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


#if FL_OS_WINDOWS

class ThreadLock {
	CRITICAL_SECTION	LockData;
public:
	ThreadLock()
	{
		InitializeCriticalSection( &LockData );
	}
	~ThreadLock()
	{
	}
	void	Lock()
	{
		EnterCriticalSection( &LockData );
	}
	void	Unlock()
	{
		LeaveCriticalSection( &LockData );
	}
};

#elif FL_OS_UNIX

class ThreadLock {
	pthread_mutex_t		LockData;
public:
	ThreadLock()
	{
		pthread_mutex_init( &LockData, nullptr );
	}
	~ThreadLock()
	{
		pthread_mutex_destroy( &LockData );
	}
	void	Lock()
	{
		pthread_mutex_lock( &LockData );
	}
	void	Unlock()
	{
		pthread_mutex_unlock( &LockData );
	}
};

#else
# error	"unknown platform"
#endif


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template<typename T>
class ScopedLock {
	T&	Lock;
public:
	ScopedLock( T& lock ) : Lock( lock )
	{
		Lock.Lock();
	}
	~ScopedLock()
	{
		Lock.Unlock();
	}
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}



#endif





