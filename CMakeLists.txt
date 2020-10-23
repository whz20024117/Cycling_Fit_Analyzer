cmake_minimum_required(VERSION 3.10)


# Building Windows exe-----------------------------------------
option(Win "Build for windows. Need to specify compiler using -DCMAKE_CXX_COMPILER" OFF)
if(Win)
    message(Status "\n**********To build for windows, please specify compiler using -DCMAKE_CXX_COMPILER***********")

    set(CMAKE_SYSTEM_NAME Windows)
    set(TOOLCHAIN_PREFIX x86_64-w64-mingw32)

    # cross compilers to use for C, C++ and Fortran
    # set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
    # set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)
    # set(CMAKE_Fortran_COMPILER ${TOOLCHAIN_PREFIX}-gfortran)
    # set(CMAKE_RC_COMPILER ${TOOLCHAIN_PREFIX}-windres)

    # target environment on the build host system
    set(CMAKE_FIND_ROOT_PATH /usr/${TOOLCHAIN_PREFIX})

    # modify default behavior of FIND_XXX() commands
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

    # Compiler flag
    set(CMAKE_BUILD_TYPE Release)
    set(CMAKE_CXX_FLAGS_RELEASE "-static")

endif()
#--------------------------------------------------

# Debug flag for g++
# set(CMAKE_CXX_FLAGS_DEBUG  "-g")
# set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
project("Cycling_Fit_Analyzer" LANGUAGES CXX)

add_executable(cfa src/main.cpp src/my_listener.cpp)
target_include_directories(cfa PUBLIC include lib/fitsdk lib/)

file(GLOB fitsdk_SRC "lib/fitsdk/*.hpp" "lib/fitsdk/*.cpp")
add_library(fitsdk STATIC "${fitsdk_SRC}")

target_link_libraries(cfa PRIVATE fitsdk)

# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
