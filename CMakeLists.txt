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

# # Debug flag for g++
# set(CMAKE_CXX_FLAGS_DEBUG  "-g")
# set(CMAKE_BUILD_TYPE Debug)

project("Cycling_Fit_Analyzer" LANGUAGES CXX)

############## QT Settings ##################
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

############ Static Libraries ##################
# fitsdk
file(GLOB fitsdk_SRC "lib/fitsdk/*.hpp" "lib/fitsdk/*.cpp")
add_library(fitsdk STATIC "${fitsdk_SRC}")

# qcustomplot
add_library(qcustomplot STATIC "lib/qcustomplot/qcustomplot.cpp" "lib/qcustomplot/qcustomplot.h")
target_link_libraries(qcustomplot Qt5::Widgets)

########## GUI ############

add_executable(cfa_gui
    src/cfa_gui_window.ui
    src/cfa_gui_window.cpp
    src/cfa_gui.cpp
    include/cfa_gui_window.hpp
)
target_include_directories(cfa_gui PUBLIC include/)
target_link_libraries(cfa_gui Qt5::Widgets)

########## CMD ##############

add_executable(cfa_cmd 
    src/cfa_cmd.cpp 
    src/cfa_fitlistener.cpp 
    src/cfa_analysis.cpp

    include/cfa_fitlistener.hpp 
    include/cfa_analysis.hpp
)

target_include_directories(cfa_cmd PUBLIC include/ lib/fitsdk lib/)
target_link_libraries(cfa_cmd PRIVATE fitsdk)

# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
