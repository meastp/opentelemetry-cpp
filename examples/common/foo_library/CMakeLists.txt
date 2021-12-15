add_library(common_foo_library foo_library.cc)
target_link_libraries(common_foo_library PUBLIC ${CMAKE_THREAD_LIBS_INIT} opentelemetry_api)
     
if(BUILD_SHARED_LIBS)
    target_compile_definitions(common_foo_library PUBLIC FOO_LIBRARY_EXPORT)
else()
    target_compile_definitions(common_foo_library PUBLIC FOO_LIBRARY_STATIC)
endif()

target_include_directories(common_foo_library PUBLIC ${CMAKE_CURRENT_LIST_DIR})