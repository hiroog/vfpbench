// 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef	MULTI_ADAPTER_H_
#define	MULTI_ADAPTER_H_

#include	<minilib/CoreLib.h>
#include	<minilib/Thread.h>
#include	<minilib/FixedArray.h>
#include	"TestBase.h"


template<typename T>
class MultiAdapter : public ITestBase {
public:
private:
	flatlib::util::FixedArray<T>	InstanceArray;
	flatlib::util::FixedArrayPOD<flatlib::thread::ThreadFunctionBase*>	ThreadArray;
public:

	MultiAdapter( unsigned int group ) : ITestBase( true, group )
	{
		unsigned int	core= flatlib::Info.GetThreadCount( group );
		InstanceArray.Init( core );
		ThreadArray.Init( core );
		ThreadArray.ClearZero();
		unsigned int	instance_count= InstanceArray.GetSize();
		for( unsigned int ci= 0 ; ci< instance_count ; ci++ ){
			InstanceArray[ci].SetIsMultithread( true );
			InstanceArray[ci].SetCoreGroup( group );
		}
	}
	~MultiAdapter()
	{
		Quit();
	}

	void	Run_Direct()
	{
		flASSERT( 0 );
	}

	void	Quit()
	{
		FL_LOG( "MultiAdapter QUIT\n" );
		Join();
		InstanceArray.Clear();
		ThreadArray.Clear();
		FL_LOG( "MultiAdapter QUIT COMPLETE\n" );
	}

	void	Join()
	{
		unsigned int	thread_count= ThreadArray.GetSize();
		for( unsigned int ci= 0 ; ci< thread_count ; ci++ ){
			if( ThreadArray[ci] ){
				FL_LOG( "MultiAdapter JOIN %d\n", ci );
				ThreadArray[ci]->Join();
				flatlib::memory::SafeDelete( ThreadArray[ci] );
				FL_LOG( "MultiAdapter JOIN QUIT %d\n", ci );
			}
		}
	}

	void	Run() override
	{
		unsigned int	thread_count= ThreadArray.GetSize();
		for( unsigned int ci= 0 ; ci< thread_count ; ci++ ){
			ThreadArray[ci]= flatlib::thread::CreateThreadFunction(
				[=](){
					FL_LOG( "Run : In MultiThread %d\n", ci );
					this->SetCpuAffinity();
					this->InstanceArray[ci].SetIsMultithread( true );
					this->InstanceArray[ci].Run();
					FL_LOG( "End MultiThread %d\n", ci );
				});
		}
		for( unsigned int ci= 0 ; ci< thread_count ; ci++ ){
			ThreadArray[ci]->Run();
		}
	}

	volatile unsigned int	IsDone() override
	{
		unsigned int	thread_count= InstanceArray.GetSize();
		for( unsigned int ci= 0 ; ci< thread_count ; ci++ ){
			if( !InstanceArray[ci].IsDone() ){
				return	false;
			}
		}
		Join();
		return	true;
	}
	volatile unsigned int	GetProgress() override
	{
		unsigned int	thread_count= InstanceArray.GetSize();
		if( thread_count == 0 ){
			return	0;
		}
		return	InstanceArray[0].GetProgress();
	}
	void SetLoop( unsigned int loop ) override
	{
		unsigned int	thread_count= InstanceArray.GetSize();
		for( unsigned int ci= 0 ; ci< thread_count ; ci++ ){
			InstanceArray[ci].SetLoop( loop );
		}
	}
	unsigned int	GetResult( unsigned int index ) const override
	{
		unsigned int	thread_count= InstanceArray.GetSize();
		uint64_t	total_time= 0;
		for( unsigned int ci= 0 ; ci< thread_count ; ci++ ){
			unsigned int	time= InstanceArray[ci].GetResult( index );
			total_time+= time;
		}
		return	static_cast<unsigned int>( total_time / thread_count );
	}
	unsigned int	GetResultInfo( InfoType index ) const override
	{
		unsigned int	thread_count= InstanceArray.GetSize();
		if( thread_count == 0 ){
			return	0;
		}
		return	InstanceArray[0].GetResultInfo( index );
	}
	unsigned int	GetLoopOp( unsigned int index ) const override
	{
		unsigned int	thread_count= InstanceArray.GetSize();
		unsigned int	total_op= 0;
		for( unsigned int ci= 0 ; ci< thread_count ; ci++ ){
			total_op+= InstanceArray[ci].GetLoopOp( index );
		}
		return	total_op;
	}
	unsigned int	GetInstFop( unsigned int index ) const override
	{
		unsigned int	thread_count= InstanceArray.GetSize();
		unsigned int	total_op= 0;
		for( unsigned int ci= 0 ; ci< thread_count ; ci++ ){
			total_op+= InstanceArray[ci].GetInstFop( index );
		}
		return	total_op;
	}
	const char*		GetTestName() const override
	{
		return	InstanceArray[0].GetTestName();
	}
	const char*		GetInstructionName( unsigned int result_index ) const override
	{
		return	InstanceArray[0].GetInstructionName( result_index );
	}
	bool	IsMultithread() const override
	{
		return	true;
	}
	unsigned int	GetLoopType() const override
	{
		return	InstanceArray[0].GetLoopType();
	}
};



#endif



