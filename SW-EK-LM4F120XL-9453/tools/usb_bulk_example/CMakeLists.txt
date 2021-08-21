cmake_minimum_required(VERSION 3.15)

project(usb)


find_package(PkgConfig REQUIRED)
include(FindPkgConfig)

message(STATUS "--- PKG_CONFIG_FOUND: ${PKG_CONFIG_FOUND}")
message(STATUS "--- PKG_CONFIG_VERSION_STRING: ${PKG_CONFIG_VERSION_STRING}")


pkg_check_modules(PKGS_DEP REQUIRED  libusb-1.0)

Pkg_search_module(PKG_MYLIB libusb-1.0)

message(STATUS "--- INC: ${PKGS_DEP_CFLAGS}")
message(STATUS "--- LIB: ${PKGS_DEP_LDFLAGS}")


foreach(flag ${PKGS_DEP_CFLAGS})
	set(EXTRA_CFLAGS "${EXTRA_CFLAGS} ${flag}")
endforeach(flag)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CFLAGS}")

add_executable(usb_bulk_example usb_bulk_example.c)
add_executable(usb_bulk_loop usb_bulk_loop.c)

target_link_libraries(usb_bulk_example ${PKGS_DEP_LDFLAGS})
target_link_libraries(usb_bulk_loop ${PKGS_DEP_LDFLAGS})