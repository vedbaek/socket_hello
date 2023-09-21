project(hello_socket)

aux_source_directory(./ SRC)
set(CMAKE_BUILD_TYPE "Debug")

foreach(src ${SRC})
	get_filename_component(target ${src} NAME_WE)
	add_executable(${target} ${src})
endforeach()

