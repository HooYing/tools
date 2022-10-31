# CMakeList.txt: tools 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("tools")

include_directories("./include")

file(GLOB_RECURSE CPP_FILES src/*.cpp tools.cpp)
message(${CPP_FILES})
list(REMOVE_ITEM CPP_FILES ${PROJECT_SOURCE_DIR}/src/hyThread.cpp)
message(${CPP_FILES})

set(CMAKE_CXX_FLAGS_DEBUG "/MDd /Zi /Ob0 /O2")
message(${CMAKE_CXX_FLAGS_DEBUG})

# 将源代码添加到此项目的可执行文件。
add_executable (tools ${CPP_FILES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET tools PROPERTY CXX_STANDARD 20)
endif()

# TODO: 如有需要，请添加测试并安装目标。
