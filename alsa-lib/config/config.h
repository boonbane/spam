#ifndef SPN_ALSA_CONFIG_H
#define SPN_ALSA_CONFIG_H

#ifndef _GNU_SOURCE
  #define _GNU_SOURCE 1
#endif

#define ALOAD_DEVICE_DIRECTORY "/dev/"
#define ALSA_CONFIG_DIR "/usr/share/alsa"
#define ALSA_DEVICE_DIRECTORY "/dev/snd/"
#define ALSA_PKGCONF_DIR "/usr/lib/pkgconfig"
#define ALSA_PLUGIN_DIR "/usr/lib/alsa-lib"
#define BUILD_HWDEP "1"
#define BUILD_MIXER "1"
#define BUILD_PCM "1"
#define BUILD_PCM_PLUGIN_ADPCM "1"
#define BUILD_PCM_PLUGIN_ALAW "1"
#define BUILD_PCM_PLUGIN_IEC958 "1"
#define BUILD_PCM_PLUGIN_LFLOAT "1"
#define BUILD_PCM_PLUGIN_MMAP_EMUL "1"
#define BUILD_PCM_PLUGIN_MULAW "1"
#define BUILD_PCM_PLUGIN_RATE "1"
#define BUILD_PCM_PLUGIN_ROUTE "1"
#define BUILD_RAWMIDI "1"
#define BUILD_SEQ "1"
#define BUILD_TOPOLOGY "1"
#define BUILD_UCM "1"
#define LT_OBJDIR ".libs/"
#define PACKAGE "alsa-lib"
#define PACKAGE_BUGREPORT ""
#define PACKAGE_NAME "alsa-lib"
#define PACKAGE_STRING "alsa-lib 1.2.15.3"
#define PACKAGE_TARNAME "alsa-lib"
#define PACKAGE_URL ""
#define PACKAGE_VERSION "1.2.15.3"
#define SND_MAX_CARDS 32
#define SUPPORT_ALOAD "1"
#define THREAD_SAFE_API "1"
#define TMPDIR "/tmp"
#define VERSION "1.2.15.3"
#define __SYMBOL_PREFIX ""

#include "platform/linux.h"
#if defined(SPN_BUILD_ABI_GNU)
  #include "platform/glibc.h"
#elif defined(SPN_BUILD_ABI_MUSL)
  #include "platform/musl.h"
#else
  #error "spn alsa-lib: unsupported libc"
#endif

#endif
