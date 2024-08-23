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
