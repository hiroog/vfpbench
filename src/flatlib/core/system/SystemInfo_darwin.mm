// flatlib 2020 Hiroyuki Ogasawara
// vim:ts=4 sw=4:

#include	"flatlib/core/CoreBase.h"
#include	"flatlib/core/system/SystemInfo.h"
#include	"flatlib/core/system/ConsoleLog.h"
#include	"flatlib/core/memory/MemoryAddress.h"

//-----------------------------------------------------------------------------
#if FL_OS_DARWIN
//-----------------------------------------------------------------------------
#include	<sys/types.h>
#include	<sys/sysctl.h>
#include	<mach/mach.h>
#include	<mach/mach_host.h>
//#include	<Foundation/Foundation.h>


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace system {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#define	DEF_CPUFREQ(  name, soc, devname, freq )	{	devname,	1,	{	{0,0,0,0,freq,0},	}	}
#define	DEF_CPUINFO2( name, soc, devname, i0,count0,freq0, i1,count1,freq1 )	{	devname,	2,	{	{i0,count0,0,i0,freq0,1},	{i1,count1,1,i1,freq1,1},	}	}

struct CPUGroupInfo {
	unsigned int	Index;
	unsigned int	Count;			// 0== freq only
	unsigned int	GroupID;
	unsigned int	PhysicalID;
	unsigned int	CPU_Freq;
	unsigned int	HT_Step;		// 0== freq only
};

struct CPUCoreInfo {
	static constexpr unsigned int	MAX_GROUP=	4;
	const char*		DeviceName;
	unsigned int	GroupCount;
	CPUGroupInfo	Group[MAX_GROUP];
public:
	bool	IsFreqOnly() const
	{
		return	Group[0].Count == 0;
	}
	unsigned int	GetFreq() const
	{
		return	Group[0].CPU_Freq;
	}
};

static const CPUCoreInfo	CPUCoreInfoTable[]= {

	//----------------------------------------------------------------------------

	DEF_CPUFREQ(	"Apple TV HD Gen4",		"Apple A8",		"AppleTV5,3",		 1500000	),
	DEF_CPUINFO2(	"Apple TV 4K Gen5",		"Apple A10X",	"AppleTV6,2",		0,	3,	2380000,	3,	3,	0	),


	//----------------------------------------------------------------------------

	// Firestorm Icestorm
	DEF_CPUINFO2(	"MacBookAirLate2020",	"Apple M1",		"MacBookAir10,1",	0,	4,	3200000,	4,	4,	0	),
	DEF_CPUINFO2(	"MacBookProLate2020",	"Apple M1",		"MacBookPro17,1",	0,	4,	3200000,	4,	4,	0	),
	DEF_CPUINFO2(	"MacMini2020",			"Apple M1",		"Macmini9,1",		0,	4,	3200000,	4,	4,	0	),

	//----------------------------------------------------------------------------

	DEF_CPUFREQ(	"Watch",				"Apple S1",		"Watch1,1",			 520000		),
	DEF_CPUFREQ(	"Watch",				"Apple S1",		"Watch1,2",			 520000		),

	DEF_CPUFREQ(	"WatchSeries2",			"Apple S2",		"Watch2,3",			 520000		),
	DEF_CPUFREQ(	"WatchSeries2",			"Apple S2",		"Watch2,4",			 520000		),

	DEF_CPUFREQ(	"WatchSeries1",			"Apple S1P",	"Watch2,6",			 520000		),
	DEF_CPUFREQ(	"WatchSeries1",			"Apple S1P",	"Watch2,7",			 520000		),

#if 0
	DEF_CPUFREQ(	"WatchSeries3",			"Apple S3",		"Watch3,1",			 500000		),
	DEF_CPUFREQ(	"WatchSeries3",			"Apple S3",		"Watch3,2",			 500000		),
	DEF_CPUFREQ(	"WatchSeries3",			"Apple S3",		"Watch3,3",			 500000		),
	DEF_CPUFREQ(	"WatchSeries3",			"Apple S3",		"Watch3,4",			 500000		),

	DEF_CPUFREQ(	"WatchSeries4",			"Apple S4",		"Watch4,1",			 500000		),
	DEF_CPUFREQ(	"WatchSeries4",			"Apple S4",		"Watch4,2",			 500000		),
	DEF_CPUFREQ(	"WatchSeries4",			"Apple S4",		"Watch4,3",			 500000		),
	DEF_CPUFREQ(	"WatchSeries4",			"Apple S4",		"Watch4,4",			 500000		),

	DEF_CPUFREQ(	"WatchSeries5",			"Apple S5",		"Watch5,1",			 500000		),
	DEF_CPUFREQ(	"WatchSeries5",			"Apple S5",		"Watch5,2",			 500000		),
	DEF_CPUFREQ(	"WatchSeries5",			"Apple S5",		"Watch5,3",			 500000		),
	DEF_CPUFREQ(	"WatchSeries5",			"Apple S5",		"Watch5,4",			 500000		),

	DEF_CPUFREQ(	"WatchSE",				"Apple S5",		"Watch5,9",			 500000		),
	DEF_CPUFREQ(	"WatchSE",				"Apple S5",		"Watch5,10",		 500000		),
	DEF_CPUFREQ(	"WatchSE",				"Apple S5",		"Watch5,11",		 500000		),
	DEF_CPUFREQ(	"WatchSE",				"Apple S5",		"Watch5,12",		 500000		),
#endif

	DEF_CPUFREQ(	"WatchSeries6",			"Apple S6",		"Watch6,1",			1500000		),
	DEF_CPUFREQ(	"WatchSeries6",			"Apple S6",		"Watch6,2",			1500000		),
	DEF_CPUFREQ(	"WatchSeries6",			"Apple S6",		"Watch6,3",			1500000		),
	DEF_CPUFREQ(	"WatchSeries6",			"Apple S6",		"Watch6,4",			1500000		),



	//----------------------------------------------------------------------------

	// Cyclone
	DEF_CPUFREQ(	"iPadAir",				"Apple A7",		"iPad4,1",			1400000		),
	DEF_CPUFREQ(	"iPadAir",				"Apple A7",		"iPad4,2",			1400000		),
	DEF_CPUFREQ(	"iPadAir",				"Apple A7",		"iPad4,3",			1400000		),

	DEF_CPUFREQ(	"iPadMini2",			"Apple A7",		"iPad4,4",			1300000		),
	DEF_CPUFREQ(	"iPadMini2",			"Apple A7",		"iPad4,5",			1300000		),
	DEF_CPUFREQ(	"iPadMini2",			"Apple A7",		"iPad4,6",			1300000		),
	DEF_CPUFREQ(	"iPadMini3",			"Apple A7",		"iPad4,7",			1300000		),
	DEF_CPUFREQ(	"iPadMini3",			"Apple A7",		"iPad4,8",			1300000		),
	DEF_CPUFREQ(	"iPadMini3",			"Apple A7",		"iPad4,9",			1300000		),

	// Typhoon
	DEF_CPUFREQ(	"iPadMini4",			"Apple A8",		"iPad5,1",			1500000		),
	DEF_CPUFREQ(	"iPadMini4",			"Apple A8",		"iPad5,2",			1500000		),
	DEF_CPUFREQ(	"iPadAir2",				"Apple A8X",	"iPad5,3",			1500000		),
	DEF_CPUFREQ(	"iPadAir2",				"Apple A8X",	"iPad5,4",			1500000		),

	// Twister
	DEF_CPUFREQ(	"iPad5 2017",			"Apple A9",		"iPad6,11",			1850000		),
	DEF_CPUFREQ(	"iPad5 2017",			"Apple A9",		"iPad6,12",			1850000		),
	DEF_CPUFREQ(	"iPadPro9.7-G1 2015",	"Apple A9X",	"iPad6,3",			2160000		),
	DEF_CPUFREQ(	"iPadPro9.7-G1 2015",	"Apple A9X",	"iPad6,4",			2160000		),
	DEF_CPUFREQ(	"iPadPro12.9-G1 2015",	"Apple A9X",	"iPad6,7",			2250000		),
	DEF_CPUFREQ(	"iPadPro12.9-G1 2015",	"Apple A9X",	"iPad6,8",			2250000		),

	// Fusion: Hurricane Zephyr
	DEF_CPUINFO2(	"iPadPro12.9-G2 2017",	"Apple A10X",	"iPad7,1",			0,	3,	2340000,	3,	3,	0	),
	DEF_CPUINFO2(	"iPadPro12.9-G2 2017",	"Apple A10X",	"iPad7,2",			0,	3,	2340000,	3,	3,	0	),
	DEF_CPUINFO2(	"iPadPro10.5-G2",		"Apple A10X",	"iPad7,3",			0,	3,	2340000,	3,	3,	0	),
	DEF_CPUINFO2(	"iPadPro10.5-G2",		"Apple A10X",	"iPad7,4",			0,	3,	2340000,	3,	3,	0	),
	DEF_CPUINFO2(	"iPad6 2018",			"Apple A10",	"iPad7,5",			0,	2,	2340000,	2,	2,	0	),
	DEF_CPUINFO2(	"iPad6 2018",			"Apple A10",	"iPad7,6",			0,	2,	2340000,	2,	2,	0	),

	DEF_CPUINFO2(	"iPad7 10.2 2019",		"Apple A10",	"iPad7,11",			0,	2,	2340000,	2,	2,	0	),
	DEF_CPUINFO2(	"iPad7 10.2 2019",		"Apple A10",	"iPad7,12",			0,	2,	2340000,	2,	2,	0	),

	// Bionic: Voltex Tempest
	DEF_CPUINFO2(	"iPadPro 11-G3 2018",	"Apple A12X",	"iPad8,1",			0,	4,	2490000,	4,	4,	0	),
	DEF_CPUINFO2(	"iPadPro 11-G3 2018",	"Apple A12X",	"iPad8,2",			0,	4,	2490000,	4,	4,	0	),
	DEF_CPUINFO2(	"iPadPro 11-G3 2018",	"Apple A12X",	"iPad8,3",			0,	4,	2490000,	4,	4,	0	),
	DEF_CPUINFO2(	"iPadPro 11-G3 2018",	"Apple A12X",	"iPad8,4",			0,	4,	2490000,	4,	4,	0	),

	DEF_CPUINFO2(	"iPadPro 12.9-G3 2018",	"Apple A12X",	"iPad8,5",			0,	4,	2490000,	4,	4,	0	),
	DEF_CPUINFO2(	"iPadPro 12.9-G3 2018",	"Apple A12X",	"iPad8,6",			0,	4,	2490000,	4,	4,	0	),
	DEF_CPUINFO2(	"iPadPro 12.9-G3 2018",	"Apple A12X",	"iPad8,7",			0,	4,	2490000,	4,	4,	0	),
	DEF_CPUINFO2(	"iPadPro 12.9-G3 2018",	"Apple A12X",	"iPad8,8",			0,	4,	2490000,	4,	4,	0	),

	DEF_CPUINFO2(	"iPadPro 11-G4 2020",	"Apple A12Z",	"iPad8,9",			0,	4,	2490000,	4,	4,	0	),
	DEF_CPUINFO2(	"iPadPro 11-G4 2020",	"Apple A12Z",	"iPad8,10",			0,	4,	2490000,	4,	4,	0	),
	DEF_CPUINFO2(	"iPadPro 12.9-G4 2020",	"Apple A12Z",	"iPad8,11",			0,	4,	2490000,	4,	4,	0	),
	DEF_CPUINFO2(	"iPadPro 12.9-G4 2020",	"Apple A12Z",	"iPad8,12",			0,	4,	2490000,	4,	4,	0	),

	DEF_CPUINFO2(	"iPadMini5",			"Apple A12",	"iPad11,1",			0,	2,	2490000,	2,	4,	0	),
	DEF_CPUINFO2(	"iPadMini5",			"Apple A12",	"iPad11,2",			0,	2,	2490000,	2,	4,	0	),

	DEF_CPUINFO2(	"iPadAir3 10.5",		"Apple A12",	"iPad11,3",			0,	2,	2490000,	2,	4,	0	),
	DEF_CPUINFO2(	"iPadAir3 10.5",		"Apple A12",	"iPad11,4",			0,	2,	2490000,	2,	4,	0	),

	DEF_CPUINFO2(	"iPad8",				"Apple A12",	"iPad11,6",			0,	2,	2490000,	2,	4,	0	),
	DEF_CPUINFO2(	"iPad8",				"Apple A12",	"iPad11,7",			0,	2,	2490000,	2,	4,	0	),


	// Bionic: Firestorm Icestorm
	DEF_CPUINFO2(	"iPadAir4",				"Apple A14",	"iPad13,1",			0,	2,	2990000,	2,	4,	0	),
	DEF_CPUINFO2(	"iPadAir4",				"Apple A14",	"iPad13,2",			0,	2,	2990000,	2,	4,	0	),



	//----------------------------------------------------------------------------

	// Swift
	DEF_CPUFREQ(	"iPhone5",				"Apple A6",		"iPhone5,1",		1300000		),
	DEF_CPUFREQ(	"iPhone5",				"Apple A6",		"iPhone5,2",		1300000		),
	DEF_CPUFREQ(	"iPhone5C",				"Apple A6",		"iPhone5,3",		1300000		),
	DEF_CPUFREQ(	"iPhone5C",				"Apple A6",		"iPhone5,4",		1300000		),

	// Cyclone
	DEF_CPUFREQ(	"iPhone5S",				"Apple A7",		"iPhone6,1",		1300000		),
	DEF_CPUFREQ(	"iPhone5S",				"Apple A7",		"iPhone6,2",		1300000		),

	// Typhoon
	DEF_CPUFREQ(	"iPhone6Plus",			"Apple A8",		"iPhone7,1",		1400000		),
	DEF_CPUFREQ(	"iPhone6",				"Apple A8",		"iPhone7,2",		1400000		),

	// Twister
	DEF_CPUFREQ(	"iPhone6S",				"Apple A9",		"iPhone8,1",		1850000		),
	DEF_CPUFREQ(	"iPhone6SPlus",			"Apple A9",		"iPhone8,2",		1850000		),
	DEF_CPUFREQ(	"iPhoneSE",				"Apple A9",		"iPhone8,4",		1850000		),

	// Fusion: Hurricane Zephyr
	DEF_CPUINFO2(	"iPhone7",				"Apple A10",	"iPhone9,1",		0,	2,	2340000,	2,	2,	0	),
	DEF_CPUINFO2(	"iPhone7Plus",			"Apple A10",	"iPhone9,2",		0,	2,	2340000,	2,	2,	0	),
	DEF_CPUINFO2(	"iPhone7",				"Apple A10",	"iPhone9,3",		0,	2,	2340000,	2,	2,	0	),
	DEF_CPUINFO2(	"iPhone7Plus",			"Apple A10",	"iPhone9,4",		0,	2,	2340000,	2,	2,	0	),

	// Bionic: Monsoon Mistral
	DEF_CPUINFO2(	"iPhone8",				"Apple A11",	"iPhone10,1",		0,	2,	2390000,	2,	4,	1420000	),
	DEF_CPUINFO2(	"iPhone8Plus",			"Apple A11",	"iPhone10,2",		0,	2,	2390000,	2,	4,	1420000	),
	DEF_CPUINFO2(	"iPhoneX",				"Apple A11",	"iPhone10,3",		0,	2,	2390000,	2,	4,	1420000	),
	DEF_CPUINFO2(	"iPhone8",				"Apple A11",	"iPhone10,4",		0,	2,	2390000,	2,	4,	1420000	),
	DEF_CPUINFO2(	"iPhone8Plus",			"Apple A11",	"iPhone10,5",		0,	2,	2390000,	2,	4,	1420000	),
	DEF_CPUINFO2(	"iPhoneX",				"Apple A11",	"iPhone10,6",		0,	2,	2390000,	2,	4,	1420000	),

	// Bionic: Voltex Tempest
	DEF_CPUINFO2(	"iPhoneXS",				"Apple A12",	"iPhone11,2",		0,	2,	2490000,	2,	4,	0	),
	DEF_CPUINFO2(	"iPhoneXSMax",			"Apple A12",	"iPhone11,4",		0,	2,	2490000,	2,	4,	0	),
	DEF_CPUINFO2(	"iPhoneXSMax",			"Apple A12",	"iPhone11,6",		0,	2,	2490000,	2,	4,	0	),
	DEF_CPUINFO2(	"iPhoneXR",				"Apple A12",	"iPhone11,8",		0,	2,	2490000,	2,	4,	0	),

	// Bionic: Lightning Thunder
	DEF_CPUINFO2(	"iPhone11",				"Apple A13",	"iPhone12,1",		0,	2,	2650000,	2,	4,	0	),
	DEF_CPUINFO2(	"iPhone11Pro",			"Apple A13",	"iPhone12,3",		0,	2,	2650000,	2,	4,	0	),
	DEF_CPUINFO2(	"iPhone11ProMax",		"Apple A13",	"iPhone12,5",		0,	2,	2650000,	2,	4,	0	),
	DEF_CPUINFO2(	"iPhoneSE",				"Apple A13",	"iPhone12,8",		0,	2,	2650000,	2,	4,	0	),

	// Bionic: Firestorm Icestorm
	DEF_CPUINFO2(	"iPhone12Mini",			"Apple A14",	"iPhone13,1",		0,	2,	2990000,	2,	4,	0	),
	DEF_CPUINFO2(	"iPhone12",				"Apple A14",	"iPhone13,2",		0,	2,	2990000,	2,	4,	0	),
	DEF_CPUINFO2(	"iPhone12Pro",			"Apple A14",	"iPhone13,3",		0,	2,	2990000,	2,	4,	0	),
	DEF_CPUINFO2(	"iPhone12ProMax",		"Apple A14",	"iPhone13,4",		0,	2,	2990000,	2,	4,	0	),


	//----------------------------------------------------------------------------

	//DEF_CPUFREQ(	"iPodTouch1",			"",				"iPod1,1",			 412000		),
	//DEF_CPUFREQ(	"iPodTouch2",			"",				"iPod2,1",			 533000		),
	//DEF_CPUFREQ(	"iPodTouch3",			"S5PC100",		"iPod3,1",			 600000		),
	DEF_CPUFREQ(	"iPodTouch4",			"Apple A4",		"iPod4,1",			 800000		),
	DEF_CPUFREQ(	"iPodTouch5",			"Apple A5",		"iPod5,1",			 800000		),
	DEF_CPUFREQ(	"iPodTouch6",			"Apple A8",		"iPod7,1",			1100000		),
	DEF_CPUINFO2(	"iPadTouch7",			"Apple A10",	"iPad9,1",		0,	2,	2340000,	2,	2,	0	),


};

static const CPUCoreInfo*	FindCPUInfo( const char* dev_name )
{
	unsigned int	ccount= sizeof(CPUCoreInfoTable) / sizeof(CPUCoreInfo);
	for( unsigned int ci= 0 ; ci< ccount ; ci++ ){
		const auto&		cpu= CPUCoreInfoTable[ci];
		if( strcmp( dev_name, cpu.DeviceName ) == 0 ){
			return	&cpu;
		}
	}
	return	nullptr;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

static int getSysInt( int hw_id )
{
	int mib[2];
	mib[0]= CTL_HW;
	mib[1]= hw_id;
	int		data= 0;
	size_t	len= sizeof(data);
	sysctl( mib, 2, &data, &len, nullptr, 0 );
	return	data;
}

static void	dumpSysInt( int hw_id, const char* msg )
{
	FL_LOG( "HW INFO: %s = %u\n", msg, getSysInt( hw_id ) );
}

static uint64_t	getSysLong( int hw_id )
{
	int mib[2];
	mib[0]= CTL_HW;
	mib[1]= hw_id;
	uint64_t	data= 0;
	size_t	len= sizeof(data);
	sysctl( mib, 2, &data, &len, nullptr, 0 );
	return	data;
}

static void	dumpSysLong( int hw_id, const char* msg )
{
	FL_LOG( "HW INFO: %s = %llu\n", msg, getSysLong( hw_id ) );
}

static void getSysString( int hw_id, char* buffer, size_t buffer_len )
{
	int mib[2];
	mib[0]= CTL_HW;
	mib[1]= hw_id;
	sysctl( mib, 2, buffer, &buffer_len, nullptr, 0 );
}

static void dumpSysString( int hw_id, const char* msg )
{
	const int	STRING_BUF_SIZE= 128;
	char	buf[STRING_BUF_SIZE];
	size_t	len= STRING_BUF_SIZE;
	getSysString( hw_id, buf, len );
	FL_LOG( "HW INFO: %s = %s\n", msg, buf );
}

//-----------------------------------------------------------------------------

static int	getSystemInt( const char* msg )
{
	int	result= 0;
	size_t	len= sizeof(result);
	if( sysctlbyname( msg, &result, &len, nullptr, 0 ) == 0 ){
		return	result;
	}
	return	-1;
}

static bool	getSystemString( const char* msg, char* buffer, size_t buffer_len )
{
	if( sysctlbyname( msg, buffer, &buffer_len, nullptr, 0 ) == 0 ){
		return	true;
	}
	return	false;
}

static int	dumpSystemInt( const char* msg )
{
	int	result= getSystemInt( msg );
	FL_LOG( "HW INFO: %s = %d\n", msg, result );
	return	result;
}

#if 0
static void dumpSystemString( const char* msg )
{
	const int	STRING_BUF_SIZE= 128;
	char	buf[STRING_BUF_SIZE];
	size_t	len= STRING_BUF_SIZE;
	if( getSystemString( msg, buf, len ) ){
		FL_LOG( "HW INFO: %s = %s\n", msg, buf );
	}
}
#endif

//-----------------------------------------------------------------------------
}

void SystemInfo::DecodeCpuTopology()
{
	unsigned int	core_clock= 0;
#if FL_OS_WATCHOS || FL_OS_TVOS
	unsigned int	core_count= 0;
	core_count= getSysInt( HW_NCPU );
#endif
	{
		core_clock= static_cast<uint32_t>(getSysInt( HW_CPU_FREQ )) / 1000;
#if FL_OS_MACOS
		getSysString( HW_MODEL, DeviceName, DEVICE_NAME_BUFFER_SIZE-1 );
#else
		getSysString( HW_MACHINE, DeviceName, DEVICE_NAME_BUFFER_SIZE-1 );
#endif

		dumpSysString( HW_MACHINE, "Machine" );
		dumpSysString( HW_MODEL, "Model" );
		dumpSysString( HW_MACHINE_ARCH, "Arch" );

		dumpSysInt( HW_NCPU, "NCPU" );
		dumpSysInt( HW_BYTEORDER, "ByteOrder" );
		dumpSysInt( HW_PHYSMEM, "PhysMem" );
		dumpSysInt( HW_USERMEM, "UserMem" );
		dumpSysInt( HW_PAGESIZE, "PageSize" );
		dumpSysInt( HW_FLOATINGPT, "Float" );
		dumpSysInt( HW_VECTORUNIT, "VectorUnit" );
		dumpSysInt( HW_BUS_FREQ, "BusFreq" );
		dumpSysInt( HW_CPU_FREQ, "CpuFreq" );
		dumpSysInt( HW_CACHELINE, "CacheLine" );
		dumpSysInt( HW_L1ICACHESIZE, "L1Size I" );
		dumpSysInt( HW_L1DCACHESIZE, "L1Size D" );
		dumpSysInt( HW_L2SETTINGS,	"L2Settings" );
		dumpSysInt( HW_L2CACHESIZE, "L2CacheSize" );
		dumpSysInt( HW_L3SETTINGS,	"L3Settings" );
		dumpSysInt( HW_L3CACHESIZE, "L3CacheSize" );
		dumpSysInt( HW_TB_FREQ,		"TB_FREQ" );
		dumpSysLong( HW_MEMSIZE,	"MemSize" );
		dumpSysInt( HW_AVAILCPU,	"AvailCpu" );

#if 0
		dumpSystemInt( "hw.optional.arm.AdvSIMD" );
		dumpSystemInt( "hw.optional.arm.AdvSIMD_HPFPCvt" );
		dumpSystemInt( "hw.optional.arm.FEAT_BF16" );
		dumpSystemInt( "hw.optional.arm.FEAT_DotProd" );
		dumpSystemInt( "hw.optional.arm.FEAT_FHM" );
		dumpSystemInt( "hw.optional.arm.FEAT_FP16" );
		dumpSystemInt( "hw.optional.arm.FEAT_I8MM" );
		dumpSystemInt( "hw.optional.arm.FEAT_SME" );
		dumpSystemInt( "hw.optional.arm.FEAT_SME2" );
		dumpSystemInt( "hw.optional.arm.SME_F32F32" );
		dumpSystemInt( "hw.optional.arm.SME_BI32I32" );
		dumpSystemInt( "hw.optional.arm.SME_B16F32" );
		dumpSystemInt( "hw.optional.arm.SME_F16F32" );
		dumpSystemInt( "hw.optional.arm.SME_I16I32" );
		dumpSystemInt( "hw.optional.arm.FEAT_SME_F16F16" );
		dumpSystemInt( "hw.optional.arm.FEAT_SME_B16B16" );
		dumpSystemInt( "hw.optional.neon" );
		dumpSystemInt( "hw.optional.neon_hpfp" );
		dumpSystemInt( "hw.optional.neon_fp16" );
#endif
	}

	{
#if 0
		unsigned int	cpu_count= 0;
		unsigned int	active_count= 0;
		uint64_t		physical_memory= 0;
		@autoreleasepool {
			id	proc_info= [NSProcessInfo processInfo];
			cpu_count= [proc_info processorCount];
			active_count= [proc_info activeProcessorCount];
			physical_memory= [proc_info physicalMemory];
			FL_LOG( "OS=%s\n", [[proc_info operatingSystemVersionString] UTF8String] );
		}
		FL_PRINT( "CORE=%d  ACTIVE=%d  MEMORY=%lld\n", cpu_count, active_count, physical_memory );
#endif
	}

	{
#if !(FL_OS_WATCHOS || FL_OS_TVOS)
		host_basic_info	basic_info;
		mach_msg_type_number_t	info_count= HOST_BASIC_INFO_COUNT;
		host_t	host= mach_host_self();
		host_info( host, HOST_BASIC_INFO, (host_info_t)&basic_info, &info_count );
		FL_LOG( "sizeof(natural_t)= %d\n",	sizeof(natural_t) );
		FL_LOG( "sizeof(integer_t)= %d\n",	sizeof(integer_t) );
		FL_LOG( "sizeof(cpu_type_t)= %d\n",	sizeof(cpu_type_t) );
		FL_LOG( "sizeof(cpu_subtype_t)= %d\n",	sizeof(cpu_subtype_t) );
		FL_LOG( "sizeof(cpu_threadtype_t)= %d\n",	sizeof(cpu_threadtype_t) );
		FL_LOG( "max_cpus         = %d\n",	basic_info.max_cpus );
		FL_LOG( "avail_cpus       = %d\n",	basic_info.avail_cpus );
		FL_LOG( "memory_size      = %d\n",	basic_info.memory_size );
		FL_LOG( "cpu_type         = %d\n",	basic_info.cpu_type );
		FL_LOG( "cpu_subtype      = %d\n",	basic_info.cpu_subtype );
		FL_LOG( "cpu_threadtype   = %d\n",	basic_info.cpu_threadtype ); // 0=NoHT, 1=HT
		FL_LOG( "physical_cpu     = %d\n",	basic_info.physical_cpu );
		FL_LOG( "physical_cpu_max = %d\n",	basic_info.physical_cpu_max );
		FL_LOG( "logical_cpu      = %d\n",	basic_info.logical_cpu );
		FL_LOG( "logical_cpu_max  = %d\n",	basic_info.logical_cpu_max );
		FL_LOG( "max_mem          = %lld\n", basic_info.max_mem );

		TotalThreadCount= basic_info.logical_cpu_max;
		PhysicalCoreCount= basic_info.physical_cpu_max;
#else
		TotalThreadCount= core_count;
		PhysicalCoreCount= core_count;
#endif
	}

#if 0
	{
		host_t	host= mach_host_self();
		processor_basic_info_t	info_ptr= nullptr;
		mach_msg_type_number_t	info_count= 0;
		natural_t	processor_count= 0;
		host_processor_info( host, PROCESSOR_BASIC_INFO, &processor_count, (processor_info_array_t*)&info_ptr, &info_count );


		FL_LOG( "*processor***********\n" );
		FL_LOG( "processor_count = %d %d %d %d %d\n", processor_count, info_count, (int)sizeof(processor_basic_info), sizeof(natural_t)*PROCESSOR_BASIC_INFO_COUNT, PROCESSOR_BASIC_INFO_COUNT );
		for( int pi= 0 ; pi< processor_count ; pi++ ){
		FL_LOG( "   cpu_type    = %08x\n", info_ptr->cpu_type );
		FL_LOG( "   cpu_subtype = %d\n", info_ptr->cpu_subtype );
		FL_LOG( "   running     = %d\n", info_ptr->running );
		FL_LOG( "   slot_num    = %d\n", info_ptr->slot_num );
		FL_LOG( "   is_master   = %d\n", info_ptr->is_master );
		info_ptr++;
		}

		vm_deallocate( mach_task_self(), (vm_address_t)info_ptr, (vm_size_t)( info_count * sizeof(natural_t) ) );
		FL_LOG( "*processor***********\n" );
	}
#endif

#if FL_OS_MACOS

	if( dumpSystemInt( "hw.optional.arm64" ) == 1 ){
		SetInstructionSet( CPUFeature::ARM_64 );
	}
	if( dumpSystemInt( "hw.optional.arm.AdvSIMD" ) == 1 ){
		SetInstructionSet( CPUFeature::ARM_NEON );
	}
	if( dumpSystemInt( "hw.optional.arm.FEAT_FP16" ) == 1 ){
		SetInstructionSet( CPUFeature::ARM_FPHP );
		SetInstructionSet( CPUFeature::ARM_SIMDHP );
	}
	if( dumpSystemInt( "hw.optional.arm.FEAT_BF16" ) == 1 ){
		SetInstructionSet( CPUFeature::ARM_BF16 );
	}
	if( dumpSystemInt( "hw.optional.arm.FEAT_DotProd" ) == 1 ){
		SetInstructionSet( CPUFeature::ARM_SIMDDP );
	}
	if( dumpSystemInt( "hw.optional.arm.FEAT_I8MM" ) == 1 ){
		SetInstructionSet( CPUFeature::ARM_I8MM );
	}
	if( dumpSystemInt( "hw.optional.arm.FEAT_SME" ) == 1 ){
		SetInstructionSet( CPUFeature::ARM_SME );
	}
	if( dumpSystemInt( "hw.optional.arm.FEAT_SME2" ) == 1 ){
		SetInstructionSet( CPUFeature::ARM_SME2 );
	}

#else

# if __ARM_NEON__
	SetInstructionSet( CPUFeature::ARM_NEON );
# endif
# if __AARCH64_SIMD__
	SetInstructionSet( CPUFeature::ARM_NEON );
# endif
# if __ARM64_ARCH_8__
	SetInstructionSet( CPUFeature::ARM_64 );
# endif
# if __ARM_VFPV4__
	SetInstructionSet( CPUFeature::ARM_VFPV4 );
# endif

# if defined(__ARM_FEATURE_FP16_SCALAR_ARITHMETIC)
	SetInstructionSet( CPUFeature::ARM_FPHP );
# endif
# if defined(__ARM_FEATURE_FP16_VECTOR_ARITHMETIC)
	SetInstructionSet( CPUFeature::ARM_SIMDHP );
# endif
# if defined(__ARM_FEATURE_DOTPROD)
	SetInstructionSet( CPUFeature::ARM_SIMDDP );
# endif
# if defined(__ARM_FEATURE_SVE)
	SetInstructionSet( CPUFeature::ARM_SVE );
# endif
# if defined(__ARM_FEATURE_SVE2)
	SetInstructionSet( CPUFeature::ARM_SVE2 );
# endif

#endif

#if FL_OS_MACOS
	{
		constexpr size_t	PERF_NAME_MAX= 32;
		constexpr int		PERF_MAX_GROUP= 4;
		char	perf_physical[PERF_NAME_MAX];
		char	perf_logical[PERF_NAME_MAX];
		char	perf_name[PERF_NAME_MAX];
		char	prev_ch= '0';
		strncpy( perf_physical, "hw.perflevel0.physicalcpu", PERF_NAME_MAX-1 );
		strncpy( perf_logical, "hw.perflevel0.logicalcpu", PERF_NAME_MAX-1 );
		strncpy( perf_name, "hw.perflevel0.name", PERF_NAME_MAX-1 );
		constexpr int	group_id_index= 12;
		FL_ASSERT( perf_physical[group_id_index] == '0' );
		FL_ASSERT( perf_logical[group_id_index] == '0' );
		FL_ASSERT( perf_name[group_id_index] == '0' );
		const auto*	info= FindCPUInfo( DeviceName );
		unsigned int	base_cpu_index= 0;
		unsigned int	group_index= 0;
		for( int i= 0 ; i< PERF_MAX_GROUP ; i++ ){
			if( perf_physical[group_id_index] == prev_ch ){
				constexpr size_t	CPU_NAME_MAX= 64;
				char	cpu_name[CPU_NAME_MAX];
				prev_ch= '0' + i;
				perf_physical[group_id_index]= prev_ch;
				perf_logical[group_id_index]= prev_ch;
				perf_name[group_id_index]= prev_ch;
				int	physical_count= dumpSystemInt( perf_physical );
				int	logical_count= dumpSystemInt( perf_logical );
				bool	result= getSystemString( perf_name, cpu_name, CPU_NAME_MAX );
				if( physical_count > 0 && logical_count > 0 && result ){
					FL_LOG( "%s = %dC %dT, %s\n", perf_name, physical_count, logical_count, cpu_name );
					int	step= logical_count/physical_count;
					unsigned int	freq= core_clock;
					if( info ){
						freq= info->Group[group_index].CPU_Freq;
					}
					SetCoreInfo( base_cpu_index, logical_count, group_index, base_cpu_index/step, freq, step );
					group_index++;
					base_cpu_index+= logical_count;
				}
			}
		}
		CoreGroupCount= group_index;
	}
#else
	{
		bool	need_setup= true;
		const auto*	info= FindCPUInfo( DeviceName );
		if( info ){
			if( info->IsFreqOnly() ){
				core_clock= info->GetFreq();
			}else{
				unsigned int	total_count= 0;
				auto	group_count= info->GroupCount;
				CoreGroupCount= group_count;
				for( unsigned int gi= 0 ; gi< group_count ; gi++ ){
					const auto&	group= info->Group[gi];
					SetCoreInfo( group.Index, group.Count, gi, group.PhysicalID, group.CPU_Freq, group.HT_Step );
					total_count+= group.Count;
					FL_ASSERT( group.GroupID == gi );
				}
				need_setup= false;
				//FL_ASSERT( TotalThreadCount == total_count );
				if( TotalThreadCount != total_count ){
					CoreGroupCount= 1;
				}
			}
		}
		if( need_setup ){
			unsigned int	step= 1;
			if( TotalThreadCount != PhysicalCoreCount ){
				step= TotalThreadCount/PhysicalCoreCount;
			}
			SetCoreInfo( 0, TotalThreadCount, 0, 0, core_clock, step );
		}
	}
#endif

	DumpCpuGroup();
}






//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
#endif // Darwin
//-----------------------------------------------------------------------------
