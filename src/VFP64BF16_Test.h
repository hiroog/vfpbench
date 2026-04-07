// 2024 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef	VFP64BF16_TEST_H_
#define	VFP64BF16_TEST_H_

#include	<flatlib/core/CoreBase.h>
#include	"TestBase.h"

namespace VFP64BF16 {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#define	VB_MINIMUM_ARM_BF16		1

class FloatTest : public TestBase {
public:
	enum {
		DEFAULT_LOOP	=	BASE_DEFAULT_LOOP_FPU,
		PER_LOOP_INST	=	5 * 8,
		PER_LOOP_INST_12=	5 * 12,
	};
	enum : unsigned int {
		RESULT_NEON_BFMMLA_IR12,

#if !VB_MINIMUM_ARM_BF16
		RESULT_NEON_BFDOT_S2_IR8,
#endif
		RESULT_NEON_BFDOT_S2_IR12,
#if !VB_MINIMUM_ARM_BF16
		RESULT_NEON_BFDOT_S2_IR1,
#endif

#if !VB_MINIMUM_ARM_BF16
		RESULT_NEON_BFDOT_S4_IR8,
#endif
		RESULT_NEON_BFDOT_S4_IR12,
#if !VB_MINIMUM_ARM_BF16
		RESULT_NEON_BFDOT_S4_IR1,
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
		return	LOOPTYPE_BF16;
	}
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

#endif


