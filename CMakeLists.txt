cmake_minimum_required(VERSION 3.15)
project(CxxGRAV VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(CxxGRAV ${SOURCES})

set_target_properties(CxxGRAV PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_compile_options(CxxGRAV PRIVATE -O2)

if (WIN32)
    if (MSVC)
        set(RAYLIB_URL https://github.com/raysan5/raylib/releases/download/5.5/raylib-5.5_win64_msvc16.zip)
        set(RAYLIB_ARCHIVE ${CMAKE_BINARY_DIR}/raylib_msvc.zip)
        set(RAYLIB_ROOT ${CMAKE_SOURCE_DIR}/libmsvc)
    else()
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
    file(DOWNLOAD ${RAYLIB_URL} ${RAYLIB_ARCHIVE} SHOW_PROGRESS)
endif()

file(ARCHIVE_EXTRACT INPUT ${RAYLIB_ARCHIVE} DESTINATION ${RAYLIB_ROOT})

file(GLOB RAYLIB_SUBDIRS LIST_DIRECTORIES true "${RAYLIB_ROOT}/raylib-*")
list(SORT RAYLIB_SUBDIRS)
list(GET RAYLIB_SUBDIRS 0 RAYLIB_DIR)

set(RAYLIB_INCLUDE_DIR "${RAYLIB_DIR}/include")
set(RAYLIB_LIB_DIR     "${RAYLIB_DIR}/lib")

if (WIN32)
    target_include_directories(CxxGRAV PRIVATE ${RAYLIB_INCLUDE_DIR})
    target_link_directories(CxxGRAV PRIVATE ${RAYLIB_LIB_DIR})
    target_link_libraries(CxxGRAV PRIVATE raylib opengl32 gdi32 winmm ws2_32)

    if (MSVC)
        target_compile_options(CxxGRAV PRIVATE /O2)
        target_link_options(CxxGRAV PRIVATE /INCREMENTAL:NO)
    else()
        target_link_options(CxxGRAV PRIVATE -s)
    endif()

elseif(UNIX)
    find_package(OpenGL)
    find_package(X11)

    if (NOT OpenGL_FOUND OR NOT X11_FOUND)
        execute_process(
            COMMAND bash ${CMAKE_SOURCE_DIR}/setup.sh
            RESULT_VARIABLE setup_result
        )
        if (NOT setup_result EQUAL 0)
            message(FATAL_ERROR "setup.sh failed.")
        endif()
    endif()

    target_include_directories(CxxGRAV PRIVATE ${RAYLIB_INCLUDE_DIR})
    target_link_directories(CxxGRAV PRIVATE ${RAYLIB_LIB_DIR})
    target_link_libraries(CxxGRAV PRIVATE raylib m dl pthread X11 GL GLU)
    target_compile_options(CxxGRAV PRIVATE -Wall -Wextra -Wpedantic)
endif()

include(FetchContent)

FetchContent_Declare(
    imgui
    URL https://github.com/ocornut/imgui/archive/refs/heads/master.zip
)

FetchContent_Declare(
    rlimgui
    URL https://github.com/raylib-extras/rlImGui/archive/refs/heads/main.zip
)

FetchContent_MakeAvailable(imgui rlimgui)

add_library(imgui_bundle STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${rlimgui_SOURCE_DIR}/rlImGui.cpp
)

target_include_directories(imgui_bundle PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${rlimgui_SOURCE_DIR}
    ${RAYLIB_INCLUDE_DIR}
)

target_link_libraries(CxxGRAV PRIVATE imgui_bundle)

install(TARGETS CxxGRAV DESTINATION bin)
