// flatlib 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#pragma once

#include	<flatlib/core/system/Platform.h>

#if FL_OS_WINDOWS
#elif FL_OS_UNIX
# include	<flatlib/core/thread/Mutex.h>
#else
# include	<flatlib/core/thread/SpinLock.h>
#endif


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace thread {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


class CriticalSection {
#if FL_OS_WINDOWS
	intptr_t		LockObject[8];	// 32/64 byte (24/40)
#elif FL_OS_UNIX
	Mutex	LockObject;
#else
	SpinLock	LockObject;
#endif
public:
	CriticalSection();
	~CriticalSection();
	void	Lock();
	void	Unlock();
	bool	TryLock();	// 0=busy, 1=enter
};



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

