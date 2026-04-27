cmake_minimum_required(VERSION 3.16)

# ccache 加速重编译（必须在 project() 之前设置）
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
endif()

# lld 加速链接
find_program(LLD_LINKER ld.lld)
if(LLD_LINKER)
    set(CMAKE_LINKER ${LLD_LINKER})
endif()

project(avl_tree_project VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置默认构建类型为 Debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# 定义不同构建类型的编译选项
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -Wall -Wextra -Wpedantic")

add_executable(avl_demo
    src/main.cpp
)

target_include_directories(avl_demo PRIVATE src)

# 编译选项（根据构建类型自动应用）
target_compile_options(avl_demo PRIVATE
    $<$<CONFIG:Debug>:-g -O0>
    $<$<CONFIG:Release>:-O3>
)

# ===================== 测试 =====================
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(avl_test
    tests/test_avl.cpp
)

target_include_directories(avl_test PRIVATE src)
target_link_libraries(avl_test PRIVATE GTest::gtest_main)

target_compile_options(avl_test PRIVATE
    $<$<CONFIG:Debug>:-g -O0>
    $<$<CONFIG:Release>:-O3>
)

include(GoogleTest)
gtest_discover_tests(avl_test)