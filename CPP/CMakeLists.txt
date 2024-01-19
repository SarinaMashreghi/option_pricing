cmake_minimum_required(VERSION 3.12)  

project(option_pricing)

find_package(Python3 3.10 COMPONENTS Interpreter Development NumPy REQUIRED)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/build")

# Add your source files here
add_executable(option_pricing
	# cpp
	main.cpp
	src/binomial_model.cpp
	src/stochastic.cpp

	# h 
	include/binomial_model.h
	include/matplotlibcpp.h
	include/stochastic.h
)

include_directories({PYTHON3_INCLUDE_DIRS} ${NUMPY_INCLUDE_DIRS})
include_directories("/usr/include/python3.10/")
include_directories("/home/sarina/.local/lib/python3.10/site-packages/numpy/core/include/")
target_link_libraries(option_pricing ${Python3_LIBRARIES})


