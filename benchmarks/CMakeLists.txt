add_executable(walk cpu/walk.cpp)
add_executable(latency cpu/latency.cpp)
target_compile_options(latency PRIVATE -O2 -Wall)
target_compile_options(walk PRIVATE -O2 -Wall)
