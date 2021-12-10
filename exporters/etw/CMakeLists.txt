find_package(nlohmann_json REQUIRED)

add_library(opentelemetry_exporter_etw INTERFACE)

target_include_directories(
  opentelemetry_exporter_etw
  INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
            "$<INSTALL_INTERFACE:include>")

set_target_properties(opentelemetry_exporter_etw PROPERTIES EXPORT_NAME
                                                            etw_exporter)

target_link_libraries(opentelemetry_exporter_etw
                      INTERFACE opentelemetry_trace nlohmann_json::nlohmann_json)
                 
target_compile_definitions(opentelemetry_exporter_etw INTERFACE OTEL_EXPORT)

install(
  TARGETS opentelemetry_exporter_etw
  EXPORT "${PROJECT_NAME}-target"
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(
  DIRECTORY include/opentelemetry/exporters/etw
  DESTINATION include/opentelemetry/exporters/
  FILES_MATCHING
  PATTERN "*.h")

if(BUILD_TESTING)
  add_executable(etw_provider_test test/etw_provider_test.cc)
  add_executable(etw_tracer_test test/etw_tracer_test.cc)
  add_executable(etw_perf_test test/etw_perf_test.cc)

  target_link_libraries(etw_provider_test ${GTEST_BOTH_LIBRARIES}
                        opentelemetry_exporter_etw ${CMAKE_THREAD_LIBS_INIT})

  target_link_libraries(etw_tracer_test ${GTEST_BOTH_LIBRARIES}
                        opentelemetry_exporter_etw ${CMAKE_THREAD_LIBS_INIT})

  target_link_libraries(
    etw_perf_test benchmark::benchmark ${GTEST_BOTH_LIBRARIES}
    opentelemetry_exporter_etw ${CMAKE_THREAD_LIBS_INIT})
                 
  target_compile_definitions(etw_provider_test PUBLIC OTEL_EXPORT)
          
  target_compile_definitions(etw_tracer_test PUBLIC OTEL_EXPORT)
          
  target_compile_definitions(etw_perf_test PUBLIC OTEL_EXPORT)

  gtest_add_tests(
    TARGET etw_provider_test
    TEST_PREFIX exporter.
    TEST_LIST etw_provider_test)
  gtest_add_tests(
    TARGET etw_tracer_test
    TEST_PREFIX exporter.
    TEST_LIST etw_tracer_test)

endif() # BUILD_TESTING
