cmake_minimum_required(VERSION 3.15)
project(SemiEuler VERSION 0.9.0 LANGUAGES CXX)

# Require modern C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Collect all source files in src/
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

# Define the executable
add_executable(SemiEuler ${SOURCES})

# Organize output binaries into bin/
set_target_properties(SemiEuler PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Add optimization flags (matches -O2)
target_compile_options(SemiEuler PRIVATE -O2)

# Platform-specific includes and libs
if (WIN32)
    # Windows headers + libs
    target_include_directories(SemiEuler PRIVATE include)
    target_link_directories(SemiEuler PRIVATE lib)
    target_link_libraries(SemiEuler PRIVATE raylib opengl32 gdi32 winmm ws2_32)

    # Strip symbols for smaller .exe
    target_link_options(SemiEuler PRIVATE -s)

elseif(UNIX)
    # Linux headers + libs
    target_include_directories(SemiEuler PRIVATE include)
    target_link_directories(SemiEuler PRIVATE liblinux)
    target_link_libraries(SemiEuler PRIVATE raylib m dl pthread X11 GL GLU)

    # Extra warnings on Linux
    target_compile_options(SemiEuler PRIVATE -Wall -Wextra -Wpedantic)
endif()

install(TARGETS SemiEuler DESTINATION bin)
