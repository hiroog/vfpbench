// 2013 Hiroyuki Ogasawara
// vim:ts=4 sw=4 noet:

#ifndef FLATLIB_PLATFORM_H_
#define FLATLIB_PLATFORM_H_


//-----------------------------------------------------------------------------
// MacOSX
//-----------------------------------------------------------------------------
#if defined(flPRESET_OSX) && flPRESET_OSX
# if defined(__i386__)
#  define	flCPU_X86			1
# elif defined(__x86_64__)
#  define	flCPU_X64			1
# else
#  error "macos"
# endif
# define	flOS_UNIX			1
# define	flOS_DARWIN			1
# define	flOS_OSX			1
# define	flCC_CLANG			1
# define	flSYSTEMTYPE_DEFINED
#endif




//-----------------------------------------------------------------------------
// iOS
//-----------------------------------------------------------------------------
#if defined(flPRESET_IOS) && flPRESET_IOS
# if defined(__i386__)
#  define	flCPU_X86			1
# elif defined(__x86_64__)
#  define	flCPU_X64			1
# elif defined(__arm64__)
#  define	flCPU_ARM64			1
#  define	flCPU_NEON			1
# elif __ARM_ARCH == 7
#  define	flCPU_ARM7			1
#  define	flCPU_NEON			1
# else
#  define	flCPU_ARM6			1
#  define	flCPU_NEON			0
# endif
# define	flOS_UNIX			1
# define	flOS_DARWIN			1
# define	flOS_IOS			1
# define	flCC_CLANG			1
# ifdef __ARM_VFPV4__
#  define	flCPU_VFPV4			1
# else
#  define	flCPU_VFPV4			0
# endif
# define	flSYSTEMTYPE_DEFINED
#endif





//-----------------------------------------------------------------------------
// Linux
//-----------------------------------------------------------------------------
#if defined(flPRESET_LINUX) && flPRESET_LINUX
# if defined(__x86_64__)
#  define	flCPU_X64			1
# elif defined(__aarch64__)
#  define	flCPU_ARM64			1
#  define	flCPU_NEON			1
# elif defined(__i386__)
#  define	flCPU_X86			1
# elif defined(__ARM_ARCH_6__)
#  define	flCPU_ARM6			1
# elif defined(__arm__)
#  define	flCPU_ARM7			1
#  define	flCPU_NEON			1
# endif
# define	flOS_UNIX			1
# define	flOS_LINUX			1
# ifdef __clang__
#  define	flCC_CLANG			1
# else
#  define	flCC_GCC			1
# endif
# ifdef __ARM_FEATURE_FMA
#  define	flCPU_VFPV4			1 // nexus7=0, raspberry pi 2=1
# endif
# define	flSYSTEMTYPE_DEFINED
#endif





//-----------------------------------------------------------------------------
// Android
//-----------------------------------------------------------------------------
#if defined(flPRESET_ANDROID) && flPRESET_ANDROID
# if defined(__x86_64__)
#  define	flCPU_X64			1
# elif defined(__AARCH64EL__)
#  define	flCPU_ARM64			1
# elif defined(__mips64)
#  define	flCPU_MIPS64		1
# elif defined(__i386__)
#  define	flCPU_X86			1
# elif defined(__mips__)
#  define	flCPU_MIPS32		1
# elif defined(__arm__)
#  if defined(flCPU_ARM5) && flCPU_ARM5
#	define	flCPU_ARM5			1
#  else
#   define	flCPU_ARM7			1
#  endif
# endif
# define	flOS_UNIX			1
# define	flOS_ANDROID		1
# ifdef __clang__
#  define	flCC_CLANG			1
# else
#  define	flCC_GCC			1
# endif
# define	flSYSTEMTYPE_DEFINED
#endif



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#define	flCPU_ARM	(flCPU_ARM5 || flCPU_ARM6 || flCPU_ARM7 || flCPU_ARM64)


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


#ifndef	flSYSTEMTYPE_DEFINED
# error "flPRESET_* not defined"
#endif


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// CPU
#ifndef flCPU_X86
# define	flCPU_X86			0
#endif
#ifndef	flCPU_X64
# define	flCPU_X64			0
#endif
#ifndef	flCPU_ARM5
# define	flCPU_ARM5			0
#endif
#ifndef	flCPU_ARM6
# define	flCPU_ARM6			0
#endif
#ifndef	flCPU_ARM7
# define	flCPU_ARM7			0
#endif
#ifndef	flCPU_ARM64
# define	flCPU_ARM64			0
#endif
#ifndef	flCPU_MIPS32
# define	flCPU_MIPS32		0
#endif
#ifndef	flCPU_MIPS64
# define	flCPU_MIPS64		0
#endif
#ifndef	flCPU_PPC64
# define	flCPU_PPC64			0
#endif


#ifndef	flCPU_NEON
# define	flCPU_NEON			0
#endif
#ifndef	flCPU_VFPV4
# define	flCPU_VFPV4			0
#endif



// OS
#ifndef	flOS_WINDOWS
# define	flOS_WINDOWS		0
#endif
#ifndef	flOS_UNIX
# define	flOS_UNIX			0
#endif
#ifndef	flOS_DARWIN
# define	flOS_DARWIN			0
#endif

#ifndef	flOS_LINUX
# define	flOS_LINUX			0
#endif
#ifndef	flOS_ANDROID
# define	flOS_ANDROID		0
#endif
#ifndef	flOS_OSX
# define	flOS_OSX			0
#endif
#ifndef	flOS_IOS
# define	flOS_IOS			0
#endif




// COMPILER
#ifndef	flCC_VC
# define	flCC_VC				0
#endif
#ifndef	flCC_GCC
# define	flCC_GCC			0
#endif
#ifndef	flCC_CLANG
# define	flCC_CLANG			0
#endif




//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#ifndef	flDEBUG
# if defined(NDEBUG)
#  define	flDEBUG			0
# elif defined(_DEBUG) || defined(DEBUG)
#  define	flDEBUG			1
# else
#  define	flDEBUG			1
# endif
#endif
#ifndef	flRETAIL
# define	flRETAIL		0
#endif




//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#endif

