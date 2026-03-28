// flatlib 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#pragma once


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace thread {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


template<typename T>
class ScopedLock {
	T*	LockObject;
public:
	ScopedLock( T& lock_object ) : LockObject( &lock_object )
	{
		LockObject->Lock();
	}
	~ScopedLock()
	{
		LockObject->Unlock();
	}
};



template<typename T>
class ScopedSharedLock {
	T*	LockObject;
public:
	ScopedSharedLock( T& lock_object ) : LockObject( &lock_object )
	{
		LockObject->SharedLock();
	}
	~ScopedSharedLock()
	{
		LockObject->SharedUnlock();
	}
};



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

