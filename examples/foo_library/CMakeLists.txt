add_library(foo_library foo_library.cc)
target_link_libraries(foo_library ${CMAKE_THREAD_LIBS_INIT} opentelemetry_api)
     
if(BUILD_SHARED_LIBS)
    target_compile_definitions(foo_library PRIVATE FOO_LIBRARY_EXPORT)
else()
    target_compile_definitions(foo_library PUBLIC FOO_LIBRARY_STATIC)
endif()

target_include_directories(foo_library PUBLIC ${CMAKE_CURRENT_LIST_DIR})