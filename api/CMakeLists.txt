add_library(opentelemetry_api src/baggage/propagation/baggage_propagator.cc src/baggage/baggage.cc src/common/kv_properties.cc src/context/propagation/global_propagator.cc
src/context/runtime_context.cc src/trace/noop.cc src/trace/provider.cc src/trace/trace_state.cc)

target_include_directories(
  opentelemetry_api
  PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
            "$<INSTALL_INTERFACE:include>")

set_target_properties(opentelemetry_api PROPERTIES EXPORT_NAME api)

install(
  TARGETS opentelemetry_api
  EXPORT "${PROJECT_NAME}-target"
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

set(LOGS_EXCLUDE_PATTERN "")
if(NOT WITH_LOGS_PREVIEW)
  set(LOGS_EXCLUDE_PATTERN "logs")
endif()

set(METRICS_EXCLUDE_PATTERN "")
if(NOT WITH_METRICS_PREVIEW)
  set(METRICS_EXCLUDE_PATTERN "metrics")
endif()

install(
  DIRECTORY include/opentelemetry
  DESTINATION include
  FILES_MATCHING
  PATTERN "*.h"
  PATTERN "${METRICS_EXCLUDE_PATTERN}" EXCLUDE)

install(
  DIRECTORY include/opentelemetry
  DESTINATION include
  FILES_MATCHING
  PATTERN "*.h"
  PATTERN "${LOGS_EXCLUDE_PATTERN}" EXCLUDE)

if(BUILD_TESTING)
  add_subdirectory(test)
endif()

target_compile_definitions(opentelemetry_api PUBLIC OTEL_EXPORT)

if(WITH_ABSEIL)
  target_compile_definitions(opentelemetry_api INTERFACE HAVE_ABSEIL)
endif()

if(WITH_STL)
  message("Building with standard library types...")
  target_compile_definitions(opentelemetry_api PUBLIC HAVE_CPP_STDLIB
                                                         HAVE_GSL)
else()
  message("Building with nostd types...")
endif()

if(CORE_RUNTIME_LIBS)
  target_link_libraries(opentelemetry_api PUBLIC ${CORE_RUNTIME_LIBS})
endif()
