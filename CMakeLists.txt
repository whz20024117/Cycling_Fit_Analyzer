cmake_minimum_required(VERSION 3.10)

# # Debug flag for g++
# set(CMAKE_CXX_FLAGS_DEBUG  "-g")
# set(CMAKE_BUILD_TYPE Debug)

project("Cycling_Fit_Analyzer" LANGUAGES CXX)

############## QT Settings ##################
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Widgets Core Gui PrintSupport REQUIRED)

############ Static Libraries ##################
# fitsdk
file(GLOB fitsdk_SRC "lib/fitsdk/*.hpp" "lib/fitsdk/*.cpp")
add_library(fitsdk STATIC "${fitsdk_SRC}")

# qcustomplot
add_library(qcustomplot STATIC "lib/qcustomplot/qcustomplot.cpp" "lib/qcustomplot/qcustomplot.h")
target_link_libraries(qcustomplot Qt5::Widgets Qt5::PrintSupport)

########## GUI ############

add_executable(cfa_gui
    src/cfa_gui_window.ui
    src/cfa_gui_plot.cpp
    src/cfa_gui.cpp
    
    include/cfa_gui_plot.hpp
)
target_include_directories(cfa_gui PUBLIC include/ lib/fitsdk lib/ lib/qcustomplot)
target_link_libraries(cfa_gui Qt5::Widgets qcustomplot)

########## CMD ##############

add_executable(cfa_cmd 
    src/cfa_cmd.cpp 
    src/cfa_fitlistener.cpp 
    src/cfa_analysis.cpp
    src/cfa_cmd_export_plot.cpp

    include/cfa_fitlistener.hpp 
    include/cfa_analysis.hpp
    include/cfa_cmd_export_plot.hpp
)

target_include_directories(cfa_cmd PUBLIC include/ lib/fitsdk lib/ lib/qcustomplot)
target_link_libraries(cfa_cmd fitsdk Qt5::Core Qt5::Gui Qt5::Widgets qcustomplot)

