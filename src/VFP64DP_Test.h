// 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef	VFP64DP_TEST_H_
#define	VFP64DP_TEST_H_

#include	<flatlib/core/CoreBase.h>
#include	"TestBase.h"

namespace VFP64DP {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

class FloatTest : public TestBase {
public:
	enum {
		//DEFAULT_LOOP	=	100000000,
		DEFAULT_LOOP	=	BASE_DEFAULT_LOOP_FPU,
		PER_LOOP_INST	=	5 * 8,
		PER_LOOP_INST_12=	5 * 12,
	};
	enum {
		RESULT_VFP_FMUL_IR8,
		RESULT_VFP_FADD_IR8,
		RESULT_VFP_FMADD_IR8,

		RESULT_NEON_FMUL_D2_IR8,
		RESULT_NEON_FADD_D2_IR8,
		RESULT_NEON_FMLA_D2_IR8,



		RESULT_VFP_FMUL_IRS4,
		RESULT_VFP_FADD_IRS4,
		RESULT_VFP_FMADD_IRS4,

		RESULT_NEON_FMUL_D2_IRS4,
		RESULT_NEON_FADD_D2_IRS4,
		RESULT_NEON_FMLA_D2_IRS4,



		RESULT_VFP_FMUL_IR1,
		RESULT_VFP_FADD_IR1,
		RESULT_VFP_FMADD_IR1,

		RESULT_NEON_FMUL_D2_IR1,
		RESULT_NEON_FADD_D2_IR1,
		RESULT_NEON_FMLA_D2_IR1,



		RESULT_NEON_FMUL_D2_IR12,
		RESULT_NEON_FADD_D2_IR12,
		RESULT_NEON_FMLA_D2_IR12,


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



