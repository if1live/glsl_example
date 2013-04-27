// Ŭnicode please 
#pragma once

// platform detect code
#undef SR_IOS
#undef SR_WIN
#undef SR_ANDROID
#define SR_IOS 0
#define SR_WIN 0
#define SR_ANDROID 0

// platform check macro
#if __APPLE__
// iphone check
// 이것은 가장 밑바닥에 인클루드될 헤더이므로
// 다른 헤더를 include하지 않는다.
// #import <Availability.h>
// #if __IPHONE_2_0
#undef SR_IOS
#define SR_IOS 1
// #endif
#endif

// android check
#ifdef __ANDROID__
#undef SR_ANDROID
#define SR_ANDROID 1
#endif

#if (defined _WIN32) || (defined _W64)
#undef SR_WIN
#define SR_WIN 1
#endif

#if SR_IOS == 1 || SR_WIN == 1
#define SR_USE_PCH 1
#else
#define SR_USE_PCH 0
#endif

// platform define
#define SR_ARCH_IOS 0x01
#define SR_ARCH_ANDROID 0x02
#define SR_ARCH_WIN 0x03

#if SR_IOS
#define SR_CURR_ARCH SR_ARCH_IOS
#elif SR_ANDROID
#define SR_CURR_ARCH SR_ARCH_ANDROID
#elif SR_WIN
#define SR_CURR_ARCH SR_ARCH_WIN
#else
#error "not valid architecture"
#endif
