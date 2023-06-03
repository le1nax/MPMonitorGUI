# If someone uses a lower version of CMake building this fails.
cmake_minimum_required (VERSION 3.17)

# Name of your project.
project(BasicGui CXX)

#set module path
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
list(APPEND CMAKE_PREFIX_PATH "C:/src/vcpkg/packages")

# Project version (e.g. 1.0.0).
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

# C++ standard to use.
set(CXX_STANDARD 17)
message("Generating project ${CMAKE_PROJECT_NAME} ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")


#setup with PkgConfig
#include(FindPkgConfig)
#if(NOT PKG_CONFIG_FOUND)
#    MESSAGE(FATAL_ERROR "No PKG_CONFIG Found")
#endif()


# Find the required libs from your package manager (vcpkg).
# find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(Threads REQUIRED)


# target_link_libraries(${PROJECT_NAME} PRIVATE
#     # PkgConfig::GLAD
#     PkgConfig::GLFW
#     PkgConfig::IMGUI
# )

# Add an executable MyApp.exe running the app.
add_executable(
  BasicApp
  "main.cpp"
  # other .cpp files go here
 "source/ControllerBox.cpp")

# Link the GUI libraries into the executable.
target_link_libraries(BasicApp PUBLIC glfw imgui::imgui Threads::Threads ws2_32 -lsocket -lnsl -pthread)

# Add the folder with 'gui_application.hpp' and 'gui_main.hpp' to the include path.
# This way we can '#include<gui_application.hpp>'.
target_include_directories(BasicApp PRIVATE "../lib")
