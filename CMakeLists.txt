project(hello_socket)

aux_source_directory(./ SRC)
set(CMAKE_BUILD_TYPE "Debug")

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")
#find_package(Threads REQUIRED)
#link_libraries(PRIVATE Threads::Threads)

foreach(src ${SRC})
	get_filename_component(target ${src} NAME_WE)
	add_executable(${target} ${src})
endforeach()

