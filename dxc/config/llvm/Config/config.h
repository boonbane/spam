#ifndef CONFIG_H
#define CONFIG_H 1

#define BUG_REPORT_URL "http://llvm.org/bugs/"
#define ENABLE_CRASH_OVERRIDES 1
#define DISABLE_LLVM_DYLIB_ATEXIT 1
#define ENABLE_PIC 1
#define ENABLE_TIMESTAMPS 1
#define HAVE_ERRNO_H 1
#define HAVE_FCNTL_H 1
#define HAVE_FENV_H 1
#define HAVE_INT64_T 1
#define HAVE_INTTYPES_H 1
#define HAVE_LIMITS_H 1
#define HAVE_LINK_EXPORT_DYNAMIC 1
#define HAVE_SIGNAL_H 1
#define HAVE_STDINT_H 1
#define HAVE_STRERROR 1
#define HAVE_STRTOLL 1
#define HAVE_SYS_STAT_H 1
#define HAVE_UINT64_T 1
#define LLVM_DEFAULT_TARGET_TRIPLE "dxil-ms-dx"
#define LLVM_ENABLE_THREADS 1
#define LLVM_ENABLE_ZLIB 0
#define LLVM_HAS_ATOMICS 1
#define LLVM_NATIVE_ARCH X86
#define LLVM_PREFIX "/usr/local"
#define LLVM_VERSION_MAJOR 3
#define LLVM_VERSION_MINOR 7
#define LLVM_VERSION_PATCH 0
#define LLVM_VERSION_STRING "3.7.0"
#define PACKAGE_BUGREPORT "http://llvm.org/bugs/"
#define PACKAGE_NAME "LLVM"
#define PACKAGE_STRING "LLVM 3.7.0"
#define PACKAGE_VERSION "3.7.0"
#define RETSIGTYPE void

#if defined(SPN_BUILD_OS_WINDOWS)
  #include "platform/windows.h"
#elif defined(SPN_BUILD_OS_MACOS)
  #include "platform/posix.h"
  #include "platform/macos.h"
#elif defined(SPN_BUILD_OS_LINUX)
  #include "platform/posix.h"
  #include "platform/linux.h"
  #if defined(SPN_BUILD_ABI_GNU)
    #include "platform/glibc.h"
  #endif
#else
  #error "spn dxc: unsupported platform"
#endif

#endif
