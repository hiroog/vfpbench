// 2024 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef	VFP64INT8_TEST_H_
#define	VFP64INT8_TEST_H_

#include	<flatlib/core/CoreBase.h>
#include	"TestBase.h"

namespace VFP64INT8 {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#define	VB_MINIMUM_ARM_INT8		1

class FloatTest : public TestBase {
public:
	enum {
		DEFAULT_LOOP	=	BASE_DEFAULT_LOOP_FPU,
		PER_LOOP_INST	=	5 * 8,
		PER_LOOP_INST_12=	5 * 12,
	};
	enum : unsigned int {
		// FEAT_I8MM: smmla/ummla/usmmla  n12 only (matrix multiply)
		RESULT_NEON_SMMLA_IR12,
		RESULT_NEON_UMMLA_IR12,
		RESULT_NEON_USMMLA_IR12,

		// FEAT_DOTPROD: sdot .2s (64-bit)   n8/n12/n1
#if !VB_MINIMUM_ARM_INT8
		RESULT_NEON_SDOT_S2_IR8,
#endif
		RESULT_NEON_SDOT_S2_IR12,
#if !VB_MINIMUM_ARM_INT8
		RESULT_NEON_SDOT_S2_IR1,
#endif

		// FEAT_DOTPROD: udot .2s (64-bit)   n8/n12/n1
#if !VB_MINIMUM_ARM_INT8
		RESULT_NEON_UDOT_S2_IR8,
#endif
		RESULT_NEON_UDOT_S2_IR12,
#if !VB_MINIMUM_ARM_INT8
		RESULT_NEON_UDOT_S2_IR1,
#endif

		// FEAT_DOTPROD: sdot .4s (128-bit)  n8/n12/n1
#if !VB_MINIMUM_ARM_INT8
		RESULT_NEON_SDOT_S4_IR8,
#endif
		RESULT_NEON_SDOT_S4_IR12,
#if !VB_MINIMUM_ARM_INT8
		RESULT_NEON_SDOT_S4_IR1,
#endif

		// FEAT_DOTPROD: udot .4s (128-bit)  n8/n12/n1
#if !VB_MINIMUM_ARM_INT8
		RESULT_NEON_UDOT_S4_IR8,
#endif
		RESULT_NEON_UDOT_S4_IR12,
#if !VB_MINIMUM_ARM_INT8
		RESULT_NEON_UDOT_S4_IR1,
#endif

		RESULT_MAX,
	};
private:
	unsigned int	GetProgressStep() const;
public:

	FloatTest();

	void	Run() override;
	unsigned int	GetResultInfo( InfoType index ) const override
	{
		switch( index ){
		case InfoType::INFO_COUNT:
			return	RESULT_MAX;
		case InfoType::INFO_PROGRESS:
			return	GetProgressStep();
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
		return	LOOPTYPE_INT8;
	}
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

#endif


