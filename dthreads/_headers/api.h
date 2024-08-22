// ***************************************************************************************
//    Project: dthreads -> https://github.com/dezashibi-c/dthreads
//    File: api.h
//    Date: 2024-08-16
//    Author: Navid Dezashibi
//    Contact: navid@dezashibi.com
//    Website: https://www.dezashibi.com | https://github.com/dezashibi
//    License:
//     Please refer to the LICENSE file, repository or website for more information about
//     the licensing of this work. If you have any questions or concerns,
//     please feel free to contact me at the email address provided above.
// ***************************************************************************************
// *  Description: Declaration of DTHREAD_API for dynamically or statically linking
// ***************************************************************************************

#ifndef DTHREAD_API_

#define DTHREAD_API_

#include <stdint.h>
#include <string.h>

#include <limits.h>
#include <stdint.h>

#if UINT_MAX == 0xFFFFFFFFU
typedef uint32_t dthread_uint_t;
#elif UINT_MAX == 0xFFFFU
typedef uint16_t dthread_uint_t;
#elif UINT_MAX == 0xFFFFFFFFFFFFFFFFU
typedef uint64_t dthread_uint_t;
#else
#error "Unsupported platform for unsigned int"
#endif

#if INT_MAX == 0x7FFFFFFF
typedef int32_t dthread_int_t;
#elif INT_MAX == 0x7FFF
typedef int16_t dthread_int_t;
#elif INT_MAX == 0x7FFFFFFFFFFFFFFF
typedef int64_t dthread_int_t;
#else
#error "Unsupported platform for int"
#endif

#if ULONG_MAX == 0xFFFFFFFFUL
typedef uint32_t dthread_ulong_t;
#elif ULONG_MAX == 0xFFFFFFFFFFFFFFFFUL
typedef uint64_t dthread_ulong_t;
#else
#error "Unsupported platform for unsigned long"
#endif

#if LONG_MAX == 0x7FFFFFFF
typedef int32_t dthread_long_t;
#elif LONG_MAX == 0x7FFFFFFFFFFFFFFF
typedef int64_t dthread_long_t;
#else
#error "Unsupported platform for long"
#endif

#if ULLONG_MAX == 0xFFFFFFFFFFFFFFFFULL
typedef uint64_t dthread_ullong_t;
#else
#error "Unsupported platform for unsigned long long"
#endif

#if LLONG_MAX == 0x7FFFFFFFFFFFFFFFLL
typedef int64_t dthread_llong_t;
#else
#error "Unsupported platform for long long"
#endif

#ifdef _MSC_VER // MSVC
#define DTHREAD_API_EXPORT __declspec(dllexport)
#define DTHREAD_API_IMPORT __declspec(dllimport)
#elif defined(__GNUC__) // GCC, Clang
#define DTHREAD_API_EXPORT __attribute__((visibility("default")))
#define DTHREAD_API_IMPORT __attribute__((visibility("default")))
#else
#define DTHREAD_API_EXPORT
#define DTHREAD_API_IMPORT
#endif

#ifdef DTHREAD_DLL_EXPORTS
#define DTHREAD_API DTHREAD_API_EXPORT
#else
#define DTHREAD_API DTHREAD_API_IMPORT
#endif

#endif // DTHREAD_API_
