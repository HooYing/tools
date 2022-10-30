# CMakeList.txt: tools 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("tools")

include_directories("./include")

# 将源代码添加到此项目的可执行文件。
add_executable (tools "tools.cpp" "tools.h" "dump/getDump.h" "include/pch.h" "dump/enum2string.h" "dump/hyLog.h" "dump/hyLog.cpp" "dump/hyTimeStamp.cpp"    "dump/hyCurrentThread.h" "dump/hyCurrentThread.cpp" "dump/hyThread.h" "dump/hyThread.cpp" "src/hyLogStream.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET tools PROPERTY CXX_STANDARD 20)
endif()

# TODO: 如有需要，请添加测试并安装目标。
