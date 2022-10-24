cmake_minimum_required(VERSION 3.10...3.18)
project(Mockturtle_api LANGUAGES CXX)

find_package(Python 3.8 COMPONENTS Interpreter Development REQUIRED)

set(MOCKTURTLE_CXX_STANDARD "17" CACHE STRING "C++ standard")
set(CMAKE_CXX_STANDARD ${MOCKTURTLE_CXX_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(extern)

set(CMAKE_CXX_FLAGS "-std=gnu++17 -Wall -march=native -fopenmp -fPIC -lreadline -O3 -g -DNDEBUG -W -Wextra -Wno-unknown-pragmas -pthread -DABC_NAMESPACE=pabc -DABC_NO_USE_READLINE -DDISABLE_NAUTY -DFMT_HEADER_ONLY -DLIN64")

include_directories(extern/mockturtle/include)
include_directories(extern/mockturtle/lib/abcesop)
include_directories(extern/mockturtle/lib/abcresub)
include_directories(extern/mockturtle/lib/abcsat)
include_directories(extern/mockturtle/lib/bill)
include_directories(extern/mockturtle/lib/fmt)
include_directories(extern/mockturtle/lib/json)
include_directories(extern/mockturtle/lib/kitty)
include_directories(extern/mockturtle/lib/lorina)
include_directories(extern/mockturtle/lib/matplot)
include_directories(extern/mockturtle/lib/nauty)
include_directories(extern/mockturtle/lib/parallel_hashmap)
include_directories(extern/mockturtle/lib/percy)
include_directories(extern/mockturtle/lib/rang)

pybind11_add_module(Mockturtle_api src/mockturtle_py.cpp)