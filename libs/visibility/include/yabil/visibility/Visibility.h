#pragma once

// Generic helper definitions for shared library support
// From: https://gcc.gnu.org/wiki/Visibility

#if defined _WIN32 || defined __CYGWIN__
#define YABIL_HELPER_DLL_IMPORT __declspec(dllimport)
#define YABIL_HELPER_DLL_EXPORT __declspec(dllexport)
#define YABIL_HELPER_DLL_LOCAL
#else
#if __GNUC__ >= 4
#define YABIL_HELPER_DLL_IMPORT __attribute__((visibility("default")))
#define YABIL_HELPER_DLL_EXPORT __attribute__((visibility("default")))
#define YABIL_HELPER_DLL_LOCAL __attribute__((visibility("hidden")))
#else
#define YABIL_HELPER_DLL_IMPORT
#define YABIL_HELPER_DLL_EXPORT
#define YABIL_HELPER_DLL_LOCAL
#endif
#endif

// Now we use the generic helper definitions above to define YABIL_PUBLIC and YABIL_LOCAL.
// - YABIL_PUBLIC is used for the public API symbols. It either DLL imports or DLL exports
// (or does nothing for static build)
// - YABIL_LOCAL is used for non-api symbols.

#ifdef YABIL_DLL          // defined if YABIL is compiled as a DLL
#ifdef YABIL_DLL_EXPORTS  // defined if we are building the YABIL DLL (instead of using it)
#define YABIL_PUBLIC YABIL_HELPER_DLL_EXPORT
#else
#define YABIL_PUBLIC YABIL_HELPER_DLL_IMPORT
#endif
#define YABIL_LOCAL YABIL_HELPER_DLL_LOCAL
#else  // YABIL_DLL is not defined: this means YABIL is a static lib.
#define YABIL_PUBLIC
#define YABIL_LOCAL
#endif
