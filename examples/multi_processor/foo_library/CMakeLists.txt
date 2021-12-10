add_library(foo_multi_library foo_library.cc)
target_link_libraries(foo_multi_library ${CMAKE_THREAD_LIBS_INIT} opentelemetry_api)
                  
target_compile_definitions(foo_multi_library PUBLIC FOO_LIBRARY_EXPORT)

#target_include_directories(foo_multi_library PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/examples/multi_processor>")
target_include_directories(foo_multi_library PUBLIC ${CMAKE_CURRENT_LIST_DIR})
