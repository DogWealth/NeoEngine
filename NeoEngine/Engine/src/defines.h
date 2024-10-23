#pragma once

#ifdef NEXPORT
//export
#ifdef _MSC_VER //if use MSVC compiler
#define NAPI _declspec(dllexport)
#else			//other compilers
#define NAPI __attribute__(visibility("default"))
#endif // _MSC_VER

#else
//import
#ifdef _MSC_VER
#define NAPI _declspec(dllimport)
#else
#define NAPI
#endif // _MSC_VER
#endif // NEXPORT
