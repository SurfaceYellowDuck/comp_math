cmake_minimum_required(VERSION 3.23)
project(repo C)

set(CMAKE_C_STANDARD 99)

#add_compile_options(-fopenmp -fsanitize=address)
#find_package(OpenMP REQUIRED)
#add_executable(repo main.c ex_1.c )
add_executable(repo parallel_block.c)

#add_executable(parallel_block parallel_wave.c)
#target_link_libraries(parallel_block PRIVATE OpenMP::OpenMP_C)
