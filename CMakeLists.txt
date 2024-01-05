cmake_minimum_required(VERSION 3.12)  

project(option_pricing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/build")

# Add your source files here
add_executable(option_pricing
	# cpp
	main.cpp
	src/binomial_model.cpp

	# h 
	include/binomial_model.h
)

# target_link_libraries(YourExecutableName YourLibraryName)

# target_include_directories(YourExecutableName PRIVATE include)

