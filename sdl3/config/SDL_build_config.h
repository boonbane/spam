#ifndef SDL_build_config_h_
#define SDL_build_config_h_

#include <SDL3/SDL_platform_defines.h>

#if defined(SPN_BUILD_OS_WINDOWS)
  #include "SDL_build_config_windows.h"
  #if defined(__has_include) && !__has_include(<windows.gaming.input.h>)
    #undef HAVE_WINDOWS_GAMING_INPUT_H
    #undef SDL_JOYSTICK_WGI
  #endif
#elif defined(SPN_BUILD_OS_MACOS)
  #include "SDL_build_config_macos.h"
#elif defined(SPN_BUILD_OS_LINUX)
  #include "platform/linux.h"
  #if defined(SPN_BUILD_ABI_GNU)
    #include "platform/glibc.h"
  #elif defined(SPN_BUILD_ABI_MUSL)
    #include "platform/musl.h"
  #else
    #error "spn sdl3: unsupported libc"
  #endif
  #include "platform/backends.h"
#else
  #error "spn sdl3: unsupported platform"
#endif

#endif
