#ifndef SPN_SDL3_GLIBC_H
#define SPN_SDL3_GLIBC_H

#include <features.h>

#if __GLIBC_PREREQ(2, 38)
  #define HAVE_STRLCPY 1
  #define HAVE_STRLCAT 1
  #define HAVE_WCSLCPY 1
  #define HAVE_WCSLCAT 1
#endif

#endif
