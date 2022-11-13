cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)

# If you are working in Linux and want to check for memory leaks,
# uncomment this line:
# set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address -fno-optimize-sibling-calls")

project ("Graf")

add_executable(application
    "GraphTest.cpp"
)
target_include_directories(application PRIVATE ".")
