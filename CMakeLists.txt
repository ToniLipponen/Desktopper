cmake_minimum_required(VERSION 3.0)
project(Desktopper)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)
pkg_check_modules(adwaita REQUIRED libadwaita-1)

include_directories(${GTK4_INCLUDE_DIRS} ${adwaita_INCLUDE_DIRS})
link_directories(${GTK4_LIBRARY_DIRS} ${adwaita_LIBRARY_DIRS})
add_definitions(${GTK4_CFLAGS_OTHER} ${adwaita_CFLAGS_OTHER})

add_executable(${CMAKE_PROJECT_NAME} src/main.cpp src/Application.cpp src/Signals.h src/Signals.cpp src/Common.h src/Util.h src/Incbin.h src/EntryFile.h src/EntryFile.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} ${GTK4_LIBRARIES} ${adwaita_LIBRARIES})