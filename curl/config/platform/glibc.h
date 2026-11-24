#ifndef SPN_CURL_GLIBC_H
#define SPN_CURL_GLIBC_H

#include <features.h>

#define SIZEOF_TIME_T __SIZEOF_LONG__

#define HAVE_GLIBC_STRERROR_R 1
#define HAVE_LINUX_TCP_H 1
#if __GLIBC_PREREQ(2, 36)
  #define HAVE_ARC4RANDOM 1
#endif

#endif
