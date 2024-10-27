//
// Created by NeoLiu on 2024/10/27.
//

#ifndef ASSERT_H
#define ASSERT_H

#define NEO_ASSERTIONS_ENABLED

#ifdef NEO_ASSERTIONS_ENABLED
#if _MSC_VER
#include <intrin.h>
#define NeoDebugBreak() __debugbreak()
#else
#define NeoDebugBreak() __builtin_trap()
#endif

//if inline function, we don't have to use NEO_API. otherwise need to use NEO_API to export to the dll
inline void ReportAssertionFailure(const char* expr, const char* message, const char* file, const int line) {
    if(message == "") {
        NEO_FATAL("Assertion failure: %s, in file: %s, line: %d\n", expr, file, line);
    } else {
        NEO_FATAL("Assertion failure: %s, message: %s, in file: %s, line: %d\n", expr, message, file, line);
    }
}

#define NEO_ASSERT(expr)                                        \
{                                                               \
    if(expr) {                                                  \
    } else {                                                    \
        ReportAssertionFailure(#expr, "", __FILE__, __LINE__);   \
        NeoDebugBreak();                                           \
    }                                                           \
}

#define NEO_ASSERT_MSG(expr, message)                               \
{                                                                   \
    if(expr) {                                                      \
    } else {                                                        \
        ReportAssertionFailure(#expr, message, __FILE__, __LINE__);  \
        NeoDebugBreak();                                               \
    }                                                               \
}

#ifdef NEO_DEBUG
#define NEO_ASSERT_DEBUG(expr)                                        \
{                                                                     \
    if(expr) {                                                        \
    } else {                                                          \
        ReportAssertionFailure(#expr, "", __FILE__, __LINE__);         \
        NeoDebugBreak();                                                 \
    }                                                                 \
}
#else
#define NEO_ASSERT_DEBUG(expr)
#endif

#else

#define NEO_ASSERT(expr)
#define NEO_ASSERT_MSG(expr, message)
#define NEO_ASSERT_DEBUG(expr)

#endif
#endif //ASSERT_H
