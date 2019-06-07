// 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef	THREAD_ADAPTER_H_
#define	THREAD_ADAPTER_H_

#include	<minilib/CoreLib.h>
#include	<minilib/ThreadFunction.h>
#include	"TestBase.h"


template<typename T>
class ThreadAdapter : public ITestBase {
public:
private:
	T	Instance;
	flatlib::thread::ThreadFunctionBase*	BenchThread;
public:

	ThreadAdapter( unsigned int group ) : ITestBase( false, group ), BenchThread( nullptr )
	{
		Instance.SetIsMultithread( false );
		Instance.SetCoreGroup( group );
	}
	~ThreadAdapter()
	{
		Quit();
	}

	void	Run_Direct()
	{
		Instance.Run();
	}

	void	Quit()
	{
		FL_LOG( "ThreadAdapter QUIT\n" );
		Join();
		FL_LOG( "ThreadAdapter QUIT COMPLETE\n" );
	}

	void	Join()
	{
		if( BenchThread ){
			FL_LOG( "ThreadAdapter Join\n" );
			BenchThread->Join();
			FL_LOG( "ThreadAdapter Join COMPLETE\n" );
			flatlib::memory::SafeDelete( BenchThread );
		}
	}

	void	Run() override
	{
		BenchThread= flatlib::thread::CreateThreadFunction(
			[=](){
				FL_LOG( "Run : In Thread\n" );
				this->SetCpuAffinity();
				this->Instance.SetIsMultithread( false );
				this->Instance.Run();
				FL_LOG( "End Thread\n" );
			});
		BenchThread->Run();
	}

	volatile unsigned int	IsDone() override
	{
		if( Instance.IsDone() ){
			Join();
			return	true;
		}
		return	false;
	}
	volatile unsigned int	GetProgress() override
	{
		return	Instance.GetProgress();
	}
	void SetLoop( unsigned int loop ) override
	{
		Instance.SetLoop( loop );
	}
	unsigned int	GetResult( unsigned int index ) const override
	{
		return	Instance.GetResult( index );
	}
	unsigned int	GetResultInfo( InfoType index ) const override
	{
		return	Instance.GetResultInfo( index );
	}
	unsigned int	GetLoopOp( unsigned int index ) const override
	{
		return	Instance.GetLoopOp( index );
	}
	unsigned int	GetInstFop( unsigned int index ) const override
	{
		return	Instance.GetInstFop( index );
	}
	const char*		GetTestName() const override
	{
		return	Instance.GetTestName();
	}
	const char*		GetInstructionName( unsigned int result_index ) const override
	{
		return	Instance.GetInstructionName( result_index );
	}
	bool	IsMultithread() const override
	{
		return	false;
	}
	unsigned int	GetLoopType() const override
	{
		return	Instance.GetLoopType();
	}
};



#endif


