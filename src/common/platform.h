//
// Copyright (c) 2014 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

// platform.h: Operating system specific includes and defines.

#ifndef COMMON_PLATFORM_H_
#define COMMON_PLATFORM_H_

#if defined(_WIN32) || defined(_WIN64)
#   define ANGLE_PLATFORM_WINDOWS 1
#elif defined(__APPLE__)
#   define ANGLE_PLATFORM_APPLE 1
#   define ANGLE_PLATFORM_POSIX 1
#elif defined(__linux__)
#   define ANGLE_PLATFORM_LINUX 1
#   define ANGLE_PLATFORM_POSIX 1
#elif defined(ANDROID)
#   define ANGLE_PLATFORM_ANDROID 1
#   define ANGLE_PLATFORM_POSIX 1
#elif defined(__FreeBSD__) || \
      defined(__OpenBSD__) || \
      defined(__NetBSD__) || \
      defined(__DragonFly__) || \
      defined(__sun) || \
      defined(__GLIBC__) || \
      defined(__GNU__) || \
      defined(__QNX__)
#   define ANGLE_PLATFORM_POSIX 1
#else
#   error Unsupported platform.
#endif

#if defined(ANGLE_PLATFORM_WINDOWS)
#   ifndef STRICT
#       define STRICT 1
#   endif
#   ifndef WIN32_LEAN_AND_MEAN
#       define WIN32_LEAN_AND_MEAN 1
#   endif
#   ifndef NOMINMAX
#       define NOMINMAX 1
#   endif

#   include <windows.h>
#   include <intrin.h>
#   if !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
#       define ANGLE_PLATFORM_WINRT 1
#       include "third_party/ThreadEmulation/ThreadEmulation.h"
        using namespace ThreadEmulation;
#       if NTDDI_VERSION < NTDDI_WINBLUE && WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_PHONE)
#           define ANGLE_PLATFORM_WP8 1
#       endif
#   endif

#   if defined(ANGLE_ENABLE_D3D9) || defined(ANGLE_ENABLE_PERF)
#       include <d3d9.h>
#       include <d3dcompiler.h>
#   endif

#   if defined(ANGLE_ENABLE_D3D11)
#       if !defined(ANGLE_PLATFORM_WINRT)
#           include <d3d10_1.h>
#           include <d3d11.h>
#           include <dxgi.h>
#       else
#           include <d3d11_1.h>
#       endif
#       include <dxgi1_2.h>
#       if !defined(ANGLE_PLATFORM_WP8)
#           include <d3dcompiler.h>
#       endif
#   endif

#   undef near
#   undef far
#endif

#endif // COMMON_PLATFORM_H_
