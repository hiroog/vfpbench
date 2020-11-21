// 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef	TEST_BASE_H_
#define	TEST_BASE_H_

#include	<flatlib/core/thread/AtomicValue.h>

namespace flatlib {
	namespace system {
		class SystemInfo;
	}
}

enum class InfoType : unsigned int {
	INFO_COUNT,
	INFO_PROGRESS,
	INFO_LOOP,
};

enum LoopType : unsigned int {
	LOOPTYPE_HP,
	LOOPTYPE_SP,
	LOOPTYPE_DP,
	LOOPTYPE_MAT,
};

struct ResultTable {
	uint64_t		Time;
	unsigned int	Lop;	// Fop per loop
	float			Fop;	// Fop per instruction
};

class ITestBase {
	unsigned int	CoreGroup;
	bool			Multithread;
public:
	ITestBase() : CoreGroup( 0 ), Multithread( false )
	{
	}
	ITestBase( bool multithread, unsigned int group ) : CoreGroup( group ), Multithread( multithread )
	{
	}
	virtual ~ITestBase()
	{
	}
	void	SetCpuAffinity();

	virtual void					Run()= 0;
	virtual void					SetLoop( unsigned int loop )= 0;
	virtual volatile unsigned int	GetProgress()= 0;
	virtual volatile unsigned int	IsDone()= 0;
	virtual unsigned int			GetResultInfo( InfoType index ) const= 0;

	virtual unsigned int			GetResult( unsigned int result_index ) const= 0;
	virtual unsigned int			GetLoopOp( unsigned int result_index ) const= 0;
	virtual float					GetInstFop( unsigned int result_index ) const= 0;

	virtual const char*				GetTestName() const
	{
		return	nullptr;
	}
	virtual const char*				GetInstructionName( unsigned int result_index ) const
	{
		return	nullptr;
	}

	unsigned int					GetResultCount() const
	{
		return	GetResultInfo( InfoType::INFO_COUNT );
	}
	virtual bool	IsMultithread() const
	{
		return	Multithread;
	}
	virtual void	SetIsMultithread( bool multithread )
	{
		Multithread= multithread;
	}
	virtual unsigned int	GetLoopType() const= 0;
	virtual unsigned int	GetCoreGroup() const
	{
		return	CoreGroup;
	}
	virtual	void			SetCoreGroup( unsigned int group )
	{
		CoreGroup= group;
	}
};



class TestBase : public ITestBase {
protected:
	enum {
		//BASE_DEFAULT_LOOP_MATRIX	=	500000,
		BASE_DEFAULT_LOOP_MATRIX	=	1000000,
		//BASE_DEFAULT_LOOP_FPU		=	10000000,
		BASE_DEFAULT_LOOP_FPU		=	30000000,
		RESULT_BUFFER_MAX			=	128,
	};
	ResultTable		TestResult[RESULT_BUFFER_MAX];

	flatlib::thread::AtomicValue<unsigned int>	Progress;
	flatlib::thread::AtomicValue<unsigned int>	DoneFlag;
	flatlib::system::SystemInfo&	Info;
	unsigned int			LoopCount;
protected:
	void	SetOp( unsigned int index, unsigned int lop, unsigned int fop );
	void	SetOp2( unsigned int index, unsigned int ipl, unsigned int fop );
	void	SetOp2f( unsigned int index, unsigned int ipl, float fop );
	void	SetResult( unsigned int index, uint64_t time );
	void	InitClear();
	void	ClearResult();
public:

	TestBase();
	virtual ~TestBase() override
	{
	}

	virtual void					SetLoop( unsigned int loop ) override;
	virtual volatile unsigned int	GetProgress() override;
	virtual volatile unsigned int	IsDone() override;

	virtual unsigned int			GetResult( unsigned int result_index ) const override;
	virtual unsigned int			GetLoopOp( unsigned int result_index ) const override;
	virtual float					GetInstFop( unsigned int result_index ) const override;


};



#endif


