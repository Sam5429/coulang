cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

add_executable(coulang
	${CMAKE_SOURCE_DIR}/src/main.c ${CMAKE_SOURCE_DIR}/src/lexer.c)
