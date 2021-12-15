#pragma once

#ifdef _WIN32
#  ifdef OTEL_SDK_EXPORT
#    define OTEL_SDK_API __declspec(dllexport)
#  else
#    define OTEL_SDK_API __declspec(dllimport)
#  endif
#else
#  define OTEL_SDK_API
#endif
