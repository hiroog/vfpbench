// 2025 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef	SSE64BF16_TEST_H_
#define	SSE64BF16_TEST_H_

#include	<flatlib/core/CoreBase.h>
#include	"TestBase.h"

namespace SSE64BF16 {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#define	VB_MINIMUM_BF16		1

class FloatTest : public TestBase {
public:
	enum {
		DEFAULT_LOOP	=	BASE_DEFAULT_LOOP_FPU,
		PER_LOOP_INST	=	5 * 8,
		PER_LOOP_INST_12=	5 * 12,
	};
	enum : unsigned int {
		// AVX512-BF16: vdpbf16ps ymm (256-bit)  n8/n12/n1
#if !VB_MINIMUM_BF16
		RESULT_AVX512_VDPBF16PS_YMM_IR8,
#endif
		RESULT_AVX512_VDPBF16PS_YMM_IR12,
#if !VB_MINIMUM_BF16
		RESULT_AVX512_VDPBF16PS_YMM_IR1,
#endif

		// AVX512-BF16: vdpbf16ps zmm (512-bit)  n8/n12/n1
#if !VB_MINIMUM_BF16
		RESULT_AVX512_VDPBF16PS_ZMM_IR8,
#endif
		RESULT_AVX512_VDPBF16PS_ZMM_IR12,
#if !VB_MINIMUM_BF16
		RESULT_AVX512_VDPBF16PS_ZMM_IR1,
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
