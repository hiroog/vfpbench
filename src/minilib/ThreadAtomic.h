// 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef	FLATLIB_THREAD_THREADATOMIC_H_
#define	FLATLIB_THREAD_THREADATOMIC_H_

#include	<minilib/Platform.h>
//#include	<minilib/ConsoleIO.h>
#include	<atomic>


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace thread {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template<typename T>
class Atomic {
	std::atomic<T>	Value;
	static const std::memory_order	DEFAULT_MEMORY_ORDER_LD= std::memory_order_seq_cst;
	static const std::memory_order	DEFAULT_MEMORY_ORDER_ST= std::memory_order_seq_cst;
public:
	Atomic() : Value( 0 )
	{
	}
	Atomic( T value ) : Value( value )
	{
	}
	void	Increment()
	{
		Value++;
	}
	void Set( T value )
	{
		Value.store( value, DEFAULT_MEMORY_ORDER_ST );
	}
	T	Get() const
	{
		return	Value.load( DEFAULT_MEMORY_ORDER_LD );
	}
	T	operator++()
	{
		return	++Value;
	}
	T	operator++( int )
	{
		//FL_LOG( "Step=%d\n", Value.load( DEFAULT_MEMORY_ORDER_LD ) );
		return	Value++;
	}
	T	operator+=( T value )
	{
		return	Value+= value;
	}
	T	operator=( T value )
	{
		Set( value );
		return	Get();
	}
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}



#endif





