#pragma once

// Cygwin
// This is a pseudo-platform which will be defined along with ABYTEK_PLATFORM_LINUX when
// using the Cygwin build environment.
#if defined(__CYGWIN__)
	#define ABYTEK_PLATFORM_CYGWIN 1
	#define ABYTEK_PLATFORM_DESKTOP 1
#endif

// MinGW
// This is a pseudo-platform which will be defined along with ABYTEK_PLATFORM_WINDOWS when
// using the MinGW Windows build environment.
#if defined(__MINGW32__) || defined(__MINGW64__)
	#define ABYTEK_PLATFORM_MINGW 1
	#define ABYTEK_PLATFORM_DESKTOP 1
#endif

#if defined(ABYTEK_PLATFORM_PS4) || defined(__ORBIS__) || defined(ABYTEK_PLATFORM_KETTLE)
	// PlayStation 4
	// Orbis was Sony's code-name for the platform, which is now obsolete.
	// Kettle was an EA-specific code-name for the platform, which is now obsolete.
	#if defined(ABYTEK_PLATFORM_PS4)
		#undef  ABYTEK_PLATFORM_PS4
	#endif
	#define ABYTEK_PLATFORM_PS4 1

	// Backward compatibility:
		#if defined(ABYTEK_PLATFORM_KETTLE)
			#undef  ABYTEK_PLATFORM_KETTLE
		#endif
	// End backward compatbility

	#define ABYTEK_PLATFORM_KETTLE 1
	#define ABYTEK_PLATFORM_NAME "PS4"
	#define ABYTEK_SYSTEM_LITTLE_ENDIAN 1
	#define ABYTEK_PLATFORM_DESCRIPTION "PS4 on x64"
	#define ABYTEK_PLATFORM_CONSOLE 1
	#define ABYTEK_PLATFORM_SONY 1
	#define ABYTEK_PLATFORM_POSIX 1
	// #define ABYTEK_POSIX_THREADS_AVAILABLE 1  // POSIX threading API is available but discouraged.  Sony indicated use of the scePthreads* API is preferred.
	#define ABYTEK_PROCESSOR_X86_64 1
	#if defined(__GNUC__) || defined(__clang__)
		#define ABYTEK_ASM_STYLE_ATT 1
	#endif

#elif defined(ABYTEK_PLATFORM_XBOXONE) || defined(_DURANGO) || defined(_XBOX_ONE) || defined(ABYTEK_PLATFORM_CAPILANO) || defined(_GAMING_XBOX)
	// XBox One
	// Durango was Microsoft's code-name for the platform, which is now obsolete.
	// Microsoft uses _DURANGO instead of some variation of _XBOX, though it's not natively defined by the compiler.
	// Capilano was an EA-specific code-name for the platform, which is now obsolete.
	#if defined(ABYTEK_PLATFORM_XBOXONE)
		#undef  ABYTEK_PLATFORM_XBOXONE
	#endif
	#define ABYTEK_PLATFORM_XBOXONE 1

	// Backward compatibility:
		#if defined(ABYTEK_PLATFORM_CAPILANO)
			#undef  ABYTEK_PLATFORM_CAPILANO
		#endif
		#define ABYTEK_PLATFORM_CAPILANO 1
		#if defined(ABYTEK_PLATFORM_CAPILANO_XDK) && !defined(ABYTEK_PLATFORM_XBOXONE_XDK)
			#define ABYTEK_PLATFORM_XBOXONE_XDK 1
		#endif
		#if defined(ABYTEK_PLATFORM_CAPILANO_ADK) && !defined(ABYTEK_PLATFORM_XBOXONE_ADK)
			#define ABYTEK_PLATFORM_XBOXONE_ADK 1
		#endif
	// End backward compatibility

	#if !defined(_DURANGO)
		#define _DURANGO
	#endif
	#define ABYTEK_PLATFORM_NAME "XBox One"
  //#define ABYTEK_PROCESSOR_X86  Currently our policy is that we don't define this, even though x64 is something of a superset of x86.
	#define ABYTEK_PROCESSOR_X86_64 1
	#define ABYTEK_SYSTEM_LITTLE_ENDIAN 1
	#define ABYTEK_PLATFORM_DESCRIPTION "XBox One on x64"
	#define ABYTEK_ASM_STYLE_INTEL 1
	#define ABYTEK_PLATFORM_CONSOLE 1
	#define ABYTEK_PLATFORM_MICROSOFT 1

	// WINAPI_FAMILY defines - mirrored from winapifamily.h
	#define ABYTEK_WINAPI_FAMILY_APP         1000
	#define ABYTEK_WINAPI_FAMILY_DESKTOP_APP 1001
	#define ABYTEK_WINAPI_FAMILY_PHONE_APP   1002
	#define ABYTEK_WINAPI_FAMILY_TV_APP      1003
	#define ABYTEK_WINAPI_FAMILY_TV_TITLE    1004
	#define ABYTEK_WINAPI_FAMILY_GAMES       1006

	#if defined(WINAPI_FAMILY)
		#include <winapifamily.h>
		#if defined(WINAPI_FAMILY_TV_TITLE) && WINAPI_FAMILY == WINAPI_FAMILY_TV_TITLE
			#define ABYTEK_WINAPI_FAMILY ABYTEK_WINAPI_FAMILY_TV_TITLE
		#elif defined(WINAPI_FAMILY_DESKTOP_APP) && WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
			#define ABYTEK_WINAPI_FAMILY ABYTEK_WINAPI_FAMILY_DESKTOP_APP
		#elif defined(WINAPI_FAMILY_GAMES) && WINAPI_FAMILY == WINAPI_FAMILY_GAMES
			#define ABYTEK_WINAPI_FAMILY ABYTEK_WINAPI_FAMILY_GAMES
		#else
			#error Unsupported WINAPI_FAMILY
		#endif
	#else
		#error WINAPI_FAMILY should always be defined on Capilano.
	#endif

	// Macro to determine if a partition is enabled.
	#define ABYTEK_WINAPI_FAMILY_PARTITION(Partition)	(Partition)

	#if ABYTEK_WINAPI_FAMILY == ABYTEK_WINAPI_FAMILY_DESKTOP_APP
		#define ABYTEK_WINAPI_PARTITION_CORE     1
		#define ABYTEK_WINAPI_PARTITION_DESKTOP  1
		#define ABYTEK_WINAPI_PARTITION_APP      1
		#define ABYTEK_WINAPI_PARTITION_PC_APP   0
		#define ABYTEK_WIANPI_PARTITION_PHONE    0
		#define ABYTEK_WINAPI_PARTITION_TV_APP   0
		#define ABYTEK_WINAPI_PARTITION_TV_TITLE 0
		#define ABYTEK_WINAPI_PARTITION_GAMES    0
	#elif ABYTEK_WINAPI_FAMILY == ABYTEK_WINAPI_FAMILY_TV_TITLE
		#define ABYTEK_WINAPI_PARTITION_CORE     1
		#define ABYTEK_WINAPI_PARTITION_DESKTOP  0
		#define ABYTEK_WINAPI_PARTITION_APP      0
		#define ABYTEK_WINAPI_PARTITION_PC_APP   0
		#define ABYTEK_WIANPI_PARTITION_PHONE    0
		#define ABYTEK_WINAPI_PARTITION_TV_APP   0
		#define ABYTEK_WINAPI_PARTITION_TV_TITLE 1
		#define ABYTEK_WINAPI_PARTITION_GAMES    0
	#elif ABYTEK_WINAPI_FAMILY == ABYTEK_WINAPI_FAMILY_GAMES
		#define ABYTEK_WINAPI_PARTITION_CORE     1
		#define ABYTEK_WINAPI_PARTITION_DESKTOP  0
		#define ABYTEK_WINAPI_PARTITION_APP      0
		#define ABYTEK_WINAPI_PARTITION_PC_APP   0
		#define ABYTEK_WIANPI_PARTITION_PHONE    0
		#define ABYTEK_WINAPI_PARTITION_TV_APP   0
		#define ABYTEK_WINAPI_PARTITION_TV_TITLE 0
		#define ABYTEK_WINAPI_PARTITION_GAMES    1
	#else
		#error Unsupported WINAPI_FAMILY
	#endif

	#if ABYTEK_WINAPI_FAMILY_PARTITION(ABYTEK_WINAPI_PARTITION_GAMES)
		#define CS_UNDEFINED_STRING 			1
		#define CS_UNDEFINED_STRING 		1
	#endif

	#if ABYTEK_WINAPI_FAMILY_PARTITION(ABYTEK_WINAPI_PARTITION_TV_TITLE)
		#define ABYTEK_PLATFORM_XBOXONE_XDK 	1
	#endif
#elif defined(ABYTEK_PLATFORM_LRB) || defined(__LRB__) || (defined(__EDG__) && defined(__ICC) && defined(__x86_64__))
	#undef  ABYTEK_PLATFORM_LRB
	#define ABYTEK_PLATFORM_LRB         1
	#define ABYTEK_PLATFORM_NAME        "Larrabee"
	#define ABYTEK_PLATFORM_DESCRIPTION "Larrabee on LRB1"
	#define ABYTEK_PROCESSOR_X86_64 1
	#if defined(BYTE_ORDER) && (BYTE_ORDER == 4321)
		#define ABYTEK_SYSTEM_BIG_ENDIAN 1
	#else
		#define ABYTEK_SYSTEM_LITTLE_ENDIAN 1
	#endif
	#define ABYTEK_PROCESSOR_LRB 1
	#define ABYTEK_PROCESSOR_LRB1 1       // Larrabee version 1
	#define ABYTEK_ASM_STYLE_ATT 1        // Both types of asm style
	#define ABYTEK_ASM_STYLE_INTEL 1      // are supported.
	#define ABYTEK_PLATFORM_DESKTOP 1

// Android (Google phone OS)
#elif defined(ABYTEK_PLATFORM_ANDROID) || defined(__ANDROID__)
	#undef  ABYTEK_PLATFORM_ANDROID
	#define ABYTEK_PLATFORM_ANDROID 1
	#define ABYTEK_PLATFORM_LINUX 1
	#define ABYTEK_PLATFORM_UNIX 1
	#define ABYTEK_PLATFORM_POSIX 1
	#define ABYTEK_PLATFORM_NAME "Android"
	#define ABYTEK_ASM_STYLE_ATT 1
	#if defined(__arm__)
		#define ABYTEK_ABI_ARM_LINUX 1  // a.k.a. "ARM eabi"
		#define ABYTEK_PROCESSOR_ARM32 1
		#define ABYTEK_PLATFORM_DESCRIPTION "Android on ARM"
	#elif defined(__aarch64__)
		#define ABYTEK_PROCESSOR_ARM64 1
		#define ABYTEK_PLATFORM_DESCRIPTION "Android on ARM64"
	#elif defined(__i386__)
		#define ABYTEK_PROCESSOR_X86 1
		#define ABYTEK_PLATFORM_DESCRIPTION "Android on x86"
	#elif defined(__x86_64)
		#define ABYTEK_PROCESSOR_X86_64 1
		#define ABYTEK_PLATFORM_DESCRIPTION "Android on x64"
	#else
		#error Unknown processor
	#endif
	#if !defined(ABYTEK_SYSTEM_BIG_ENDIAN) && !defined(ABYTEK_SYSTEM_LITTLE_ENDIAN)
		#define ABYTEK_SYSTEM_LITTLE_ENDIAN 1
	#endif
	#define ABYTEK_PLATFORM_MOBILE 1

// Samsung SMART TV - a Linux-based smart TV
#elif defined(ABYTEK_PLATFORM_SAMSUNG_TV)
	#undef  ABYTEK_PLATFORM_SAMSUNG_TV
	#define ABYTEK_PLATFORM_SAMSUNG_TV 1
	#define ABYTEK_PLATFORM_LINUX 1
	#define ABYTEK_PLATFORM_UNIX 1
	#define ABYTEK_PLATFORM_POSIX 1
	#define ABYTEK_PLATFORM_NAME "SamsungTV"
	#define ABYTEK_PLATFORM_DESCRIPTION "Samsung SMART TV on ARM"
	#define ABYTEK_ASM_STYLE_ATT 1
	#define ABYTEK_SYSTEM_LITTLE_ENDIAN 1
	#define ABYTEK_PROCESSOR_ARM32 1
	#define ABYTEK_ABI_ARM_LINUX 1 // a.k.a. "ARM eabi"
	#define ABYTEK_PROCESSOR_ARM7 1

#elif defined(__APPLE__) && __APPLE__
	#include <TargetConditionals.h>

	// Apple family of operating systems.
	#define ABYTEK_PLATFORM_APPLE
	#define ABYTEK_PLATFORM_POSIX 1

	// iPhone
	// TARGET_OS_IPHONE will be undefined on an unknown compiler, and will be defined on gcc.
	#if defined(ABYTEK_PLATFORM_IPHONE) || defined(__IPHONE__) || (defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) || (defined(TARGET_IPHONE_SIMULATOR) && TARGET_IPHONE_SIMULATOR)
		#undef  ABYTEK_PLATFORM_IPHONE
		#define ABYTEK_PLATFORM_IPHONE 1
		#define ABYTEK_PLATFORM_NAME "iPhone"
		#define ABYTEK_ASM_STYLE_ATT 1
		#define ABYTEK_POSIX_THREADS_AVAILABLE 1
		#if defined(__arm__)
			#define ABYTEK_ABI_ARM_APPLE 1
			#define ABYTEK_PROCESSOR_ARM32 1
			#define ABYTEK_SYSTEM_LITTLE_ENDIAN 1
			#define ABYTEK_PLATFORM_DESCRIPTION "iPhone on ARM"
		#elif defined(__aarch64__) || defined(__AARCH64)
			#define ABYTEK_ABI_ARM64_APPLE 1
			#define ABYTEK_PROCESSOR_ARM64 1
			#define ABYTEK_SYSTEM_LITTLE_ENDIAN 1
			#define ABYTEK_PLATFORM_DESCRIPTION "iPhone on ARM64"
		#elif defined(__i386__)
			#define ABYTEK_PLATFORM_IPHONE_SIMULATOR 1
			#define ABYTEK_PROCESSOR_X86 1
			#define ABYTEK_SYSTEM_LITTLE_ENDIAN 1
			#define ABYTEK_PLATFORM_DESCRIPTION "iPhone simulator on x86"
		#elif defined(__x86_64) || defined(__amd64)
			#define ABYTEK_PROCESSOR_X86_64 1
			#define ABYTEK_SYSTEM_LITTLE_ENDIAN 1
			#define ABYTEK_PLATFORM_DESCRIPTION "iPhone simulator on x64"
		#else
			#error Unknown processor
		#endif
		#define ABYTEK_PLATFORM_MOBILE 1

	// Macintosh OSX
	// TARGET_OS_MAC is defined by the Metrowerks and older AppleC compilers.
	// Howerver, TARGET_OS_MAC is defined to be 1 in all cases.
	// __i386__ and __intel__ are defined by the GCC compiler.
	// __dest_os is defined by the Metrowerks compiler.
	// __MACH__ is defined by the Metrowerks and GCC compilers.
	// powerc and __powerc are defined by the Metrowerks and GCC compilers.
	#elif defined(ABYTEK_PLATFORM_OSX) || defined(__MACH__) || (defined(__MSL__) && (__dest_os == __mac_os_x))
		#undef  ABYTEK_PLATFORM_OSX
		#define ABYTEK_PLATFORM_OSX 1
		#define ABYTEK_PLATFORM_UNIX 1
		#define ABYTEK_PLATFORM_POSIX 1
	  //#define ABYTEK_PLATFORM_BSD 1           We don't currently define this. OSX has some BSD history but a lot of the API is different.
		#define ABYTEK_PLATFORM_NAME "OSX"
		#if defined(__i386__) || defined(__intel__)
			#define ABYTEK_PROCESSOR_X86 1
			#define ABYTEK_SYSTEM_LITTLE_ENDIAN 1
			#define ABYTEK_PLATFORM_DESCRIPTION "OSX on x86"
		#elif defined(__x86_64) || defined(__amd64)
			#define ABYTEK_PROCESSOR_X86_64 1
			#define ABYTEK_SYSTEM_LITTLE_ENDIAN 1
			#define ABYTEK_PLATFORM_DESCRIPTION "OSX on x64"
		#elif defined(__arm__)
			#define ABYTEK_ABI_ARM_APPLE 1
			#define ABYTEK_PROCESSOR_ARM32 1
			#define ABYTEK_SYSTEM_LITTLE_ENDIAN 1
			#define ABYTEK_PLATFORM_DESCRIPTION "OSX on ARM"
		#elif defined(__aarch64__) || defined(__AARCH64)
			#define ABYTEK_ABI_ARM64_APPLE 1
			#define ABYTEK_PROCESSOR_ARM64 1
			#define ABYTEK_SYSTEM_LITTLE_ENDIAN 1
			#define ABYTEK_PLATFORM_DESCRIPTION "OSX on ARM64"
		#elif defined(__POWERPC64__) || defined(__powerpc64__)
			#define ABYTEK_PROCESSOR_POWERPC 1
			#define ABYTEK_PROCESSOR_POWERPC_64 1
			#define ABYTEK_SYSTEM_BIG_ENDIAN 1
			#define ABYTEK_PLATFORM_DESCRIPTION "OSX on PowerPC 64"
		#elif defined(__POWERPC__) || defined(__powerpc__)
			#define ABYTEK_PROCESSOR_POWERPC 1
			#define ABYTEK_PROCESSOR_POWERPC_32 1
			#define ABYTEK_SYSTEM_BIG_ENDIAN 1
			#define ABYTEK_PLATFORM_DESCRIPTION "OSX on PowerPC"
		#else
			#error Unknown processor
		#endif
		#if defined(__GNUC__)
			#define ABYTEK_ASM_STYLE_ATT 1
		#else
			#define ABYTEK_ASM_STYLE_MOTOROLA 1
		#endif
		#define ABYTEK_PLATFORM_DESKTOP 1
	#else
		#error Unknown Apple Platform
	#endif

// Linux
// __linux and __linux__ are defined by the GCC and Borland compiler.
// __i386__ and __intel__ are defined by the GCC compiler.
// __i386__ is defined by the Metrowerks compiler.
// _M_IX86 is defined by the Borland compiler.
// __sparc__ is defined by the GCC compiler.
// __powerpc__ is defined by the GCC compiler.
// __ARM_EABI__ is defined by GCC on an ARM v6l (Raspberry Pi 1)
// __ARM_ARCH_7A__ is defined by GCC on an ARM v7l (Raspberry Pi 2)
#elif defined(ABYTEK_PLATFORM_LINUX) || (defined(__linux) || defined(__linux__))
	#undef  ABYTEK_PLATFORM_LINUX
	#define ABYTEK_PLATFORM_LINUX 1
	#define ABYTEK_PLATFORM_UNIX 1
	#define ABYTEK_PLATFORM_POSIX 1
	#define ABYTEK_PLATFORM_NAME "Linux"
	#if defined(__i386__) || defined(__intel__) || defined(_M_IX86)
		#define ABYTEK_PROCESSOR_X86 1
		#define ABYTEK_SYSTEM_LITTLE_ENDIAN 1
		#define ABYTEK_PLATFORM_DESCRIPTION "Linux on x86"
	#elif defined(__ARM_ARCH_7A__) || defined(__ARM_EABI__)
		#define ABYTEK_ABI_ARM_LINUX 1
		#define ABYTEK_PROCESSOR_ARM32 1
		#define ABYTEK_PLATFORM_DESCRIPTION "Linux on ARM 6/7 32-bits"
	#elif defined(__aarch64__) || defined(__AARCH64)
		#define ABYTEK_PROCESSOR_ARM64 1
		#define ABYTEK_PLATFORM_DESCRIPTION "Linux on ARM64"
	#elif defined(__x86_64__)
		#define ABYTEK_PROCESSOR_X86_64 1
		#define ABYTEK_SYSTEM_LITTLE_ENDIAN 1
		#define ABYTEK_PLATFORM_DESCRIPTION "Linux on x64"
	#elif defined(__powerpc64__)
		#define ABYTEK_PROCESSOR_POWERPC 1
		#define ABYTEK_PROCESSOR_POWERPC_64 1
		#define ABYTEK_SYSTEM_BIG_ENDIAN 1
		#define ABYTEK_PLATFORM_DESCRIPTION "Linux on PowerPC 64"
	#elif defined(__powerpc__)
		#define ABYTEK_PROCESSOR_POWERPC 1
		#define ABYTEK_PROCESSOR_POWERPC_32 1
		#define ABYTEK_SYSTEM_BIG_ENDIAN 1
		#define ABYTEK_PLATFORM_DESCRIPTION "Linux on PowerPC"
	#else
		#error Unknown processor
		#error Unknown endianness
	#endif
	#if defined(__GNUC__)
		#define ABYTEK_ASM_STYLE_ATT 1
	#endif
	#define ABYTEK_PLATFORM_DESKTOP 1


#elif defined(ABYTEK_PLATFORM_BSD) || (defined(__BSD__) || defined(__FreeBSD__))
	#undef  ABYTEK_PLATFORM_BSD
	#define ABYTEK_PLATFORM_BSD 1
	#define ABYTEK_PLATFORM_UNIX 1
	#define ABYTEK_PLATFORM_POSIX 1     // BSD's posix complaince is not identical to Linux's
	#define ABYTEK_PLATFORM_NAME "BSD Unix"
	#if defined(__i386__) || defined(__intel__)
		#define ABYTEK_PROCESSOR_X86 1
		#define ABYTEK_SYSTEM_LITTLE_ENDIAN 1
		#define ABYTEK_PLATFORM_DESCRIPTION "BSD on x86"
	#elif defined(__x86_64__)
		#define ABYTEK_PROCESSOR_X86_64 1
		#define ABYTEK_SYSTEM_LITTLE_ENDIAN 1
		#define ABYTEK_PLATFORM_DESCRIPTION "BSD on x64"
	#elif defined(__powerpc64__)
		#define ABYTEK_PROCESSOR_POWERPC 1
		#define ABYTEK_PROCESSOR_POWERPC_64 1
		#define ABYTEK_SYSTEM_BIG_ENDIAN 1
		#define ABYTEK_PLATFORM_DESCRIPTION "BSD on PowerPC 64"
	#elif defined(__powerpc__)
		#define ABYTEK_PROCESSOR_POWERPC 1
		#define ABYTEK_PROCESSOR_POWERPC_32 1
		#define ABYTEK_SYSTEM_BIG_ENDIAN 1
		#define ABYTEK_PLATFORM_DESCRIPTION "BSD on PowerPC"
	#else
		#error Unknown processor
		#error Unknown endianness
	#endif
	#if !defined(ABYTEK_PLATFORM_FREEBSD) && defined(__FreeBSD__)
		#define ABYTEK_PLATFORM_FREEBSD 1 // This is a variation of BSD.
	#endif
	#if defined(__GNUC__)
		#define ABYTEK_ASM_STYLE_ATT 1
	#endif
	#define ABYTEK_PLATFORM_DESKTOP 1


#elif defined(ABYTEK_PLATFORM_WINDOWS_PHONE)
	#undef ABYTEK_PLATFORM_WINDOWS_PHONE
	#define ABYTEK_PLATFORM_WINDOWS_PHONE 1
	#define ABYTEK_PLATFORM_NAME "Windows Phone"
	#if defined(_M_AMD64) || defined(_AMD64_) || defined(__x86_64__)
		#define ABYTEK_PROCESSOR_X86_64 1
		#define ABYTEK_SYSTEM_LITTLE_ENDIAN 1
		#define ABYTEK_PLATFORM_DESCRIPTION "Windows Phone on x64"
	#elif defined(_M_IX86) || defined(_X86_)
		#define ABYTEK_PROCESSOR_X86 1
		#define ABYTEK_SYSTEM_LITTLE_ENDIAN 1
		#define ABYTEK_PLATFORM_DESCRIPTION "Windows Phone on X86"
	#elif defined(_M_ARM)
		#define ABYTEK_ABI_ARM_WINCE 1
		#define ABYTEK_PROCESSOR_ARM32 1
		#define ABYTEK_SYSTEM_LITTLE_ENDIAN 1
		#define ABYTEK_PLATFORM_DESCRIPTION "Windows Phone on ARM"
	#else //Possibly other Windows Phone variants
		#error Unknown processor
		#error Unknown endianness
	#endif
	#define ABYTEK_PLATFORM_MICROSOFT 1

	// WINAPI_FAMILY defines - mirrored from winapifamily.h
	#define ABYTEK_WINAPI_FAMILY_APP         1
	#define ABYTEK_WINAPI_FAMILY_DESKTOP_APP 2
	#define ABYTEK_WINAPI_FAMILY_PHONE_APP   3

	#if defined(WINAPI_FAMILY)
		#include <winapifamily.h>
		#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
			#define ABYTEK_WINAPI_FAMILY ABYTEK_WINAPI_FAMILY_PHONE_APP
		#else
			#error Unsupported WINAPI_FAMILY for Windows Phone
		#endif
	#else
		#error WINAPI_FAMILY should always be defined on Windows Phone.
	#endif

	// Macro to determine if a partition is enabled.
	#define ABYTEK_WINAPI_FAMILY_PARTITION(Partition)   (Partition)

	// Enable the appropriate partitions for the current family
	#if ABYTEK_WINAPI_FAMILY == ABYTEK_WINAPI_FAMILY_PHONE_APP
	#   define ABYTEK_WINAPI_PARTITION_CORE    1
	#   define ABYTEK_WINAPI_PARTITION_PHONE   1
	#   define ABYTEK_WINAPI_PARTITION_APP     1
	#else
	#   error Unsupported WINAPI_FAMILY for Windows Phone
	#endif


// Windows
// _WIN32 is defined by the VC++, Intel and GCC compilers.
// _WIN64 is defined by the VC++, Intel and GCC compilers.
// __WIN32__ is defined by the Borland compiler.
// __INTEL__ is defined by the Metrowerks compiler.
// _M_IX86, _M_AMD64 and _M_IA64 are defined by the VC++, Intel, and Borland compilers.
// _X86_, _AMD64_, and _IA64_ are defined by the Metrowerks compiler.
// _M_ARM is defined by the VC++ compiler.
#elif (defined(ABYTEK_PLATFORM_WINDOWS) || (defined(_WIN32) || defined(__WIN32__) || defined(_WIN64))) && !defined(CS_UNDEFINED_STRING)
	#undef  ABYTEK_PLATFORM_WINDOWS
	#define ABYTEK_PLATFORM_WINDOWS 1
	#define ABYTEK_PLATFORM_NAME "Windows"
	#ifdef _WIN64 // VC++ defines both _WIN32 and _WIN64 when compiling for Win64.
		#define ABYTEK_PLATFORM_WIN64 1
	#else
		#define ABYTEK_PLATFORM_WIN32 1
	#endif
	#if defined(_M_AMD64) || defined(_AMD64_) || defined(__x86_64__)
		#define ABYTEK_PROCESSOR_X86_64 1
		#define ABYTEK_SYSTEM_LITTLE_ENDIAN 1
		#define ABYTEK_PLATFORM_DESCRIPTION "Windows on x64"
	#elif defined(_M_IX86) || defined(_X86_)
		#define ABYTEK_PROCESSOR_X86 1
		#define ABYTEK_SYSTEM_LITTLE_ENDIAN 1
		#define ABYTEK_PLATFORM_DESCRIPTION "Windows on X86"
	#elif defined(_M_IA64) || defined(_IA64_)
		#define ABYTEK_PROCESSOR_IA64 1
		#define ABYTEK_SYSTEM_LITTLE_ENDIAN 1
		#define ABYTEK_PLATFORM_DESCRIPTION "Windows on IA-64"
	#elif defined(_M_ARM)
		#define ABYTEK_ABI_ARM_WINCE 1
		#define ABYTEK_PROCESSOR_ARM32 1
		#define ABYTEK_SYSTEM_LITTLE_ENDIAN 1
		#define ABYTEK_PLATFORM_DESCRIPTION "Windows on ARM"
	#elif defined(_M_ARM64)
		#define ABYTEK_PROCESSOR_ARM64 1
		#define ABYTEK_SYSTEM_LITTLE_ENDIAN 1
		#define ABYTEK_PLATFORM_DESCRIPTION "Windows on ARM64"
	#else //Possibly other Windows CE variants
		#error Unknown processor
		#error Unknown endianness
	#endif
	#if defined(__GNUC__)
		#define ABYTEK_ASM_STYLE_ATT 1
	#elif defined(_MSC_VER) || defined(__BORLANDC__) || defined(__ICL)
		#define ABYTEK_ASM_STYLE_INTEL 1
	#endif
	#define ABYTEK_PLATFORM_DESKTOP 1
	#define ABYTEK_PLATFORM_MICROSOFT 1

	#if defined(_KERNEL_MODE)
		#define ABYTEK_PLATFORM_WINDOWS_KERNEL 1
	#endif

	// WINAPI_FAMILY defines to support Windows 8 Metro Apps - mirroring winapifamily.h in the Windows 8 SDK
	#define ABYTEK_WINAPI_FAMILY_APP         1000
	#define ABYTEK_WINAPI_FAMILY_DESKTOP_APP 1001
	#define ABYTEK_WINAPI_FAMILY_GAMES       1006

	#if defined(WINAPI_FAMILY)
		#if defined(_MSC_VER)
			#pragma warning(push, 0)
		#endif
		#include <winapifamily.h>
		#if defined(_MSC_VER)
			#pragma warning(pop)
		#endif
		#if defined(WINAPI_FAMILY_DESKTOP_APP) && WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
			#define ABYTEK_WINAPI_FAMILY ABYTEK_WINAPI_FAMILY_DESKTOP_APP
		#elif defined(WINAPI_FAMILY_APP) && WINAPI_FAMILY == WINAPI_FAMILY_APP
			#define ABYTEK_WINAPI_FAMILY ABYTEK_WINAPI_FAMILY_APP
		#elif defined(WINAPI_FAMILY_GAMES) && WINAPI_FAMILY == WINAPI_FAMILY_GAMES
			#define ABYTEK_WINAPI_FAMILY ABYTEK_WINAPI_FAMILY_GAMES
		#else
			#error Unsupported WINAPI_FAMILY
		#endif
	#else
		#define ABYTEK_WINAPI_FAMILY ABYTEK_WINAPI_FAMILY_DESKTOP_APP
	#endif

	#define ABYTEK_WINAPI_PARTITION_DESKTOP   1
	#define ABYTEK_WINAPI_PARTITION_APP       1
	#define ABYTEK_WINAPI_PARTITION_GAMES    (ABYTEK_WINAPI_FAMILY == ABYTEK_WINAPI_FAMILY_GAMES)

	#define ABYTEK_WINAPI_FAMILY_PARTITION(Partition)   (Partition)

	// ABYTEK_PLATFORM_WINRT
	// This is a subset of Windows which is used for tablets and the "Metro" (restricted) Windows user interface.
	// WinRT doesn't doesn't have access to the Windows "desktop" API, but WinRT can nevertheless run on
	// desktop computers in addition to tablets. The Windows Phone API is a subset of WinRT and is not included
	// in it due to it being only a part of the API.
	#if defined(__cplusplus_winrt)
		#define ABYTEK_PLATFORM_WINRT 1
	#endif

// Sun (Solaris)
// __SUNPRO_CC is defined by the Sun compiler.
// __sun is defined by the GCC compiler.
// __i386 is defined by the Sun and GCC compilers.
// __sparc is defined by the Sun and GCC compilers.
#elif defined(__EMSCRIPTEN__)
	#undef  ABYTEK_PLATFORM_EMSCRIPTEN
	#define ABYTEK_PLATFORM_LINUX 1
	#define ABYTEK_PLATFORM_UNIX 1
	#define ABYTEK_PLATFORM_POSIX 1
	#define ABYTEK_PLATFORM_NAME "Emscripten"

	#define ABYTEK_PROCESSOR_WASM

	#if defined(__wasm64__)
		#define ABYTEK_PROCESSOR_WASM_64
		#define ABYTEK_PLATFORM_DESCRIPTION "Emscripten on WASM 64"
	#elif defined(__wasm32__)
		#define ABYTEK_PROCESSOR_WASM_32
		#define ABYTEK_PLATFORM_DESCRIPTION "Emscripten on WASM 32"
	#else
		#error Unknown WASM
	#endif
	
	#if !defined(ABYTEK_SYSTEM_BIG_ENDIAN) && !defined(ABYTEK_SYSTEM_LITTLE_ENDIAN)
		#define ABYTEK_SYSTEM_LITTLE_ENDIAN 1
	#endif
#else
	#error Unknown platform
	#error Unknown processor
	#error Unknown endianness
#endif

#ifndef ABYTEK_PROCESSOR_ARM
	#if defined(ABYTEK_PROCESSOR_ARM32) || defined(ABYTEK_PROCESSOR_ARM64) || defined(ABYTEK_PROCESSOR_ARM7)
		#define ABYTEK_PROCESSOR_ARM
	#endif
#endif

// ABYTEK_PLATFORM_PTR_SIZE
// Platform pointer size; same as sizeof(void*).
// This is not the same as sizeof(int), as int is usually 32 bits on
// even 64 bit platforms.
//
// _WIN64 is defined by Win64 compilers, such as VC++.
// _M_IA64 is defined by VC++ and Intel compilers for IA64 processors.
// __LP64__ is defined by HP compilers for the LP64 standard.
// _LP64 is defined by the GCC and Sun compilers for the LP64 standard.
// __ia64__ is defined by the GCC compiler for IA64 processors.
// __arch64__ is defined by the Sparc compiler for 64 bit processors.
// __mips64__ is defined by the GCC compiler for MIPS processors.
// __powerpc64__ is defined by the GCC compiler for PowerPC processors.
// __64BIT__ is defined by the AIX compiler for 64 bit processors.
// __sizeof_ptr is defined by the ARM compiler (armcc, armcpp).
//
#ifndef ABYTEK_PLATFORM_PTR_SIZE
	#if defined(__WORDSIZE) // Defined by some variations of GCC.
		#define ABYTEK_PLATFORM_PTR_SIZE ((__WORDSIZE) / 8)
	#elif defined(_WIN64) || defined(__LP64__) || defined(_LP64) || defined(_M_IA64) || defined(__ia64__) || defined(__arch64__) || defined(__aarch64__) || defined(__mips64__) || defined(__64BIT__) || defined(__Ptr_Is_64)
		#define ABYTEK_PLATFORM_PTR_SIZE 8
	#elif defined(__CC_ARM) && (__sizeof_ptr == 8)
		#define ABYTEK_PLATFORM_PTR_SIZE 8
	#else
		#define ABYTEK_PLATFORM_PTR_SIZE 4
	#endif
#endif


// ABYTEK_PLATFORM_WORD_SIZE
// This defines the size of a machine word. This will be the same as
// the size of registers on the machine but not necessarily the same
// as the size of pointers on the machine. A number of 64 bit platforms
// have 64 bit registers but 32 bit pointers.
//
#ifndef ABYTEK_PLATFORM_WORD_SIZE
	#define ABYTEK_PLATFORM_WORD_SIZE ABYTEK_PLATFORM_PTR_SIZE
#endif

// ABYTEK_PLATFORM_MIN_MALLOC_ALIGNMENT
// This defines the minimal alignment that the platform's malloc
// implementation will return. This should be used when writing custom
// allocators to ensure that the alignment matches that of malloc
#ifndef ABYTEK_PLATFORM_MIN_MALLOC_ALIGNMENT
	#if defined(ABYTEK_PLATFORM_APPLE)
		#define ABYTEK_PLATFORM_MIN_MALLOC_ALIGNMENT 16
	#elif defined(ABYTEK_PLATFORM_ANDROID) && defined(ABYTEK_PROCESSOR_ARM)
		#define ABYTEK_PLATFORM_MIN_MALLOC_ALIGNMENT 8
	#elif defined(ABYTEK_PLATFORM_ANDROID) && defined(ABYTEK_PROCESSOR_X86_64)
		#define ABYTEK_PLATFORM_MIN_MALLOC_ALIGNMENT 8
	#else
		#define ABYTEK_PLATFORM_MIN_MALLOC_ALIGNMENT (ABYTEK_PLATFORM_PTR_SIZE * 2)
	#endif
#endif


// ABYTEK_MISALIGNED_SUPPORT_LEVEL
// Specifies if the processor can read and write built-in types that aren't
// naturally aligned.
//    0 - not supported. Likely causes an exception.
//    1 - supported but slow.
//    2 - supported and fast.
//
#ifndef ABYTEK_MISALIGNED_SUPPORT_LEVEL
	#if defined(ABYTEK_PROCESSOR_X86_64)
		#define ABYTEK_MISALIGNED_SUPPORT_LEVEL 2
	#else
		#define ABYTEK_MISALIGNED_SUPPORT_LEVEL 0
	#endif
#endif

// Macro to determine if a Windows API partition is enabled. Always false on non Microsoft platforms.
#if !defined(ABYTEK_WINAPI_FAMILY_PARTITION)
	#define ABYTEK_WINAPI_FAMILY_PARTITION(Partition) (0)
#endif


// ABYTEK_CACHE_LINE_SIZE
// Specifies the cache line size broken down by compile target.
// This the expected best guess values for the targets that we can make at compilation time.

#ifndef ABYTEK_CACHE_LINE_SIZE
	#if   defined(ABYTEK_PROCESSOR_X86)
		#define ABYTEK_CACHE_LINE_SIZE 32    // This is the minimum possible value.
	#elif defined(ABYTEK_PROCESSOR_X86_64)
		#define ABYTEK_CACHE_LINE_SIZE 64    // This is the minimum possible value
	#elif defined(ABYTEK_PROCESSOR_ARM32)
		#define ABYTEK_CACHE_LINE_SIZE 32    // This varies between implementations and is usually 32 or 64.
	#elif defined(ABYTEK_PROCESSOR_ARM64)
		#define ABYTEK_CACHE_LINE_SIZE 64    // Cache line Cortex-A8  (64 bytes) http://shervinemami.info/armAssembly.html however this remains to be mostly an assumption at this stage
	#elif (ABYTEK_PLATFORM_WORD_SIZE == 4)
		#define ABYTEK_CACHE_LINE_SIZE 32    // This is the minimum possible value
	#else
		#define ABYTEK_CACHE_LINE_SIZE 64    // This is the minimum possible value
	#endif
#endif

// Apple Vision Pro
#if defined(__APPLE__) && defined(TARGET_OS_VISION)
#define ABYTEK_PLATFORM_APPLE_VISION
#ifndef ABYTEK_PLATFORM_MOBILE
#define ABYTEK_PLATFORM_MOBILE
#endif
#ifndef ABYTEK_PLATFORM_XR
#define ABYTEK_PLATFORM_XR
#endif
#endif