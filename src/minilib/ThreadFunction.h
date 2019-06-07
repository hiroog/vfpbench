// 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4:

#ifndef	FLATLIB_THREAD_FUNCTION_H_
#define	FLATLIB_THREAD_FUNCTION_H_

#include	<minilib/Thread.h>
#include	<minilib/MemoryAllocator.h>


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace thread {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

class ThreadFunctionBase {
public:
	ThreadFunctionBase()
	{
	}
	virtual ~ThreadFunctionBase()
	{
	}
	virtual void 		Run()= 0;
	virtual void 		Join()= 0;
	virtual void 		Quit()= 0;
	virtual Thread& 	RThread()= 0;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template<typename FT>
class ThreadFunction : public ThreadFunctionBase {
	FT		Func;
	Thread	Instance;
	static void 	ThreadFunc( void* arg )
	{
		ThreadFunction<FT>*	pThis= reinterpret_cast<ThreadFunction<FT>*>( arg );
		(pThis->Func)();
	}
public:
	ThreadFunction( FT exec ) : Func( exec )
	{
	}
	~ThreadFunction()
	{
		Quit();
	}
	void 	Run()
	{
		Instance.Run( ThreadFunc, this );
	}
	void 	Join()
	{
		Instance.Join();
	}
	void 	Quit()
	{
		Instance.Quit();
	}
	Thread& 	RThread()
	{
		return	Instance;
	}
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template<typename FT>
ThreadFunction<FT>* CreateThreadFunction( FT func )
{
	return	flatlib::memory::New<ThreadFunction<FT>>( func );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

#endif

