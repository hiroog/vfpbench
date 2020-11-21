// 2014 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#include	"CoreLib.h"
#include	"SystemInfo.h"
#include	"ThreadFunction.h"
#include	"FixedArray.h"
#include	"SystemAPI.h"
#include	"AtomicValue.h"
#include	"Matrix4.h"

#if FL_OS_LINUX
# include	<stdio.h>
# include	<stdlib.h>
# include	<sys/types.h>
# include	<sys/stat.h>
# include	<map>
#endif

#if FL_OS_DARWIN
# include	<sys/types.h>
# include	<sys/sysctl.h>
# include	<mach/mach.h>
# include	<mach/mach_host.h>
#endif

#if (FL_CPU_X86 || FL_CPU_X64) && FL_CC_CLANG
# include	<cpuid.h>
#endif


namespace flatlib {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

SystemInfo	Info;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//=============================================================================
// OS
//=============================================================================


//-----------------------------------------------------------------------------
#if FL_OS_LINUX
//-----------------------------------------------------------------------------

namespace {
//-----------------------------------------------------------------------------

static unsigned int GetBitCount( uint64_t bit )
{
	unsigned int	count= 0;
	for( unsigned int i= 0 ; i< 32 ; i++, bit>>= 1 ){
		if( bit & 1 ){
			count++;
		}
	}
	return	count;
}

static bool	LoadOneLine( char* line_buffer, size_t buffer_size, const char* path )
{
	*line_buffer= '\0';
	FILE*	fp= fopen( path, "r" );
	if( fp ){
		if( !fgets( line_buffer, buffer_size-1, fp ) ){
			*line_buffer= '\0';
		}
		fclose( fp );
	}
	return	*line_buffer != '\0';
}

static unsigned int		IsExists( const char* path )
{
	struct stat	sbuf;
	if( stat( path, &sbuf ) == -1 ){
		return	0;
	}
	if( S_ISDIR( sbuf.st_mode ) ){
		return	2;
	}
	if( S_ISREG( sbuf.st_mode ) ){
		return	1;
	}
	return	0;
}

static char*	SkipColon( char* ptr )
{
	for(; *ptr && *ptr != ':' ; ptr++ );
	if( *ptr == ':' ){
		ptr++;
	}
	return	ptr;
}

static char*	SkipSpace( char* ptr )
{
	for(; *ptr && *ptr <= ' ' ; ptr++ );
	return	ptr;
}

static const char*	SkipSpace( const char* ptr )
{
	for(; *ptr && *ptr <= ' ' ; ptr++ );
	return	ptr;
}

static uint64_t	DecodeCpuRegion( const char* ptr )
{
	uint64_t		bit= 0;
	unsigned int	previd= 0;
	for(; *ptr ;){
		bool	region= false;
		ptr= SkipSpace( ptr );
		if( *ptr == '-' ){
			region= true;
			ptr++;
		}
		if( *ptr < '0' || *ptr > '9' ){
			break;
		}
		unsigned int id= atoi( ptr );
		for(; *ptr >= '0' && *ptr <= '9' ; ptr++ );
		for(; *ptr == ',' ; ptr++ );
		if( region ){
		   for( unsigned int i= previd+1 ; i< id ; i++ ){
			   bit|= 1<<i;
		   }
		}
		bit|= 1<<id;
		previd= id;
	}
	return	bit;
}

static unsigned int GetCpuCount( const char* path )
{
	const int	MAX_LINE_SIZE= 128;
	char	line_buffer[MAX_LINE_SIZE];
	if( LoadOneLine( line_buffer, MAX_LINE_SIZE-2, path ) ){
		return	GetBitCount( DecodeCpuRegion( line_buffer ) );
	}
	return	0;
}

static uint64_t	GetCpuBitMask( const char* line )
{
	const int	MAX_MASK= 8;
	uint64_t	mask_buffer[MAX_MASK];
	unsigned int	mask_index= 0;
	const char*	ptr= line;
	for(; *ptr && mask_index < MAX_MASK ;){
		auto	mask= strtoull( ptr, nullptr, 16 );
		mask_buffer[mask_index++]= mask;
		for(; *ptr && *ptr != ',' ; ptr++ );
		if( *ptr == ',' ){
			ptr++;
		}
	}
	if( mask_index ){
		return	mask_buffer[mask_index-1];
	}
	return	0;
}


//-----------------------------------------------------------------------------
}

void SystemInfo::DecodeCpuTopologyImmediate()
{
	{
		const int	MAX_PATH_SIZE= 256;
		const int	MAX_LINE_SIZE= 256;
		char	path_buffer[MAX_PATH_SIZE];
		char	line_buffer[MAX_LINE_SIZE];
		for( unsigned int ci= 0 ; ci< CPU_COUNT_MAX ; ci++ ){
			sprintf_s( path_buffer, MAX_PATH_SIZE-1, "/sys/devices/system/cpu/cpu%d", ci );
			if( !IsExists( path_buffer ) ){
				break;
			}
			auto&	core= CoreList[ci];
			core.Index= ci;
			sprintf_s( path_buffer, MAX_PATH_SIZE-1, "/sys/devices/system/cpu/cpu%d/topology/thread_siblings", ci );
			if( LoadOneLine( line_buffer, MAX_LINE_SIZE, path_buffer ) ){
				core.ThreadMask= GetCpuBitMask( line_buffer );
			}

			sprintf_s( path_buffer, MAX_PATH_SIZE-1, "/sys/devices/system/cpu/cpu%d/topology/core_siblings", ci );
			if( LoadOneLine( line_buffer, MAX_LINE_SIZE, path_buffer ) ){
				core.ClusterMask= GetCpuBitMask( line_buffer );
			}

			sprintf_s( path_buffer, MAX_PATH_SIZE-1, "/sys/devices/system/cpu/cpu%d/cpufreq/cpuinfo_max_freq", ci );
			if( LoadOneLine( line_buffer, MAX_LINE_SIZE, path_buffer ) ){
				core.CoreClock= atoi( line_buffer );
			}

			TotalThreadCount= ci + 1;
		}
	}
	{
		unsigned int	core_count= TotalThreadCount;
		unsigned int	group_id= 0;
		unsigned int	thread_id= 0;
		std::map<uint64_t,unsigned int>		group_map;
		std::map<uint64_t,unsigned int>		thread_map;
		for( unsigned int ci= 0 ; ci< core_count ; ci++ ){
			auto&	core= CoreList[ci];
			{
				const auto&	thread= thread_map.find( core.ThreadMask );
				if( thread == thread_map.end() ){
					thread_map[ core.ThreadMask ]= thread_id++;
				}
			}
			{
				const auto&	group= group_map.find( core.ClusterMask );
				if( group == group_map.end() ){
					group_map[ core.ClusterMask ]= group_id++;
				}
			}
		}
		PhysicalCoreCount= thread_id;
		CoreGroupCount= group_id;
		for( unsigned int ci= 0 ; ci< core_count ; ci++ ){
			auto&	core= CoreList[ci];
			const auto&	thread= thread_map.find( core.ThreadMask );
			core.PhysicalCoreID= thread->second;
			const auto&	group= group_map.find( core.ClusterMask );
			core.ClusterGroupID= group->second;
		}
	}
}


void SystemInfo::DecodeCpuTopology()
{
	unsigned int	present= GetCpuCount( "/sys/devices/system/cpu/present" );
	{
		unsigned int	online= GetCpuCount( "/sys/devices/system/cpu/online" );
		unsigned int	offline= GetCpuCount( "/sys/devices/system/cpu/offline" );
		if( offline == 0 || online >= present ){
			DecodeCpuTopologyImmediate();
			return;
		}
	}
	time::TickTime	tick_time;
	auto	start_time= tick_time.GetUS();
	thread::AtomicValue<unsigned int>	ExitFlag( 0 );
	ut::FixedArrayPOD<thread::ThreadFunctionBase*>	ThreadArray( present );
	for( unsigned int ti= 0 ; ti< present ; ti++ ){
		auto*	thread= thread::CreateThreadFunction( [&ExitFlag](){
					for(; ExitFlag.Get() == 0 ;){
						math::Matrix4	m1, m2, m3;
						m1.SetIdentity();
						m2.SetIdentity();
						m3.MulCopy( m1, m2 );
					}
				});
		ThreadArray[ti]= thread;
		thread->Run();
	}
	for(;;){
		if( tick_time.GetUS() - start_time > 2000000 ){
			DecodeCpuTopologyImmediate();
			ExitFlag= 1;
			break;
		}
		unsigned int	online= GetCpuCount( "/sys/devices/system/cpu/online" );
		unsigned int	offline= GetCpuCount( "/sys/devices/system/cpu/offline" );
		if( offline == 0 || online >= present ){
			DecodeCpuTopologyImmediate();
			ExitFlag= 1;
			break;
		}
		time::SleepMS( 50 );
	}
	for( unsigned int ti= 0 ; ti< present ; ti++ ){
		auto*	thread= ThreadArray[ti];
		thread->Join();
		memory::SafeDelete( thread );
		ThreadArray[ti]= nullptr;
	}
}


void SystemInfo::DecodeCpuInfo()
{
	bool	info_feature= false;
	bool	info_devname= false;
	const int	MAX_LINE_BYTE= 1024 * 2;
	char	linebuffer[MAX_LINE_BYTE+8];
	FILE*	fp= fopen( "/proc/cpuinfo", "r" );
	if( !fp ){
		return;
	}
	for(; fgets( linebuffer, MAX_LINE_BYTE, fp ) ;){
//FL_LOG( "[%s]\n", linebuffer );
		if( !info_feature ){
			if(
#if FL_CPU_ARM6 || FL_CPU_ARM7 || FL_CPU_ARM64
				!strncmp( linebuffer, "Features", 8 )
#endif
#if FL_CPU_X64 || FL_CPU_X86
				!strncmp( linebuffer, "flags", 5 )
#endif
					){
				char*	lp= SkipColon( linebuffer );
				if( *lp ){
					for(; *lp ;){
						lp= SkipSpace( lp );
						const char*	top= lp;
						for(; *lp && !(*lp <= ' ' && *lp) ; lp++ );
						if( *lp && *lp <= ' ' ){
							*lp++= '\0';
						}
  //FL_LOG( "word=[%s]\n", top );
#if FL_CPU_ARM7 || FL_CPU_ARM64
						if( !strcmp( top, "neon" ) ){
							SetInstructionSet( CPUFeature::ARM_NEON );
						}else if( !strcmp( top, "vfpv4" ) ){
							SetInstructionSet( CPUFeature::ARM_VFPV4 );
						}else if( !strcmp( top, "asimd" ) ){
							SetInstructionSet( CPUFeature::ARM_NEON );
						}else if( !strcmp( top, "fphp" ) ){
							SetInstructionSet( CPUFeature::ARM_FPHP );
						}else if( !strcmp( top, "asimdhp" ) ){
							SetInstructionSet( CPUFeature::ARM_SIMDHP );
						}else if( !strcmp( top, "simddp" ) ){
							SetInstructionSet( CPUFeature::ARM_SIMDDP );
						}else if( !strcmp( top, "sve" ) ){
							SetInstructionSet( CPUFeature::ARM_SVE );
						}else if( !strcmp( top, "crc32" ) ){
							SetInstructionSet( CPUFeature::ARM_CRC32 );
						}else if( !strcmp( top, "sha1" ) ){
							SetInstructionSet( CPUFeature::ARM_SHA1 );
						}else if( !strcmp( top, "sha2" ) ){
							SetInstructionSet( CPUFeature::ARM_SHA2 );
						}else if( !strcmp( top, "sha3" ) ){
							SetInstructionSet( CPUFeature::ARM_SHA3 );
						}else if( !strcmp( top, "sha512" ) ){
							SetInstructionSet( CPUFeature::ARM_SHA512 );
						}else if( !strcmp( top, "aes" ) ){
							SetInstructionSet( CPUFeature::ARM_AES );
						}
#else
						if( !strcmp( top, "aes" ) ){
						}
#endif
					}
				}
				info_feature= true;
			}
		}
		if( !info_devname ){
#if FL_CPU_ARM7 || FL_CPU_ARM64
			if( !strncmp( linebuffer, "Hardware", 8 ) ){
				char*	lp= SkipColon( linebuffer );
				lp= SkipSpace( lp );
				strcpy_s( DeviceName, DEVICE_NAME_BUFFER_SIZE-1, lp );
				info_devname= true;
			}
#endif
#if FL_CPU_X64 || FL_CPU_X86
			if( !strncmp( linebuffer, "model name", 10 ) ){
				char*	lp= SkipColon( linebuffer );
				lp= SkipSpace( lp );
				strcpy_s( DeviceName, DEVICE_NAME_BUFFER_SIZE-1, lp );
				info_devname= true;
			}
#endif
		}
	}
	fclose( fp );
}


//-----------------------------------------------------------------------------
#endif  // Linux
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
#if FL_OS_DARWIN
//-----------------------------------------------------------------------------

namespace {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

static int getSysInt( int id )
{
	int mib[2];
	mib[0]= CTL_HW;
	mib[1]= id;
	int		data= 0;
	size_t	len= sizeof(data);
	sysctl( mib, 2, &data, &len, nullptr, 0 );
	return	data;
}

static void	dumpSysInt( int id, const char* msg )
{
	FL_LOG( "HW INFO: %s = %u\n", msg, getSysInt( id ) );
}

static uint64_t	getSysLong( int id )
{
	int mib[2];
	mib[0]= CTL_HW;
	mib[1]= id;
	uint64_t	data= 0;
	size_t	len= sizeof(data);
	sysctl( mib, 2, &data, &len, nullptr, 0 );
	return	data;
}

static void	dumpSysLong( int id, const char* msg )
{
	FL_LOG( "HW INFO: %s = %llu\n", msg, getSysLong( id ) );
}

static void dumpSysString( int id, const char* msg )
{
	int mib[2];
	mib[0]= CTL_HW;
	mib[1]= id;
	const int	STRING_BUF_SIZE= 128;
	char	buf[STRING_BUF_SIZE];
	size_t	len= STRING_BUF_SIZE;
	sysctl( mib, 2, buf, &len, nullptr, 0 );
	FL_LOG( "HW INFO: %s = %s\n", msg, buf );
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

void SystemInfo::GetHWInfo()
{
	{
		//int		count= getSysInt( HW_NCPU );
		CoreList[0].CoreClock= static_cast<uint32_t>(getSysInt( HW_CPU_FREQ )) / 1000;

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
	}

	{
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
		FL_LOG( "cpu_threadtype   = %d\n",	basic_info.cpu_threadtype ); // 1=HT
		FL_LOG( "physical_cpu     = %d\n",	basic_info.physical_cpu );
		FL_LOG( "physical_cpu_max = %d\n",	basic_info.physical_cpu_max );
		FL_LOG( "logical_cpu      = %d\n",	basic_info.logical_cpu );
		FL_LOG( "logical_cpu_max  = %d\n",	basic_info.logical_cpu_max );
		FL_LOG( "max_mem          = %lld\n", basic_info.max_mem );

		TotalThreadCount= basic_info.logical_cpu_max;
		PhysicalCoreCount= basic_info.physical_cpu_max;
	}

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


	DumpCpuGroup();
}



//-----------------------------------------------------------------------------
#endif // Darwin
//-----------------------------------------------------------------------------



//=============================================================================
// CPU
//=============================================================================



//-----------------------------------------------------------------------------
#if FL_CPU_X86 || FL_CPU_X64
//-----------------------------------------------------------------------------

enum {
	//-- id=1 ECX
	CPUID_1_ECX_FEATURE_SSE3		=	(1<< 0),
	CPUID_1_ECX_FEATURE_MONITOR		=	(1<< 3),
	CPUID_1_ECX_FEATURE_SSSE3		=	(1<< 9),
	CPUID_1_ECX_FEATURE_FMA3		=	(1<<12),
	CPUID_1_ECX_FEATURE_SSE41		=	(1<<19),
	CPUID_1_ECX_FEATURE_SSE42		=	(1<<20),
	CPUID_1_ECX_FEATURE_AES			=	(1<<25),
	CPUID_1_ECX_FEATURE_AVX			=	(1<<28),
	CPUID_1_ECX_FEATURE_F16C		=	(1<<29),
	CPUID_1_ECX_FEATURE_RAND		=	(1<<30),
	//-- id=1 EDX
	CPUID_1_EDX_FEATURE_MMX			=	(1<<23),
	CPUID_1_EDX_FEATURE_SSE			=	(1<<25),
	CPUID_1_EDX_FEATURE_SSE2		=	(1<<26),
	CPUID_1_EDX_FEATURE_HT			=	(1<<28),
	//-- id=7,0 EBX
	CPUID_7_EBX_FEATURE_AVX2		=	(1<< 5),
	CPUID_7_EBX_FEATURE_AVX512F		=	(1<<16),	// Foundation
	CPUID_7_EBX_FEATURE_AVX512DQ	=	(1<<17),	// Doubleword and Quadword instructions
	CPUID_7_EBX_FEATURE_AVX512IFMA	=	(1<<21),	// 
	CPUID_7_EBX_FEATURE_AVX512PF	=	(1<<26),	//
	CPUID_7_EBX_FEATURE_AVX512ER	=	(1<<27),	//
	CPUID_7_EBX_FEATURE_AVX512CD	=	(1<<28),	//
	CPUID_7_EBX_FEATURE_SHA			=	(1<<29),
	CPUID_7_EBX_FEATURE_AVX512BW	=	(1<<30),	// Byte and Word instructions
	CPUID_7_EBX_FEATURE_AVX512VL	=	(1<<31),	// Vector Length extensions
	//-- id=7,0 ECX
	CPUID_7_ECX_FEATURE_AVX512VBMI	=	(1<< 1),	//
	CPUID_7_ECX_FEATURE_AVX512VBMI2	=	(1<< 6),	//
	CPUID_7_ECX_FEATURE_GFNI		=	(1<< 8),	//
	CPUID_7_ECX_FEATURE_VAES		=	(1<< 9),	//
	CPUID_7_ECX_FEATURE_AVX512VNNI	=	(1<<11),	// Neural Network Instructions
	CPUID_7_ECX_FEATURE_AVX512BITALG=	(1<<12),	//
	CPUID_7_ECX_FEATURE_AVX512VPOPCNTDQ=(1<<14),	//
	//-- id=7,0 EDX
	CPUID_7_EDX_FEATURE_AVX5124VNNIW=	(1<< 2),	//
	CPUID_7_EDX_FEATURE_AVX5124FMAPS=	(1<< 3),	//
	CPUID_7_EDX_FEATURE_AVX512VP2INTERSECT=	(1<< 8),	//
	//-- id=7,1 EAX
	CPUID_7_1_EAX_FEATURE_AVX512BFLOAT16=	(1<< 5),	// BFloat16
	//-- id=0x80000001 ECX
	CPUID_80_ECX_FEATURE_SSE4A		=	(1<< 6),
	CPUID_80_ECX_FEATURE_FMA4		=	(1<<16),
	//-- id=0x80000001 EDX
	CPUID_80_EDX_FEATURE_X64		=	(1<<29),	// Long Mode
	CPUID_80_EDX_FEATURE_3DNOWEXT	=	(1<<30),
	CPUID_80_EDX_FEATURE_3DNOW		=	(1<<31),
};

struct RegCPUID {
	unsigned int	EAX;
	unsigned int	EBX;
	unsigned int	ECX;
	unsigned int	EDX;
};

static void	GetCPUID( RegCPUID& reg, unsigned int index, unsigned int subindex= 0 )
{
#if FL_CC_VC
//	__cpuid( reinterpret_cast<int*>(&reg.EAX), index );
	__cpuidex( reinterpret_cast<int*>(&reg.EAX), index, subindex );
#endif
#if FL_CC_CLANG || FL_CC_GCC
	unsigned int	a= 0;
	unsigned int	b= 0;
	unsigned int	c= 0;
	unsigned int	d= 0;
	__cpuid_count( index, subindex, a, b, c, d );
	reg.EAX= a;
	reg.EBX= b;
	reg.ECX= c;
	reg.EDX= d;
#endif
}

void	SystemInfo::GetCPUSpecification()
{
	RegCPUID	Feature01;
	RegCPUID	Feature07;
	RegCPUID	Feature07_01;
	RegCPUID	Feature80;

	GetCPUID( Feature01, 1 );
	GetCPUID( Feature07, 7 );
	GetCPUID( Feature07_01, 7, 1 );
	GetCPUID( Feature80, 0x80000001 );

FL_LOG( "01: %08x %08x %08x %08x\n", Feature01.EAX, Feature01.EBX, Feature01.ECX, Feature01.EDX );
FL_LOG( "07: %08x %08x %08x %08x\n", Feature07.EAX, Feature07.EBX, Feature07.ECX, Feature07.EDX );
FL_LOG( "071 %08x %08x %08x %08x\n", Feature07_01.EAX, Feature07_01.EBX, Feature07_01.ECX, Feature07_01.EDX );
FL_LOG( "80: %08x %08x %08x %08x\n", Feature80.EAX, Feature80.EBX, Feature80.ECX, Feature80.EDX );

	if( Feature01.ECX & CPUID_1_ECX_FEATURE_SSE3 ){
		SetInstructionSet( CPUFeature::IA_SSE3 );
	}
	if( Feature01.ECX & CPUID_1_ECX_FEATURE_SSSE3 ){
		SetInstructionSet( CPUFeature::IA_SSSE3 );
	}
	if( Feature01.ECX & CPUID_1_ECX_FEATURE_SSE41 ){
		SetInstructionSet( CPUFeature::IA_SSE41 );
	}
	if( Feature01.ECX & CPUID_1_ECX_FEATURE_SSE42 ){
		SetInstructionSet( CPUFeature::IA_SSE42 );
	}
	if( Feature01.ECX & CPUID_1_ECX_FEATURE_AES ){
		SetInstructionSet( CPUFeature::IA_AES );
	}
	if( Feature01.ECX & CPUID_1_ECX_FEATURE_AVX ){
		SetInstructionSet( CPUFeature::IA_AVX );
	}
	if( Feature01.ECX & CPUID_1_ECX_FEATURE_FMA3 ){
		SetInstructionSet( CPUFeature::IA_FMA3 );
	}
	if( Feature01.ECX & CPUID_1_ECX_FEATURE_F16C ){
		SetInstructionSet( CPUFeature::IA_F16C );
	}


	if( Feature01.EDX & CPUID_1_EDX_FEATURE_SSE ){
		SetInstructionSet( CPUFeature::IA_SSE );
	}
	if( Feature01.EDX & CPUID_1_EDX_FEATURE_SSE2 ){
		SetInstructionSet( CPUFeature::IA_SSE2 );
	}


	if( Feature07.EBX & CPUID_7_EBX_FEATURE_AVX2 ){
		SetInstructionSet( CPUFeature::IA_AVX2 );
	}
	if( Feature07.EBX & CPUID_7_EBX_FEATURE_AVX512F ){
		SetInstructionSet( CPUFeature::IA_AVX512F );
	}
	if( Feature07.EBX & CPUID_7_EBX_FEATURE_AVX512DQ ){
		SetInstructionSet( CPUFeature::IA_AVX512DQ );
	}
	if( Feature07.EBX & CPUID_7_EBX_FEATURE_SHA ){
		SetInstructionSet( CPUFeature::IA_SHA );
	}
	if( Feature07.EBX & CPUID_7_EBX_FEATURE_AVX512BW ){
		SetInstructionSet( CPUFeature::IA_AVX512BW );
	}
	if( Feature07.EBX & CPUID_7_EBX_FEATURE_AVX512VL ){
		SetInstructionSet( CPUFeature::IA_AVX512VL );
	}

	if( Feature07.ECX & CPUID_7_ECX_FEATURE_AVX512VNNI ){
		SetInstructionSet( CPUFeature::IA_AVX512VNNI );
	}

	if( Feature07_01.EAX & CPUID_7_1_EAX_FEATURE_AVX512BFLOAT16 ){
		SetInstructionSet( CPUFeature::IA_AVX512BF16 );
	}

	if( Feature80.ECX & CPUID_80_ECX_FEATURE_SSE4A ){
		SetInstructionSet( CPUFeature::IA_SSE4A );
	}
	if( Feature80.ECX & CPUID_80_ECX_FEATURE_FMA4 ){
		SetInstructionSet( CPUFeature::IA_FMA4 );
	}

	if( Feature80.EDX & CPUID_80_EDX_FEATURE_X64 ){
		SetInstructionSet( CPUFeature::IA_X64 );
	}
}

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
#if FL_CPU_ARM6 || FL_CPU_ARM7 || FL_CPU_ARM64
//-----------------------------------------------------------------------------

void	SystemInfo::GetCPUSpecification()
{
#if FL_CPU_ARM64
	SetInstructionSet( CPUFeature::ARM_NEON );
	SetInstructionSet( CPUFeature::ARM_VFPV4 );
	SetInstructionSet( CPUFeature::ARM_64 );
#endif
}

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
#if FL_CPU_MIPS || FL_CPU_MIPS64
//-----------------------------------------------------------------------------

void	SystemInfo::GetCPUSpecification()
{
}

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------




//=============================================================================
// Common
//=============================================================================

SystemInfo::SystemInfo() :
			InstructionSet( 0 ),
			TotalThreadCount( 1 ),
			PhysicalCoreCount( 1 ),
			CoreGroupCount( 1 ),
			Initialized( false )
{
	flASSERT( (unsigned int)CPUFeature::FEATURE_MAX <= CPU_FEATURE_MAX );
	memory::MemClear( CoreList );
	memory::MemClear( DeviceName );
}

void	SystemInfo::Init()
{
	if( !Initialized ){
		Initialized= true;
		GetCPUSpecification();
#if FL_OS_PPO
		CoreList[0].CoreClock= 2100000;
		TotalThreadCount=  4;
		PhysicalCoreCount= 2;
#elif FL_OS_LINUX
		DecodeCpuInfo();
		DecodeCpuTopology();
#endif
#if FL_OS_DARWIN
		GetHWInfo();
#endif
	}
	FL_LOG( "InstructionSet= %08x\n", InstructionSet );
}

void SystemInfo::DumpCpuGroup() const
{
	FL_LOG( "Total Core Cound : %d\n", TotalThreadCount );
	unsigned int	core_count= TotalThreadCount;
	for( unsigned int ci= 0 ; ci< core_count ; ci++ ){
		const auto&	core= CoreList[ci];
		FL_PRINT( "%2d: P:%02d G:%02d %8dKHz G:%08llx T:%08llx\n", core.Index, core.PhysicalCoreID, core.ClusterGroupID, core.CoreClock, core.ClusterMask, core.ThreadMask );
	}
}

#define	DEF_CPUFEATURE_NAME_IA(name)	{	#name, CPUFeature::IA_##name	}
#define	DEF_CPUFEATURE_NAME_ARM(name)	{	#name, CPUFeature::ARM_##name	}
#define	DEF_CPUFEATURE_NAME_MIPS(name)	{	#name, CPUFeature::MIPS_##name	}
CPUFeatureNameTable	SystemInfo::FeatureNameTable[]= {
DEF_CPUFEATURE_NAME_IA( SSE ),
DEF_CPUFEATURE_NAME_IA( SSE2 ),
DEF_CPUFEATURE_NAME_IA( SSE3 ),
DEF_CPUFEATURE_NAME_IA( SSSE3 ),
DEF_CPUFEATURE_NAME_IA( SSE41 ),
DEF_CPUFEATURE_NAME_IA( SSE42 ),
DEF_CPUFEATURE_NAME_IA( SSE4A ),
DEF_CPUFEATURE_NAME_IA( AVX ),
DEF_CPUFEATURE_NAME_IA( AVX2 ),
DEF_CPUFEATURE_NAME_IA( AVX512F ),
DEF_CPUFEATURE_NAME_IA( AVX512BW ),
DEF_CPUFEATURE_NAME_IA( AVX512DQ ),
DEF_CPUFEATURE_NAME_IA( AVX512VL ),
DEF_CPUFEATURE_NAME_IA( AVX512VNNI ),
DEF_CPUFEATURE_NAME_IA( AVX512BF16 ),
DEF_CPUFEATURE_NAME_IA( AES ),
DEF_CPUFEATURE_NAME_IA( FMA3 ),
DEF_CPUFEATURE_NAME_IA( FMA4 ),
DEF_CPUFEATURE_NAME_IA( SHA ),
DEF_CPUFEATURE_NAME_IA( F16C ),
DEF_CPUFEATURE_NAME_IA( X64 ),

DEF_CPUFEATURE_NAME_ARM( NEON ),
DEF_CPUFEATURE_NAME_ARM( VFPV4 ),
DEF_CPUFEATURE_NAME_ARM( FPHP ),
DEF_CPUFEATURE_NAME_ARM( SIMDHP ),
DEF_CPUFEATURE_NAME_ARM( SIMDDP ),
DEF_CPUFEATURE_NAME_ARM( SVE ),
DEF_CPUFEATURE_NAME_ARM( CRC32 ),
DEF_CPUFEATURE_NAME_ARM( SHA1 ),
DEF_CPUFEATURE_NAME_ARM( SHA2 ),
DEF_CPUFEATURE_NAME_ARM( SHA3 ),
DEF_CPUFEATURE_NAME_ARM( SHA512 ),
DEF_CPUFEATURE_NAME_ARM( AES ),
DEF_CPUFEATURE_NAME_ARM( 64 ),
DEF_CPUFEATURE_NAME_MIPS( MSA ),
DEF_CPUFEATURE_NAME_MIPS( F64 ),
DEF_CPUFEATURE_NAME_MIPS( PS ),
};

const char* SystemInfo::GetFeatureName( CPUFeature feature ) const
{
	flASSERT( sizeof(FeatureNameTable)/sizeof(CPUFeatureNameTable) == (int)CPUFeature::FEATURE_MAX );
	if( feature < CPUFeature::FEATURE_MAX ){
		unsigned int	index= (unsigned int)feature;
		const auto&	name= FeatureNameTable[index];
		flASSERT( name.Feature == feature );
		return	name.FeatureName;
	}
	return	"Unknown";
}

void SystemInfo::GetCpuFeatureString( char* buffer, size_t buffer_size ) const
{
	char*	str= buffer;
	char*	str_end= buffer + buffer_size;
	for( unsigned int fi= 0 ; str < str_end && fi< (unsigned int)CPUFeature::FEATURE_MAX ; fi++ ){
		if( HasInstructionSet( (CPUFeature)fi ) ){
			const char*	ptr= GetFeatureName( (CPUFeature)fi );
			strcpy_s( str, str_end - str, ptr );
			str+= strlen(ptr);
			*str++= ' ';
		}
	}
	*str= '\0';
}

void SystemInfo::DumpSystemInfo() const
{
	DumpCpuGroup();
	const int	CPU_FEATURE_BUFFER_SIZE= 512;
	char	buffer[CPU_FEATURE_BUFFER_SIZE];
	GetCpuFeatureString( buffer, CPU_FEATURE_BUFFER_SIZE );
	FL_PRINT( "%s\n", buffer );
}

unsigned int	SystemInfo::GetCoreClock( unsigned int group_index ) const
{
	unsigned int	core_count= TotalThreadCount;
	for( unsigned int ci= 0 ; ci< core_count ; ci++ ){
		const auto&	core= CoreList[ci];
		if( core.ClusterGroupID == group_index ){
			return	core.CoreClock;
		}
	}
	return	0;
}

unsigned int	SystemInfo::GetThreadCount( unsigned int group_index ) const
{
	unsigned int	thread_count= 0;
	unsigned int	core_count= TotalThreadCount;
	for( unsigned int ci= 0 ; ci< core_count ; ci++ ){
		const auto&	core= CoreList[ci];
		if( core.ClusterGroupID == group_index ){
			thread_count++;
		}
	}
	return	thread_count;
}

uint64_t	SystemInfo::GetAffinityMask( unsigned int group_index ) const
{
	unsigned int	core_count= TotalThreadCount;
	for( unsigned int ci= 0 ; ci< core_count ; ci++ ){
		const auto&	core= CoreList[ci];
		if( core.ClusterGroupID == group_index ){
			return	core.ClusterMask;
		}
	}
	return	0;
}

bool	SystemInfo::HasInstructionSet( CPUFeature feature ) const
{
	flASSERT( feature < CPUFeature::FEATURE_MAX );
	return	(InstructionSet& (1<<(unsigned int)feature)) != 0;
}

void	SystemInfo::SetInstructionSet( CPUFeature feature )
{
	flASSERT( feature < CPUFeature::FEATURE_MAX );
	InstructionSet|= 1<<(unsigned int)feature;
}

const char*	SystemInfo::GetDeviceName() const
{
	return	DeviceName;
}

CPUArch	SystemInfo::GetArch() const
{
#if FL_CPU_ARM64
	return	CPUArch::CPU_ARM64;
#elif FL_CPU_ARM7
	return	CPUArch::CPU_ARM7;
#elif FL_CPU_ARM5
	return	CPUArch::CPU_ARM5;
#elif FL_CPU_ARM6
	return	CPUArch::CPU_ARM6;
#elif FL_CPU_X86
	return	CPUArch::CPU_X86;
#elif FL_CPU_X64
	return	CPUArch::CPU_X64;
#elif FL_CPU_MIPS32
	return	CPUArch::CPU_MIPS32;
#elif FL_CPU_MIPS64
	return	CPUArch::CPU_MIPS64;
#else
	return	CPUArch::CPU_UNKNOWN;
#endif
}

const char* SystemInfo::GetArchNameLong() const
{
#if FL_CPU_ARM64
	if( HasInstructionSet( CPUFeature::ARM_FPHP ) ){
		return	"ARMv8.2A AArch64";
	}
	return	"ARMv8A AArch64";
#elif FL_CPU_ARM7
	return	"ARMv7A";
#elif FL_CPU_ARM5
	return	"ARMv5TE";
#elif FL_CPU_ARM6
	return	"ARMv6";
#elif FL_CPU_X86
	return	"x86";
#elif FL_CPU_X64
	return	"x64 (x86_64)";
#elif FL_CPU_MIPS32
	return	"MIPS32";
#elif FL_CPU_MIPS64
	return	"MIPS64";
#else
	return	"Unknown";
#endif
}

const char* SystemInfo::GetArchNameShort() const
{
#if FL_CPU_ARM64
	return	"ARMv8A";
#elif FL_CPU_ARM7
	return	"ARMv7A";
#elif FL_CPU_ARM5
	return	"ARMv5TE";
#elif FL_CPU_ARM6
	return	"ARMv6";
#elif FL_CPU_X86
	return	"x86";
#elif FL_CPU_X64
	return	"x64";
#elif FL_CPU_MIPS32
	return	"MIPS32";
#elif FL_CPU_MIPS64
	return	"MIPS64";
#else
	return	"Unknown";
#endif
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}

