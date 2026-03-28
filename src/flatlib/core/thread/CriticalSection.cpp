// flatlib 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	"flatlib/core/thread/ThreadBase.h"
#include	"flatlib/core/thread/CriticalSection.h"
#include	"flatlib/core/system/Assert.h"

#if FL_OS_WINDOWS
# include	"flatlib/core/platform/WindowsAPI.h"
#endif



namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace thread {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


CriticalSection::CriticalSection()
{
#if FL_OS_WINDOWS
	FL_ASSERT( sizeof(LockObject) >= sizeof(CRITICAL_SECTION) );
	CRITICAL_SECTION*	lock_object= reinterpret_cast<CRITICAL_SECTION*>( &LockObject );
	InitializeCriticalSection( lock_object );
#endif
}

CriticalSection::~CriticalSection()
{
#if FL_OS_WINDOWS
	CRITICAL_SECTION*	lock_object= reinterpret_cast<CRITICAL_SECTION*>( &LockObject );
	DeleteCriticalSection( lock_object );
#endif
}


bool	CriticalSection::TryLock()
{
#if FL_OS_WINDOWS
	CRITICAL_SECTION*	lock_object= reinterpret_cast<CRITICAL_SECTION*>( &LockObject );
	return	TryEnterCriticalSection( lock_object ) != 0;
#else
	return	LockObject.TryLock();
#endif
}


void	CriticalSection::Lock()
{
#if FL_OS_WINDOWS
	CRITICAL_SECTION*	lock_object= reinterpret_cast<CRITICAL_SECTION*>( &LockObject );
	EnterCriticalSection( lock_object );
#else
	LockObject.Lock();
#endif
}


void	CriticalSection::Unlock()
{
#if FL_OS_WINDOWS
	CRITICAL_SECTION*	lock_object= reinterpret_cast<CRITICAL_SECTION*>( &LockObject );
	LeaveCriticalSection( lock_object );
#else
	LockObject.Unlock();
#endif
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

