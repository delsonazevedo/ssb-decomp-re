#ifndef __STDDEF_H__
#define __STDDEF_H__

#include <PR/ultratypes.h>

#ifdef _MSC_VER
/* MSVC: offsetof is a compiler intrinsic */
#ifndef offsetof
#define offsetof(type, member) ((size_t)&(((type*)0)->member))
#endif
#elif !defined(__sgi)
/* GCC/Clang: use built-in offsetof macro */
#define offsetof(type, member) __builtin_offsetof(type, member)
#else
/* IDO: use macro from Indy headers */
#define offsetof(s, m) (size_t)(&(((s*)0)->m))
#endif

/* ptrdiff_t. Decomp source never uses it, but this shim shadows the real
 * <stddef.h> on the include path, so any system header pulled in by a port
 * TU that transitively references ptrdiff_t breaks if we don't supply it.
 * NDK r29 / bionic <unistd.h> surfaces this; macOS/glibc happen to declare
 * it elsewhere on the include chain, which masked the issue on desktop.
 * Provided via the compiler's built-in __PTRDIFF_TYPE__ macro (GCC/Clang). */
#if (defined(__GNUC__) || defined(__clang__)) && !defined(_PTRDIFF_T_DEFINED_)
typedef __PTRDIFF_TYPE__ ptrdiff_t;
#define _PTRDIFF_T_DEFINED_
#endif

/* wint_t. Same story as ptrdiff_t above, but the offender is devkitPro's
 * newlib (Switch): <sys/_types.h> does `#define __need_wint_t` then
 * `#include <stddef.h>`, expecting the compiler's stddef to define wint_t for
 * it. Our shim shadows that real stddef and has an include guard, so the
 * second (__need_wint_t) include is a no-op and wint_t never appears —
 * newlib's _mbstate_t then fails with "unknown type name 'wint_t'". Define it
 * up front from the compiler builtin. Harmless on glibc/bionic (identical
 * typedef); _WINT_T also stops newlib's real stddef path from re-typedef'ing. */
#if (defined(__GNUC__) || defined(__clang__)) && !defined(_WINT_T) && !defined(_WINT_T_DEFINED_)
typedef __WINT_TYPE__ wint_t;
#define _WINT_T
#define _WINT_T_DEFINED_
#endif

#endif /* __STDDEF_H__ */
