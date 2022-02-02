set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE dynamic)

if(PORT STREQUAL "protobuf")
	set(VCPKG_LIBRARY_LINKAGE static)
endif()

if(PORT STREQUAL "grpc")
	set(VCPKG_LIBRARY_LINKAGE static)
endif()