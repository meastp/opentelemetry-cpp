add_subdirectory(foo_library)

add_executable(example_multi_processor main.cc)
target_link_libraries(example_multi_processor ${CMAKE_THREAD_LIBS_INIT} foo_multi_library
                      opentelemetry_trace opentelemetry_exporter_ostream_span opentelemetry_exporter_in_memory)