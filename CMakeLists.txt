cmake_minimum_required(VERSION 3.10)

project(coulang LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

add_compile_options(-Wall -g)

add_executable(coulang
	${CMAKE_SOURCE_DIR}/src/ast.c
	${CMAKE_SOURCE_DIR}/src/main.c
	${CMAKE_SOURCE_DIR}/src/lexer.c
	${CMAKE_SOURCE_DIR}/src/map.c
	${CMAKE_SOURCE_DIR}/src/parser.c
	${CMAKE_SOURCE_DIR}/src/scope.c
	${CMAKE_SOURCE_DIR}/src/string.c
	${CMAKE_SOURCE_DIR}/src/token.c
	${CMAKE_SOURCE_DIR}/src/value.c)

target_include_directories(coulang PRIVATE ${CMAKE_SOURCE_DIR}/include)
