cmake_minimum_required(VERSION 3.5)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(clientMultmatrix LANGUAGES CXX)
add_executable(clientMultmatrix utils.h utils.cpp clientMultmatrix.cpp multmatrix_stub.h operaciones.h)
target_link_libraries(clientMultmatrix pthread)

project(serverMultmatrix LANGUAGES CXX)
add_executable(serverMultmatrix utils.h utils.cpp serverMultmatrix.cpp multmatrix.h multmatrix.cpp multmatrix_imp.h operaciones.h)
target_link_libraries(serverMultmatrix pthread)