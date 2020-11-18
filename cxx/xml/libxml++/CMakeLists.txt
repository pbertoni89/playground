cmake_minimum_required(VERSION 3.10)
project(sandbox)

set(CMAKE_CXX_STANDARD 14)

find_package(LibXml2 REQUIRED)
find_package (PkgConfig REQUIRED)
pkg_check_modules (LIBXMLXX REQUIRED IMPORTED_TARGET libxml++-2.6)

include_directories (${LIBXML2_INCLUDE_DIR})
include_directories ("/usr/include/libxml++2.6/")

add_executable(sandbox main.cpp)

target_link_libraries(sandbox ${LIBXML2_LIBRARIES})
target_link_libraries(sandbox PkgConfig::LIBXMLXX)
