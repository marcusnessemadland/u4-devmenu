cmake_minimum_required(VERSION 3.15)
project(uncharted4-debug-menu LANGUAGES CXX)

# Config: Runtime directory of u4.exe
set(RUNTIME_DIR ${CMAKE_SOURCE_DIR}/runtime)

# Should look something like this...
#set(RUNTIME_DIR "C:/Program Files (x86)/Steam/steamapps/common/Uncharted Legacy of Thieves Collection/")

# Force x64 build (game is 64-bit)
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "This project must be built as x64 (game is 64-bit). Configure with -A x64.")
endif()

# CMake
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# DLL
add_library(${PROJECT_NAME} SHARED main.cpp)

# 3rdparty
set(MINHOOK_DIR ${CMAKE_SOURCE_DIR}/3rdparties/minhook)
set(ASI_DIR ${CMAKE_SOURCE_DIR}/3rdparties/asi)

# Includes
target_include_directories(${PROJECT_NAME} PRIVATE
    ${MINHOOK_DIR}/include
)

# Links
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${MINHOOK_DIR}/bin/MinHook.x64.lib
)

# Rename output directly to .asi (skips the manual rename step)
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".asi")

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    # Ensure folder structure exists
    COMMAND ${CMAKE_COMMAND} -E make_directory ${RUNTIME_DIR}/scripts

    # Copy 3rdparties into runtime root
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${MINHOOK_DIR}/bin/MinHook.x64.dll
        ${RUNTIME_DIR}

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${ASI_DIR}/bin/version.dll
        ${RUNTIME_DIR}

    # Copy the built .asi into runtime/scripts
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:${PROJECT_NAME}>
        ${RUNTIME_DIR}/scripts

    COMMENT "Assembling runtime folder (MinHook + .asi)"
)