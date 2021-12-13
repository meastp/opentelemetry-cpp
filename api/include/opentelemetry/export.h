#pragma once

#ifdef _WIN32
#  ifdef OTEL_EXPORT
#    define OTEL_API __declspec(dllexport)
#  else
#    ifdef OTEL_WITH_HEADER_ONLY_API
#      define OTEL_API
#    else
#      define OTEL_API __declspec(dllimport)
#    endif  // OTEL_WITH_HEADER_ONLY_API
#  endif
#else
#  define OTEL_API
#endif

#ifdef OTEL_WITH_HEADER_ONLY_API
#  define OTEL_HEADER_ONLY_API_INLINE inline
#else
#  define OTEL_HEADER_ONLY_API_INLINE
#endif  // OTEL_WITH_HEADER_ONLY_API
