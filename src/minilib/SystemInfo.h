// 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef	FLATLIB_SYSTEMINFO_H_
#define	FLATLIB_SYSTEMINFO_H_

#include	<minilib/Platform.h>

namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

enum class CPUArch : unsigned int {
	CPU_UNKNOWN,
	CPU_ARM5,		// Android
	CPU_ARM6,		// Raspberry Pi
	CPU_ARM7,		// iOS, Linux, Android, Termux, Raspberry Pi 2
	CPU_ARM64,		// iOS, Linux, Android, Termux, Raspberry Pi 3, Jetson
	CPU_X86,		// Linux, Android, Termux, Windows
	CPU_X64,		// Linux, Android, Termux, Windows
	CPU_MIPS32,		// Android
	CPU_MIPS64,		// Android
//	CPU_PPC64,		// PS3 Linux
};


enum class CPUFeature : unsigned int {
	IA_SSE,
	IA_SSE2,
	IA_SSE3,
	IA_SSSE3,
	IA_SSE41,
	IA_SSE42,
	IA_SSE4A,
	IA_AVX,
	IA_AVX2,
	IA_AVX512F,
	IA_AVX512BW,
	IA_AVX512DQ,
	IA_AVX512VL,
	IA_AVX512VNNI,
	IA_AVX512BF16,
	IA_AES,
	IA_FMA3,
	IA_FMA4,
	IA_SHA,
	IA_F16C,
	IA_X64,
	//--
	ARM_NEON,
	ARM_VFPV4,		// include fma/half
	ARM_FPHP,
	ARM_SIMDHP,
	ARM_SIMDDP,
	ARM_SVE,
	ARM_CRC32,
	ARM_SHA1,
	ARM_SHA2,
	ARM_SHA3,
	ARM_SHA512,
	ARM_AES,
	ARM_64,
	//--
	MIPS_MSA,
	MIPS_F64,
	MIPS_PS,
	//--
	FEATURE_MAX,
};

struct CPUFeatureNameTable {
	const char*	FeatureName;
	CPUFeature	Feature;
};

struct CPUGroup {
	uint64_t		ClusterMask;
	uint64_t		ThreadMask;
	unsigned int	Index;
	unsigned int	PhysicalCoreID;
	unsigned int	ClusterGroupID;	// big.LITTLE
	unsigned int	CoreClock;		// KHz	3600000 = 3.6GHz
};

class SystemInfo {
public:
	enum : unsigned int {
		DEVICE_NAME_BUFFER_SIZE	=	256,
		CPU_FEATURE_MAX			=	64,
		CPU_COUNT_MAX			=	64,
	};
private:
	CPUGroup		CoreList[CPU_COUNT_MAX];
	uint64_t		InstructionSet;
	unsigned int	TotalThreadCount;
	unsigned int	PhysicalCoreCount;
	unsigned int	CoreGroupCount;
	bool			Initialized;
	char			DeviceName[DEVICE_NAME_BUFFER_SIZE];
	static CPUFeatureNameTable	FeatureNameTable[];
private:
	void	DecodeCpuInfo();
	void	DecodeCpuTopologyImmediate();
	void	DecodeCpuTopology();
	void	GetHWInfo();
	void	GetCPUSpecification();
public:
	SystemInfo();
	void	Init();
	unsigned int	GetTotalThreadCount() const
	{
		return	TotalThreadCount;
	}
	unsigned int	GetPhysicalCoreCount() const
	{
		return	PhysicalCoreCount;
	}
	unsigned int	GetCoreGroupCount() const
	{
		return	CoreGroupCount;
	}
	unsigned int	GetThreadCount( unsigned int group_index ) const;
	unsigned int	GetCoreClock( unsigned int group_index ) const;
	uint64_t		GetAffinityMask( unsigned int group_index ) const;
	bool IsSingleThread() const
	{
		return	GetTotalThreadCount() == 1;
	}
	bool	HasInstructionSet( CPUFeature feature ) const;
	void	SetInstructionSet( CPUFeature feature );

	const char*		GetDeviceName() const;
	CPUArch			GetArch() const;
	const char* 	GetArchNameLong() const;
	const char* 	GetArchNameShort() const;

	const char*		GetFeatureName( CPUFeature feature ) const;
	void			DumpCpuGroup() const;
	void			GetCpuFeatureString( char* buffer, size_t buffer_size ) const;
	void			DumpSystemInfo() const;
};


extern	SystemInfo	Info;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

#endif


