cmake_minimum_required(VERSION 3.23)
project(GMS_NETWORK)

set(CMAKE_CXX_STANDARD 14)

add_executable(GMS_NETWORK main.cpp IO/reader.cpp IO/reader.h core/base.cpp core/base.h core/BaseBlock.cpp core/BaseBlock.h core/task.cpp core/task.h core/move.cpp core/move.h core/FakeBase.cpp core/FakeBase.h)
