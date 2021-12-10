#pragma once

#ifdef _WIN32
#  ifdef OTEL_EXPORT
#    define OTEL_API __declspec(dllexport)
#  else
#    define OTEL_API __declspec(dllimport)
#  endif
#else
#  define OTEL_API
#endif