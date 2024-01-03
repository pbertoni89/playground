# Authors: Patrizio Bertoni pbertoni@x-next.com, Nevio Rebesco nrebesco@x-next.com
# Copyright: Xnext S.p.A. 2021
#
# Best Resources:
# https://stackoverflow.com/questions/43772887/cmake-creating-a-static-library
# https://stackoverflow.com/questions/3201154/automatically-add-all-files-in-a-folder-to-a-target-using-cmake
#

cmake_minimum_required (VERSION 3.10.2)

# We used to set a custom toolchain with `update-alternatives --config gcc`,
# but such system-wide decisions are dangerous. These flags have to be set before project()
# this is useless now, because we're going to discontinue build processes for Debian9 with g++-6 ABI
# DONE test the deprecation of strict setup, in facts Ubuntu 21 carries g++ 11.2
# This PERHAPS was a culprit for IPU strange behaviour inside quantile computation ? No, it was a configuration issue
# In facts these constraints won't help for future jumps to C++20
# set (CMAKE_C_COMPILER "/usr/bin/gcc-10")
# set (CMAKE_CXX_COMPILER "/usr/bin/g++-10")

project (playground)

if (WIN32)
	message (FATAL_ERROR "sorry, we don't do that here")
else ()
	string (ASCII 27 Esc)
	# https://stackoverflow.com/questions/18968979 - FIXME broken colours?
	set (ColourReset "${Esc}[m")
	set (Red         "${Esc}[31m")
	set (Green       "${Esc}[32m")
	set (Yellow      "${Esc}[33m")
	set (Blue        "${Esc}[34m")
	set (Magenta     "${Esc}[35m")
endif ()

set (CMAKE_C_STANDARD   11)
set (CMAKE_CXX_STANDARD 17)  # TODO consider moving to c++20 standard

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

#  ~  ~  ~  ~  ~  ~  ~  ~  ~ FIND Packages  ~  ~  ~  ~  ~  ~  ~  ~  ~

# Resources: https://stackoverflow.com/questions/1620918/cmake-and-libpthread
set (THREADS_PREFER_PTHREAD_FLAG ON)
find_package (Threads REQUIRED)

# TODO make curl non-required. If Not Found, disable xspectra-email compilation and email header
find_package (CURL 7.68.0 REQUIRED)

# XIT-973
# https://github.com/justusc/FindTBB gave the former solution but it's outdated and not supporting oneapi.
# find_package (TBB REQUIRED)
# include_directories (${TBB_INCLUDE_DIRS})
#
# find_package (OpenMP REQUIRED)
# if (OpenMP_CXX_FOUND)
#	message (STATUS "<xnext> openmp support found with flags ${OpenMP_C_FLAGS} § ${OpenMP_CXX_FLAGS}")
# endif ()

# Resources: https://stackoverflow.com/questions/20746936/what-use-is-find-package-if-you-need-to-specify-cmake-module-path-anyway
find_package (Valgrind MODULE)
if (${Valgrind_FOUND})
	message (STATUS "<xnext> valgrind support found")
	add_compile_definitions (XS_VALGRIND)
else ()
	message (WARNING "<xnext> valgrind support not found")
endif ()

# OpenCV. That strange find_path we always had fixed either with
# 1) symlinked wih `ln -s -T /usr/include/opencv4/ /usr/include/opencv2` + include_directories (/usr/include/opencv4/)
# 2) cmaked with -DCMAKE_CXX_FLAGS=-isystem\ /usr/include/opencv4  (probably boils down to include_directories)
# However, VSCode Intellisense and other scrapers won't find any CV code to source without the former
#
find_package (OpenCV 4.0.0 REQUIRED core imgproc highgui imgcodecs photo)
# find_package (OpenCV 4.5.0 REQUIRED) OpenCV 4.5.0 needs Ubuntu 22.04
include_directories (${OpenCV_INCLUDE_DIRS})
# Swap the former with the following lines to setup your own CV installation (e.g. 4.7.0 for Amadori experiments)
#
# include_directories (/usr/local/include/opencv4)
# link_directories (/usr/local/lib)
# set (OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs opencv_photo)  # opencv_ml opencv_objdetect)

# XML
# https://developer.gnome.org/libxml++/stable/
# HACK https://stackoverflow.com/questions/63749077/c-cmake-cant-find-libxml
# TODO read https://stackoverflow.com/questions/29191855/what-is-the-proper-way-to-use-pkg-config-from-cmake
find_package (LibXml2 REQUIRED)
find_package (PkgConfig REQUIRED)
pkg_check_modules (LIBXMLXX REQUIRED IMPORTED_TARGET libxml++-2.6)
add_compile_options (${LIBXMLXX_CFLAGS})

# BOOST
set (Boost_USE_STATIC_LIBS OFF)
set (Boost_USE_STATIC_RUNTIME OFF)
set (Boost_USE_MULTITHREADED ON)
add_definitions (-DBOOST_ALL_DYN_LINK)
add_definitions (-DBOOST_BIND_GLOBAL_PLACEHOLDERS)  # born in Ubuntu 21 https://github.com/boostorg/smart_ptr/issues/83
# These won't work
# option (BOOST_ALL_DYN_LINK "Enable Boost All Dynamic Link")
# set (BOOST_ALL_DYN_LINK)
# set (Boost_ALL_DYN_LINK ON)

# https://github.com/Microsoft/cpprestsdk/issues/1010
# https://stackoverflow.com/questions/42123509/cmake-finds-boost-but-the-imported-targets-not-available-for-boost-version
find_package (Boost 1.65.0 REQUIRED COMPONENTS log log_setup thread filesystem program_options serialization iostreams unit_test_framework)
# find_package (Boost 1.71.0 REQUIRED COMPONENTS python37)  # FIXME it seems that boost python requires explicit version. Should crash under ubuntu20
include_directories (${Boost_INCLUDE_DIRS})


#  ~  ~  ~  ~  ~  ~  ~  ~  ~  Build Type   ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~

message (STATUS "<xnext> build type is ${CMAKE_BUILD_TYPE}")

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	# add_definitions (-DDEBUG)                          # outdated I'll later use DEBUG_POSTFIX property
	# string (APPEND CMAKE_CXX_FLAGS_DEBUG " -DDEBUG")   # useless, already set by someone else
else ()
	string (APPEND CMAKE_CXX_FLAGS " -Wfatal-errors")    # we want clean releases
endif ()

file (GLOB SOURCES "cxx/pthreads_mtx_cv.c")
add_executable (pthreads_mtx_cv ${SOURCES})

#  ~  ~  ~  ~  ~  ~  ~  ~  ~  LINK: XSPECTRA  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~
function (our_target targetName)
	set_target_properties (${targetName} PROPERTIES DEBUG_POSTFIX "-dbg")

	## Add common linkage. Once X11 was involved, too.. maybe for opencv windows. Avoid bundled useless Boost_LIBRARIES linkage
	target_link_libraries (${targetName} PRIVATE
		m ssl crypto curl
		${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY}
		${Boost_SERIALIZATION_LIBRARY} ${Boost_IOSTREAMS_LIBRARY} ${Boost_LOG_LIBRARY} ${Boost_LOG_SETUP_LIBRARY}
		${OpenCV_LIBS}
		# TBB::tbb  # tbb # XIT-973
		${LIBXML2_LIBRARIES} PkgConfig::LIBXMLXX
		Threads::Threads
		# ${CMAKE_THREAD_LIBS_INIT} pthread pthreads
		)
	#if (OpenMP_CXX_FOUND)
	#	target_link_libraries (${targetName} PUBLIC OpenMP::OpenMP_CXX)
	#endif ()
endfunction ()

our_target (pthreads_mtx_cv)
