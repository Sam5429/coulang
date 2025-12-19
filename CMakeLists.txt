cmake_minimum_required(VERSION 3.10)

project(coulang LANGUAGES C ASM)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

add_compile_options(-Wall -g)

if(CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "x86_64")
	set(SYMBOL_RUNNER ${CMAKE_SOURCE_DIR}/src/x86_64/symbol_runner.S)
else()
	message(FATAL_ERROR "`${CMAKE_HOST_SYSTEM_PROCESSOR}` is not supported")
endif()

add_executable(coulang
	${SYMBOL_RUNNER}
	${CMAKE_SOURCE_DIR}/src/ast.c
	${CMAKE_SOURCE_DIR}/src/function.c
	${CMAKE_SOURCE_DIR}/src/main.c
	${CMAKE_SOURCE_DIR}/src/lexer.c
	${CMAKE_SOURCE_DIR}/src/interpreter.c
	${CMAKE_SOURCE_DIR}/src/map.c
	${CMAKE_SOURCE_DIR}/src/parser.c
	${CMAKE_SOURCE_DIR}/src/scope.c
	${CMAKE_SOURCE_DIR}/src/string.c
	${CMAKE_SOURCE_DIR}/src/token.c
	${CMAKE_SOURCE_DIR}/src/value.c
	${CMAKE_SOURCE_DIR}/src/variable.c)

target_include_directories(coulang PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(coulang dl)

add_library(rayglue SHARED ${CMAKE_SOURCE_DIR}/game/rayglue.c)
target_include_directories(rayglue PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(rayglue raylib)

add_library(gameglue SHARED ${CMAKE_SOURCE_DIR}/game/gameglue.c)
target_include_directories(gameglue PRIVATE ${CMAKE_SOURCE_DIR}/include)
