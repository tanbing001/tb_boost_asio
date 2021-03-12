set(BOOST_LIB ${Boost_LIBRARIES} "${CMAKE_THREAD_LIBS_INIT}")

set(TARGETS
    helloworld
	async_timer
	mem_func
	multithread
	tcp_client
	sync_tcp_server
	async_tcp_server
)

foreach(name ${TARGETS})
    add_executable(${name} ${name}.cpp)
	target_link_libraries(${name} ${BOOST_LIB})
endforeach()

