// 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef	VFP32DP_TEST_H_
#define	VFP32DP_TEST_H_

#include	<minilib/CoreLib.h>
#include	"TestBase.h"

namespace VFP32DP {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

class FloatTest : public TestBase {
public:
	enum {
		//DEFAULT_LOOP	=	100000000,
		DEFAULT_LOOP	=	BASE_DEFAULT_LOOP_FPU,
		PER_LOOP_INST	=	5 * 8,
	};
	enum {
		RESULT_VFP_FMULD_IR8,
		RESULT_VFP_FADDD_IR8,
		RESULT_VFP_FMACD_IR8,
		RESULT_VFP_VFMA_F64_IR8,

		RESULT_VFP_FMULD_IRS4,
		RESULT_VFP_FADDD_IRS4,
		RESULT_VFP_FMACD_IRS4,
		RESULT_VFP_VFMA_F64_IRS4,

		RESULT_VFP_FMULD_IR1,
		RESULT_VFP_FADDD_IR1,
		RESULT_VFP_FMACD_IR1,
		RESULT_VFP_VFMA_F64_IR1,

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
		flASSERT( 0 );
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

