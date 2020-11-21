// 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	"CoreLib.h"
#include	"Thread.h"


#define	FL_USE_WINAPI_THREAD_	1


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace thread {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Thread
//-----------------------------------------------------------------------------



void  Thread::Quit()
{
}

void  Thread::Join()
{
#if FL_OS_UNIX
	pthread_join( hThread, nullptr );
#endif
}

#if FL_OS_UNIX
void* Thread::ThreadFunc( void* arg )
{
	Thread*	This= reinterpret_cast<Thread*>( arg );
	This->Func( This->Arg );
	This->Exit();
	return	nullptr;
}
#endif


int  Thread::Run( void ( *exec)(void*), void* arg )
{
	FL_ASSERT( exec != nullptr );

	Arg= arg;
	Func= exec;

#if FL_OS_UNIX
	int	err= pthread_create( &hThread, nullptr, (void* (*)(void*))ThreadFunc, reinterpret_cast<void*>( this ) );

	// 0:success, else:errcode
	if( err ){
		FL_ERROR( "Thread: ERROR pthread_create %d\n", err );
		return	false;
	}
#endif
	return	true;
}




void  Thread::Exit()
{
#if FL_OS_UNIX
	pthread_exit( nullptr );
#endif
}



//-----------------------------------------------------------------------------
// Atomic
//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

