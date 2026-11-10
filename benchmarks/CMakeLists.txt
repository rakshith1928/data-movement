add_executable(walk cpu/walk.cpp)
target_compile_options(walk PRIVATE -O2 -Wall)

add_executable(latency cpu/latency.cpp)
target_compile_options(latency PRIVATE -O2 -Wall)
