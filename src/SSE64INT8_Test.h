// 2025 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef	SSE64INT8_TEST_H_
#define	SSE64INT8_TEST_H_

#include	<flatlib/core/CoreBase.h>
#include	"TestBase.h"

namespace SSE64INT8 {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#define	VB_MINIMUM_INT8		1

class FloatTest : public TestBase {
public:
	enum {
		DEFAULT_LOOP	=	BASE_DEFAULT_LOOP_FPU,
		PER_LOOP_INST	=	5 * 8,
		PER_LOOP_INST_12=	5 * 12,
	};
	enum : unsigned int {
		// SSSE3: pmaddubsw xmm (128-bit, legacy SSE, u8*s8)  n12
		RESULT_SSSE3_PMADDUBSW_XMM_IR12,

		// AVX: vpmaddubsw xmm (128-bit, VEX, u8*s8)  n12
		RESULT_AVX_VPMADDUBSW_XMM_IR12,

		// AVX2: vpmaddubsw ymm (256-bit, u8*s8)  n12
		RESULT_AVX2_VPMADDUBSW_YMM_IR12,

		// AVX512BW: vpmaddubsw zmm (512-bit, u8*s8)  n12
		RESULT_AVX512BW_VPMADDUBSW_ZMM_IR12,

		// AVX-VNNI: vpdpbusd ymm (256-bit, u8*s8)  n8/n12/n1
#if !VB_MINIMUM_INT8
		RESULT_AVX_VPDPBUSD_YMM_IR8,
#endif
		RESULT_AVX_VPDPBUSD_YMM_IR12,
#if !VB_MINIMUM_INT8
		RESULT_AVX_VPDPBUSD_YMM_IR1,
#endif

		// AVX512-VNNI: vpdpbusd ymm (256-bit, u8*s8)  n12
		RESULT_AVX512_VPDPBUSD_YMM_IR12,

		// AVX512-VNNI: vpdpbusd zmm (512-bit)  n8/n12/n1
#if !VB_MINIMUM_INT8
		RESULT_AVX512_VPDPBUSD_ZMM_IR8,
#endif
		RESULT_AVX512_VPDPBUSD_ZMM_IR12,
#if !VB_MINIMUM_INT8
		RESULT_AVX512_VPDPBUSD_ZMM_IR1,
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
