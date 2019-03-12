cmake_minimum_required (VERSION 3.1)
project (Project2)
set (CMAKE_CXX_STANDARD 11)

# add_compile_options(-pthread)
# add_compile_options(-lrt)
# add_compile_options(-lphtread)

find_package(Threads)

file(GLOB SOURCES_HEADERS "*.h" "*.cc" "*.cpp")
add_executable(a.out ${SOURCES_HEADERS})
target_link_libraries (a.out ${CMAKE_THREAD_LIBS_INIT})

