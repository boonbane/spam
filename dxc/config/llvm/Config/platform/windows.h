#ifndef SPN_DXC_WINDOWS_H
#define SPN_DXC_WINDOWS_H

#define HAVE_DECL_STRERROR_S 1
#define HAVE_MALLOC_H 1
#define HAVE___CHKSTK 1
#define LLVM_HOST_TRIPLE "x86_64-pc-win32"
#define LLVM_ON_WIN32 1
#define LTDL_SHLIB_EXT ".dll"
#define WIN32_ELMCB_PCSTR PCSTR
#define stricmp _stricmp
#define strdup _strdup
#define HAVE__CHSIZE_S 1

#endif
