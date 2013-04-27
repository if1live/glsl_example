// Ŭnicode please
#pragma once

#include "arch.h"
#include <cassert>
#include <cstdlib>

#if SR_WIN
//#define SR_ASSERT(EXPR) do { assert(EXPR); } while(0);
#define SR_DEBUG_BREAK() { __asm{ int 3 }; }
#define SR_ASSERT(EXPR) if(!(EXPR)) { SR_DEBUG_BREAK(); }

#elif SR_IOS
#define SR_ASSERT(EXPR) do { assert(EXPR); } while(0);

#elif SR_ANDROID
#include <android/log.h>
#define SR_ASSERT(EXPR) do { \
  if (!(EXPR)) {  \
    __android_log_print(ANDROID_LOG_FATAL, "ASSERT", "%s : %d : %s", __FILE__, __LINE__, #EXPR); \
    assert(EXPR); \
    exit(-1); \
  } } while(0);

#else
#error "not support"
#endif

//static_assert
#if SR_WIN || SR_IOS
#define SR_STATIC_ASSERT(A, B) static_assert(A, B)
#define SR_STATIC_ASSERT_NOMSG(A) static_assert(A, "no msg")
#else
#define SR_STATIC_ASSERT(A, B)
#define SR_STATIC_ASSERT_NOMSG(A)
#endif
