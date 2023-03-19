cmake_minimum_required(VERSION 3.23)
project(GMS_NETWORK)

set(CMAKE_CXX_STANDARD 14)

add_executable(GMS_NETWORK main.cpp src/reader.cpp src/reader.h src/base.cpp src/base.h src/BaseBlock.cpp src/BaseBlock.h test/task.cpp test/task.h src/move.cpp src/move.h src/FakeBase.cpp src/FakeBase.h)

set(CMAKE_EXE_LINKER_FLAGS "-static")