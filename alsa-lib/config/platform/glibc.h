#ifndef SPN_ALSA_GLIBC_H
#define SPN_ALSA_GLIBC_H

#include <features.h>

#define HAVE_LFS 1
#if __GLIBC_PREREQ(2, 34)
  #define HAVE_DECL_CLOSEFROM 1
#else
  #define HAVE_DECL_CLOSEFROM 0
#endif

#endif
