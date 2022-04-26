if(WITH_HEADER_ONLY_API)
    add_library(opentelemetry_api INTERFACE)
    target_compile_definitions(opentelemetry_api INTERFACE OTEL_WITH_HEADER_ONLY_API)

    if(MSVC)
    if(BUILD_SHARED_LIBS)
      message(
        FATAL_ERROR
          "Building header-only API combined with dynamic link libraries is not supported. Turn off option WITH_HEADER_ONLY_API to build the API as a dynamic link library as well, or turn off BUILD_SHARED_LIBS."
      )
      endif()
    endif()
else()
    add_library(opentelemetry_api 
        include/opentelemetry/baggage/baggage.cc 
        include/opentelemetry/baggage/baggage_context.cc 
        include/opentelemetry/common/kv_properties.cc 
        include/opentelemetry/context/propagation/global_propagator.cc
        include/opentelemetry/context/context.cc 
        include/opentelemetry/context/runtime_context.cc 
        include/opentelemetry/metrics/provider.cc 
        include/opentelemetry/trace/default_span.cc 
        include/opentelemetry/trace/noop.cc 
        include/opentelemetry/trace/provider.cc 
        include/opentelemetry/trace/scope.cc 
        include/opentelemetry/trace/span.cc 
        include/opentelemetry/trace/trace_state.cc
        include/opentelemetry/trace/tracer.cc
        include/opentelemetry/trace/tracer_provider.cc)

    target_compile_definitions(opentelemetry_api PUBLIC OTEL_EXPORT)
endif()

target_include_directories(
  opentelemetry_api
  INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
            "$<INSTALL_INTERFACE:include>")

set_target_properties(opentelemetry_api PROPERTIES EXPORT_NAME api)

install(
  TARGETS opentelemetry_api
  EXPORT "${PROJECT_NAME}-target"
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
  
install(
  FILES 
  include/opentelemetry/config.h 
  include/opentelemetry/export.h 
  include/opentelemetry/version.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/opentelemetry)


install(
      DIRECTORY 
      include/opentelemetry/baggage 
      include/opentelemetry/common 
      include/opentelemetry/context 
      include/opentelemetry/detail 
      include/opentelemetry/nostd 
      include/opentelemetry/plugin 
      include/opentelemetry/std 
      include/opentelemetry/trace
      DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/opentelemetry
      FILES_MATCHING 
      PATTERN "*.h")

if(WITH_HEADER_ONLY_API)
    install(
      DIRECTORY 
      include/opentelemetry/baggage 
      include/opentelemetry/common 
      include/opentelemetry/context 
      include/opentelemetry/detail 
      include/opentelemetry/metrics 
      include/opentelemetry/nostd 
      include/opentelemetry/plugin 
      include/opentelemetry/std 
      include/opentelemetry/trace
      DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/opentelemetry
      FILES_MATCHING
      PATTERN "*.cc")
 endif()

if(WITH_METRICS_PREVIEW)
    target_compile_definitions(opentelemetry_api INTERFACE ENABLE_METRICS_PREVIEW)
    install(
      DIRECTORY include/opentelemetry/_metrics
      DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/opentelemetry
      FILES_MATCHING PATTERN "*.h")
endif()

if(WITH_LOGS_PREVIEW)
  target_compile_definitions(opentelemetry_api INTERFACE ENABLE_LOGS_PREVIEW)
    install(
      DIRECTORY include/opentelemetry/logs
      DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/opentelemetry
      FILES_MATCHING PATTERN "*.h")
endif()

if(BUILD_TESTING)
  add_subdirectory(test)
endif()

if(WITH_ABSEIL)
  target_compile_definitions(opentelemetry_api INTERFACE HAVE_ABSEIL)
endif()

if(WITH_STL)
  message("Building with standard library types...")
  target_compile_definitions(opentelemetry_api INTERFACE HAVE_CPP_STDLIB
                                                         HAVE_GSL)
else()
  message("Building with nostd types...")
endif()

if(WITH_METRICS_PREVIEW)
  target_compile_definitions(opentelemetry_api INTERFACE ENABLE_METRICS_PREVIEW)
endif()

if(WITH_LOGS_PREVIEW)
  target_compile_definitions(opentelemetry_api INTERFACE ENABLE_LOGS_PREVIEW)
endif()

if(CORE_RUNTIME_LIBS)
  target_link_libraries(opentelemetry_api INTERFACE ${CORE_RUNTIME_LIBS})
endif()