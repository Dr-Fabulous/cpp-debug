if(NOT BUILD_TESTING)
	return()
endif()

add_executable(${PROJECT_NAME}-test main.cpp)
target_compile_features(${PROJECT_NAME}-test PUBLIC cxx_std_11)
target_link_libraries(${PROJECT_NAME}-test PUBLIC ${PROJECT_NAME})

add_test(NAME ${PROJECT_NAME}-test
	 COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-test)
