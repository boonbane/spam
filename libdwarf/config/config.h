#ifndef SPN_LIBDWARF_CONFIG_H
#define SPN_LIBDWARF_CONFIG_H

#define PACKAGE_VERSION "2.3.4"

#define HAVE_STDINT_H 1
#define HAVE_STDLIB_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_FCNTL_H 1

#if defined(SPN_BUILD_OS_WINDOWS)
  #define HAVE_MALLOC_H 1
  #if defined(SPN_BUILD_ABI_GNU)
    #define HAVE_UNISTD_H 1
  #endif
#else
  #define HAVE_UNISTD_H 1
  #define HAVE_FULL_MMAP 1
#endif

#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  #define WORDS_BIGENDIAN 1
#endif

#endif
