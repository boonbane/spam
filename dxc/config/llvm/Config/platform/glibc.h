#ifndef SPN_DXC_GLIBC_H
#define SPN_DXC_GLIBC_H

#include <features.h>

#define ENABLE_BACKTRACES 1
#define HAVE_BACKTRACE 1
#define HAVE_EXECINFO_H 1
#define HAVE_MALLINFO 1
#if __GLIBC_PREREQ(2, 36)
  #define HAVE_DECL_ARC4RANDOM 1
#endif
#if __GLIBC_PREREQ(2, 33)
  #define HAVE_MALLINFO2 1
#endif

#endif
