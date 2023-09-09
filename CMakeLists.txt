project(hello_socket)

aux_source_directory(./ SRC)

foreach(src ${SRC})
	get_filename_component(target ${src} NAME_WE)
	add_executable(${target} ${src})
endforeach()

