cmake_minimum_required(VERSION 3.20)
project(appsec_review_smoke_cpp LANGUAGES C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(smoke_synthetic src/synthetic.cpp)

