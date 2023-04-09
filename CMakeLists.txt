cmake_minimum_required(VERSION 3.0)
project(Desktopper)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
include(cmake/GTK4.cmake)
include(cmake/libadwaita.cmake)

add_executable(${CMAKE_PROJECT_NAME}
        src/main.cpp
        src/Application.cpp
        src/Signals.cpp
        src/EntryFile.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME} ${GTK4_LIBRARIES} ${adwaita_LIBRARIES})