// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

#ifdef _WIN32
#  ifdef FOO_LIBRARY_EXPORT
#    define FOO_LIBRARY_API __declspec(dllexport)
#  else
#    ifdef FOO_LIBRARY_STATIC
#      define FOO_LIBRARY_API
#    else
#      define FOO_LIBRARY_API __declspec(dllimport)
#    endif  // FOO_LIBRARY_STATIC
#  endif
#else
#  define FOO_LIBRARY_API
#endif

FOO_LIBRARY_API void foo_library();
