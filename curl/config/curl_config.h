#ifndef SPN_CURL_CONFIG_H
#define SPN_CURL_CONFIG_H

#if defined(SPN_BUILD_OS_WINDOWS)
  #include "config-win32.h"
  #if defined(SPN_BUILD_ABI_GNU)
    #include "platform/mingw.h"
  #endif
#elif defined(SPN_BUILD_OS_MACOS)
  #include "platform/posix.h"
  #include "platform/macos.h"
#elif defined(SPN_BUILD_OS_LINUX)
  #include "platform/posix.h"
  #include "platform/linux.h"
  #if defined(SPN_BUILD_ABI_GNU)
    #include "platform/glibc.h"
  #elif defined(SPN_BUILD_ABI_MUSL)
    #include "platform/musl.h"
  #else
    #error "spn curl: unsupported libc"
  #endif
#else
  #error "spn curl: unsupported platform"
#endif

#endif
