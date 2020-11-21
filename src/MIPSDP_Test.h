// 2015 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef	MIPSDP_TEST_H_
#define	MIPSDP_TEST_H_

#include	<minilib/CoreLib.h>
#include	"TestBase.h"

namespace MIPSDP {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

class FloatTest : public TestBase {
public:
	enum {
		DEFAULT_LOOP	=	BASE_DEFAULT_LOOP_FPU,
		PER_LOOP_INST	=	5 * 8,	// 40
	};
	enum {
		RESULT_FPU_MULD_IR8,
		RESULT_FPU_ADDD_IR8,
		RESULT_FPU_MADDD_IR8,

		RESULT_FPU_MULD_IR1,
		RESULT_FPU_ADDD_IR1,
		RESULT_FPU_MADDD_IR1,


		RESULT_MAX,
	};
public:

	FloatTest();

	void	Run() override;
	unsigned int	GetResultInfo( InfoType index ) const override
	{
		switch( index ){
		case InfoType::INFO_COUNT:
			return	RESULT_MAX;
		case InfoType::INFO_PROGRESS:
			return	RESULT_MAX;
		case InfoType::INFO_LOOP:
			return	LoopCount;
		}
		FL_ASSERT( 0 );
		return	0;
	}
	const char*		GetTestName() const override;
	const char*		GetInstructionName( unsigned int result_index ) const override;
	unsigned int	GetLoopType() const override
	{
		return	LOOPTYPE_DP;
	}
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

#endif



