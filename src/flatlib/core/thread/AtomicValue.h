// flatlib 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#pragma once

#include	<flatlib/core/system/Platform.h>

#define	FL_USE_CPP_ATOMIC	0

#if FL_USE_CPP_ATOMIC
# include	<atomic>
#else
# if FL_CC_VC
#  include	<intrin.h>
# endif
#endif


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace thread {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#if FL_USE_CPP_ATOMIC
//-----------------------------------------------------------------------------

template<typename T>
class AtomicValue {
	std::atomic<T>	Value;
	static const std::memory_order	DEFAULT_MEMORY_ORDER_LD= std::memory_order_seq_cst;
	static const std::memory_order	DEFAULT_MEMORY_ORDER_ST= std::memory_order_seq_cst;
public:
	AtomicValue() : Value( 0 )
	{
	}
	AtomicValue( T value ) : Value( value )
	{
	}
	T	Get() const
	{
		return	Value.load( DEFAULT_MEMORY_ORDER_LD );
	}
	T	Get_ACQ() const
	{
		return	Value.load( std::memory_order_acquire );
	}
	void	Set( T value )
	{
		Value.store( value, DEFAULT_MEMORY_ORDER_ST );
	}
	void	Set_REL( T value )
	{
		Value.store( value, std::memory_order_release );
	}
	T	Increment()
	{
		return	++Value;
	}
	T	Decrement()
	{
		return	--Value;
	}
	T	Exchange( T new_value )
	{
		return	Value.exchange( new_value, DEFAULT_MEMORY_ORDER_ST );
	}
	T	LoadAdd( T value )
	{
		return	Value++;
	}
	T	LoadSub( T value )
	{
		return	Value--;
	}
	T	LoadAnd( T value )
	{
		return	Value.fetch_and( value, DEFAULT_MEMORY_ORDER_ST );
	}
	T	LoadOr( T value )
	{
		return	Value.fetch_or( value, DEFAULT_MEMORY_ORDER_ST );
	}
	T	LoadXor( T value )
	{
		return	Value.fetch_xor( value, DEFAULT_MEMORY_ORDER_ST );
	}
	bool	CAS( T new_value, T comp_value )
	{
		return	Value.compare_exchange_weak( &comp_value, new_value, DEFAULT_MEMORY_ORDER_LD, DEFAULT_MEMORY_ORDER_ST );
	}
	bool	CAS_ACQ( T new_value, T comp_value )
	{
		return	Value.compare_exchange_weak( &comp_value, new_value, std::memory_order_acquire, std::memory_order_release );
	}
	bool	CAS_Strong( T new_value, T comp_value )
	{
		return	Value.compare_exchange_strong( &comp_value, new_value, DEFAULT_MEMORY_ORDER_LD, DEFAULT_MEMORY_ORDER_ST );
	}
	bool	CAS_Strong_ACQ( T new_value, T comp_value )
	{
		return	Value.compare_exchange_strong( &comp_value, new_value, std::memory_order_acquire, std::memory_order_release );
	}
};


//-----------------------------------------------------------------------------
#else
//-----------------------------------------------------------------------------

#if FL_CC_VC

template<typename T,size_t SIZE= sizeof(T)>
class AtomicValue {
};

template<typename T>
class AtomicValue<T,4> {
	T	Value;
public:
	AtomicValue()
	{
	}
	AtomicValue( T val ) : Value( val )
	{
	}
	T	Get() const
	{
#if FL_CPU_X64 || FL_CPU_X86
		return	Value;
#else
		return	const_cast<AtomicValue<T,4>*>(this)->Exchange( Value );
#endif
	}
	T	Get_ACQ() const
	{
#if FL_CPU_X64 || FL_CPU_X86
		return	Value;
#else
		return	const_cast<AtomicValue<T,4>*>(this)->Exchange( Value );
#endif
	}
	void	Set( T value )
	{
		Exchange( value );
	}
	void	Set_REL( T value )
	{
		Value= value;
	}
	/*!
		@return	++Value
	*/
	T	Increment()
	{
		return	static_cast<T>(_InterlockedIncrement( reinterpret_cast<long*>( &Value ) ));
	}
	/*!
		@return	--Value
	*/
	T	Decrement()
	{
		return	static_cast<T>(_InterlockedDecrement( reinterpret_cast<long*>( &Value ) ));
	}
	/*!
		@param[in]	new_value
		@return	old_value
	*/
	T	Exchange( T new_value )
	{
		return	static_cast<T>(_InterlockedExchange( reinterpret_cast<long*>( &Value ), new_value ));
	}
	/*!
		@param[in]	value
		@code
			tmp= Value;		// load
			Value+= value;	// add
			return	tmp;
		@endcode
	*/
	T	LoadAdd( T value )
	{
		return	static_cast<T>(_InterlockedExchangeAdd( reinterpret_cast<long*>( &Value ), value ));
	}
	T	LoadSub( T value )
	{
		return	static_cast<T>(_InterlockedExchangeAdd( reinterpret_cast<long*>( &Value ), (T)-value ));
	}
	T	LoadAnd( T value )
	{
		return	static_cast<T>(_InterlockedAnd( reinterpret_cast<long*>( &Value ), value ));
	}
	T	LoadOr( T value )
	{
		return	static_cast<T>(_InterlockedOr( reinterpret_cast<long*>( &Value ), value ));
	}
	T	LoadXor( T value )
	{
		return	static_cast<T>(_InterlockedXor( reinterpret_cast<long*>( &Value ), value ));
	}
	/*!
		@param[in]	new_value
		@param[in]	comp_value
		@return	old_value
	*/
	bool	CAS( T new_value, T comp_value )
	{
		return	static_cast<T>(_InterlockedCompareExchange( reinterpret_cast<long*>( &Value ), new_value, comp_value )) == comp_value;
	}
	bool	CAS_ACQ( T new_value, T comp_value )
	{
		return	static_cast<T>(_InterlockedCompareExchangeAcquire( reinterpret_cast<long*>( &Value ), new_value, comp_value )) == comp_value;
	}
	bool	CAS_Strong( T new_value, T comp_value )
	{
		return	CAS( new_value, comp_value );
	}
	bool	CAS_Strong_ACQ( T new_value, T comp_value )
	{
		return	CAS_ACQ( new_value, comp_value );
	}
};

template<typename T>
class AtomicValue<T,8> {
	T	Value;
public:
	AtomicValue()
	{
	}
	AtomicValue( T val ) : Value( val )
	{
	}
	T	Get() const
	{
#if FL_CPU_X64 || FL_CPU_X86
		return	Value;
#else
		return	Exchange( Value );
#endif
	}
	T	Get_ACQ() const
	{
#if FL_CPU_X64 || FL_CPU_X86
		return	Value;
#else
		return	Exchange( Value );
#endif
	}
	void	Set( T value )
	{
		Exchange( value );
	}
	void	Set_REL( T value )
	{
		Value= value;
	}
	T	Increment()
	{
		return	static_cast<T>(_InterlockedIncrement64( reinterpret_cast<long long*>( &Value ) ));
	}
	T	Decrement()
	{
		return	static_cast<T>(_InterlockedDecrement64( reinterpret_cast<long long*>( &Value ) ));
	}
	T	Exchange( T new_value )
	{
		return	static_cast<T>(_InterlockedExchange64( reinterpret_cast<long long*>( &Value ), new_value ));
	}
	T	LoadAdd( T value )
	{
		return	static_cast<T>(_InterlockedExchangeAdd64( reinterpret_cast<long*>( &Value ), value ));
	}
	T	LoadSub( T value )
	{
		return	static_cast<T>(_InterlockedExchangeAdd64( reinterpret_cast<long*>( &Value ), (T)-value ));
	}
	T	LoadAnd( T value )
	{
		return	static_cast<T>(_InterlockedAnd64( reinterpret_cast<long*>( &Value ), value ));
	}
	T	LoadOr( T value )
	{
		return	static_cast<T>(_InterlockedOr64( reinterpret_cast<long*>( &Value ), value ));
	}
	T	LoadXor( T value )
	{
		return	static_cast<T>(_InterlockedXor64( reinterpret_cast<long*>( &Value ), value ));
	}
	bool	CAS( T new_value, T comp_value )
	{
		return	static_cast<T>(_InterlockedCompareExchange64( reinterpret_cast<long long*>( &Value ), new_value, comp_value )) == comp_value;
	}
	bool	CAS_ACQ( T new_value, T comp_value )
	{
		return	static_cast<T>(_InterlockedCompareExchangeAcquire64( reinterpret_cast<long long*>( &Value ), new_value, comp_value )) == comp_value;
	}
	bool	CAS_Strong( T new_value, T comp_value )
	{
		return	CAS( new_value, comp_value );
	}
	bool	CAS_Strong_ACQ( T new_value, T comp_value )
	{
		return	CAS_ACQ( new_value, comp_value );
	}
};

#endif

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#if FL_CC_CLANG

template<typename T>
class AtomicValue {
	_Atomic(T)	Value;
public:
	AtomicValue()
	{
	}
	AtomicValue( T val )
	{
		__c11_atomic_init( &Value, val );
	}
	T	Get() const
	{
		auto*	_This= const_cast<AtomicValue<T>*>(this);
		return	__c11_atomic_load( &_This->Value, __ATOMIC_SEQ_CST );
	}
	T	Get_ACQ() const
	{
		auto*	_This= const_cast<AtomicValue<T>*>(this);
		return	__c11_atomic_load( &_This->Value, __ATOMIC_ACQUIRE );
	}
	void	Set( T value )
	{
		__c11_atomic_store( &Value, value, __ATOMIC_SEQ_CST );
	}
	void	Set_REL( T value )
	{
		__c11_atomic_store( &Value, value, __ATOMIC_RELEASE );
	}
	T	Increment()
	{
		return	__c11_atomic_fetch_add( &Value, 1, __ATOMIC_SEQ_CST ) + 1;
	}
	T	Decrement()
	{
		return	__c11_atomic_fetch_sub( &Value, 1, __ATOMIC_SEQ_CST ) - 1;
	}
	T	Exchange( T new_value )
	{
		return	__c11_atomic_exchange( &Value, new_value, __ATOMIC_SEQ_CST );
	}
	T	LoadAdd( T value )
	{
		return	__c11_atomic_fetch_add( &Value, value, __ATOMIC_SEQ_CST );
	}
	T	LoadSub( T value )
	{
		return	__c11_atomic_fetch_sub( &Value, value, __ATOMIC_SEQ_CST );
	}
	T	LoadAnd( T value )
	{
		return	__c11_atomic_fetch_and( &Value, value, __ATOMIC_SEQ_CST );
	}
	T	LoadOr( T value )
	{
		return	__c11_atomic_fetch_or( &Value, value, __ATOMIC_SEQ_CST );
	}
	T	LoadXor( T value )
	{
		return	__c11_atomic_fetch_xor( &Value, value, __ATOMIC_SEQ_CST );
	}
	bool	CAS( T new_value, T comp_value )
	{
		return	__c11_atomic_compare_exchange_weak( &Value, &comp_value, new_value, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST );
	}
	bool	CAS_ACQ( T new_value, T comp_value )
	{
		return	__c11_atomic_compare_exchange_weak( &Value, &comp_value, new_value, __ATOMIC_ACQUIRE, __ATOMIC_RELEASE );
	}
	bool	CAS_Strong( T new_value, T comp_value )
	{
		return	__c11_atomic_compare_exchange_strong( &Value, &comp_value, new_value, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST );
	}
	bool	CAS_Strong_ACQ( T new_value, T comp_value )
	{
		return	__c11_atomic_compare_exchange_strong( &Value, &comp_value, new_value, __ATOMIC_ACQUIRE, __ATOMIC_RELEASE );
	}
};

#endif

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#if FL_CC_GCC

template<typename T>
class AtomicValue {
	T	Value;
public:
	AtomicValue()
	{
	}
	AtomicValue( T val )
	{
		Set( val );
	}
	T	Get() const
	{
		auto*	_This= const_cast<AtomicValue<T>*>(this);
		return	__atomic_load_n( &_This->Value, __ATOMIC_SEQ_CST );
	}
	T	Get_ACQ() const
	{
		auto*	_This= const_cast<AtomicValue<T>*>(this);
		return	__atomic_load_n( &_This->Value, __ATOMIC_ACQUIRE );
	}
	void	Set( T value )
	{
		__atomic_store_n( &Value, value, __ATOMIC_SEQ_CST );
	}
	void	Set_REL( T value )
	{
		__atomic_store_n( &Value, value, __ATOMIC_RELEASE );
	}
	T	Increment()
	{
		return	__atomic_add_fetch( &Value, 1, __ATOMIC_SEQ_CST );
	}
	T	Decrement()
	{
		return	__atomic_sub_fetch( &Value, 1, __ATOMIC_SEQ_CST );
	}
	T	Exchange( T new_value )
	{
		return	__atomic_exchange_n( &Value, new_value, __ATOMIC_SEQ_CST );
	}
	T	LoadAdd( T value )
	{
		return	__atomic_fetch_add( &Value, value, __ATOMIC_SEQ_CST );
	}
	T	LoadSub( T value )
	{
		return	__atomic_fetch_sub( &Value, value, __ATOMIC_SEQ_CST );
	}
	T	LoadAnd( T value )
	{
		return	__atomic_fetch_and( &Value, value, __ATOMIC_SEQ_CST );
	}
	T	LoadOr( T value )
	{
		return	__atomic_fetch_or( &Value, value, __ATOMIC_SEQ_CST );
	}
	T	LoadXor( T value )
	{
		return	__atomic_fetch_xor( &Value, value, __ATOMIC_SEQ_CST );
	}
	bool	CAS( T new_value, T comp_value )
	{
		return	__atomic_compare_exchange_n( &Value, &comp_value, new_value, true, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST );
	}
	bool	CAS_ACQ( T new_value, T comp_value )
	{
		return	__atomic_compare_exchange_n( &Value, &comp_value, new_value, true, __ATOMIC_ACQUIRE, __ATOMIC_RELEASE );
	}
	bool	CAS_Strong( T new_value, T comp_value )
	{
		return	__atomic_compare_exchange_n( &Value, &comp_value, new_value, true, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST );
	}
	bool	CAS_Strong_ACQ( T new_value, T comp_value )
	{
		return	__atomic_compare_exchange_n( &Value, &comp_value, new_value, true, __ATOMIC_ACQUIRE, __ATOMIC_RELEASE );
	}
};

#endif


//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}


