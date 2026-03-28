// flatlib 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#pragma once

#include	<flatlib/core/system/Platform.h>
#include	<flatlib/core/system/Types.h>
#include	<flatlib/core/memory/MemoryAllocator.h>

#if FL_OS_UNIX
# include	<pthread.h>
#endif


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace thread {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


class ThreadInstance {
#if FL_OS_WINDOWS
	static unsigned int	__stdcall ExecFunction( void* arg );
	uintptr_t		ThreadHandle;
#elif FL_OS_UNIX
	static void*	ExecFunction( void* arg );
	pthread_t		ThreadHandle;
	bool			Initialized;
#else
# error "unknown platforms"
#endif
protected:
	void			Close();
public:
	ThreadInstance();
	virtual ~ThreadInstance()
	{
		Join();
	}
	bool			Start();
	unsigned int	Join();
	virtual	void	Run()
	{
	}
};


template<typename T>
class ThreadFunction : public ThreadInstance {
	T	Func;
public:
	explicit ThreadFunction( T&& func ) : Func( FL_Forward<T>(func) )
	{
	}
	void	Run() override
	{
		Func();
	}
};


/*!
	@code
		auto*	thread= thread::CreateThreadFunction( []{ ... });
	@endcode
*/
template<typename T>
inline ThreadInstance*	CreateThreadFunction( T&& func )
{
	return	FL_MEMORY::New<ThreadFunction<T>>( FL_Forward<T>(func) );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

