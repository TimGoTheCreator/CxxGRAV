cmake_minimum_required(VERSION 3.15)
project(SemiEuler VERSION 0.9.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(SemiEuler ${SOURCES})

set_target_properties(SemiEuler PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_compile_options(SemiEuler PRIVATE -O2)

# --- Auto-download Raylib per platform ---
if (WIN32)
    if (MSVC)
        # MSVC-compatible Raylib binary
        set(RAYLIB_URL https://github.com/raysan5/raylib/releases/download/5.5/raylib-5.5_win64_msvc16.zip)
        set(RAYLIB_ARCHIVE ${CMAKE_BINARY_DIR}/raylib_msvc.zip)
        set(RAYLIB_ROOT ${CMAKE_SOURCE_DIR}/libmsvc)
    else()
        # MinGW-compatible Raylib binary
        set(RAYLIB_URL https://github.com/raysan5/raylib/releases/download/5.5/raylib-5.5_win64_mingw-w64.zip)
        set(RAYLIB_ARCHIVE ${CMAKE_BINARY_DIR}/raylib.zip)
        set(RAYLIB_ROOT ${CMAKE_SOURCE_DIR}/lib)
    endif()
elseif(UNIX)
    set(RAYLIB_URL https://github.com/raysan5/raylib/releases/download/5.5/raylib-5.5_linux_amd64.tar.gz)
    set(RAYLIB_ARCHIVE ${CMAKE_BINARY_DIR}/raylib.tar.gz)
    set(RAYLIB_ROOT ${CMAKE_SOURCE_DIR}/liblinux)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

file(MAKE_DIRECTORY ${RAYLIB_ROOT})

if (NOT EXISTS ${RAYLIB_ARCHIVE})
    message(STATUS "Downloading Raylib from ${RAYLIB_URL}")
    file(DOWNLOAD ${RAYLIB_URL} ${RAYLIB_ARCHIVE} SHOW_PROGRESS)
endif()

# Unpack archive
message(STATUS "Extracting Raylib to ${RAYLIB_ROOT}")
file(ARCHIVE_EXTRACT INPUT ${RAYLIB_ARCHIVE} DESTINATION ${RAYLIB_ROOT})

# Detect the extracted subdirectory
file(GLOB RAYLIB_SUBDIRS LIST_DIRECTORIES true "${RAYLIB_ROOT}/raylib-*")
list(LENGTH RAYLIB_SUBDIRS _raylib_count)
if (_raylib_count EQUAL 0)
    message(FATAL_ERROR "Raylib extraction failed—no raylib-* directory found in ${RAYLIB_ROOT}")
elseif(_raylib_count GREATER 1)
    list(SORT RAYLIB_SUBDIRS)
    list(GET RAYLIB_SUBDIRS 0 RAYLIB_DIR)
else()
    list(GET RAYLIB_SUBDIRS 0 RAYLIB_DIR)
endif()

set(RAYLIB_INCLUDE_DIR "${RAYLIB_DIR}/include")
set(RAYLIB_LIB_DIR     "${RAYLIB_DIR}/lib")

# --- Platform-specific includes and libs ---
if (WIN32)
    target_include_directories(SemiEuler PRIVATE ${RAYLIB_INCLUDE_DIR})
    target_link_directories(SemiEuler PRIVATE ${RAYLIB_LIB_DIR})
    target_link_libraries(SemiEuler PRIVATE raylib opengl32 gdi32 winmm ws2_32)

    if (MSVC)
        target_compile_options(SemiEuler PRIVATE /O2)
        target_link_options(SemiEuler PRIVATE /INCREMENTAL:NO)
    else()
        target_link_options(SemiEuler PRIVATE -s)
    endif()

elseif(UNIX)
    # Check for system dependencies
    find_package(OpenGL)
    find_package(X11)

    if (NOT OpenGL_FOUND OR NOT X11_FOUND)
        message(WARNING "Missing system libraries. Running setup.sh to install dependencies...")
        execute_process(
            COMMAND bash ${CMAKE_SOURCE_DIR}/setup.sh
            RESULT_VARIABLE setup_result
        )
        if (NOT setup_result EQUAL 0)
            message(FATAL_ERROR "setup.sh failed. Please run it manually with sudo.")
        endif()
    endif()

    target_include_directories(SemiEuler PRIVATE ${RAYLIB_INCLUDE_DIR})
    target_link_directories(SemiEuler PRIVATE ${RAYLIB_LIB_DIR})
    target_link_libraries(SemiEuler PRIVATE raylib m dl pthread X11 GL GLU)
    target_compile_options(SemiEuler PRIVATE -Wall -Wextra -Wpedantic)
endif()

install(TARGETS SemiEuler DESTINATION bin)
