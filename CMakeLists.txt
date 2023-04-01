cmake_minimum_required(VERSION 3.10)

project(learn_opengl)


file(GLOB_RECURSE C_SOURCES src/*.c)
file(GLOB_RECURSE CPP_SOURCES src/*.cpp)

add_executable(
	learn_opengl
	${C_SOURCES}
	${CPP_SOURCES}
)


target_link_directories(
	learn_opengl PUBLIC
	lib
)

target_link_libraries(
	learn_opengl
	opengl32
	glfw3
	assimp-vc143-mtd
)

target_include_directories(
	learn_opengl PUBLIC
	include
)
