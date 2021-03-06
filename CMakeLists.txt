cmake_minimum_required(VERSION 3.0)
project(LIBTENSOR CXX)

if(NOT LIBUTIL_SOURCE_DIR)
set(LIBUTIL_SOURCE_DIR ${LIBTENSOR_SOURCE_DIR})
endif(NOT LIBUTIL_SOURCE_DIR)

include(cmake/CheckFortranSourceCompiles.cmake)
include(cmake/CheckFortranSourceRuns.cmake)

if(LIBVMM_SOURCE_DIR)
    set(HAVE_LIBVMM TRUE)
else(LIBVMM_SOURCE_DIR)
    set(HAVE_LIBVMM FALSE)
    message(STATUS "Warning: libvmm was not found")
endif(LIBVMM_SOURCE_DIR)

if(LIBTENSOR_STANDALONE)
    include(${LIBUTIL_SOURCE_DIR}/cmake/SetupLibUtil.txt)
    if(HAVE_LIBVMM)
        include(${LIBVMM_SOURCE_DIR}/cmake/SetupLibVmm.txt)
    endif(HAVE_LIBVMM)
endif(LIBTENSOR_STANDALONE)
include(cmake/DepsLibTensor.txt)

enable_testing()
if(LIBTENSOR_STANDALONE)
    add_subdirectory(libutil)
endif(LIBTENSOR_STANDALONE)

add_subdirectory(libtensor)
add_subdirectory(tests)
#add_subdirectory(performance_tests)
