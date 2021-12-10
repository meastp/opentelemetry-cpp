add_executable(example_simple main.cc)
target_link_libraries(example_simple ${CMAKE_THREAD_LIBS_INIT} foo_library
                      opentelemetry_trace opentelemetry_exporter_ostream_span)
