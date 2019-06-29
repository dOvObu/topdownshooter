cmake_minimum_required(VERSION 3.1)

project(topdownshooter)

set(SFML_LIB_DIR "C:/Program Files (x86)/SFML/lib/")
set(SFML_INCL_DIR "C:/Program Files (x86)/SFML/include/")

include_directories(${SFML_INCL_DIR})
link_directories(${SFML_LIB_DIR})

add_executable(topdownshooter
	main.cpp
	aabb.cpp
	audiomanager.cpp
	evilbox.cpp
	magnumbullet.cpp
	mouse.cpp
	player.cpp
	tot.cpp
	vec.cpp
	math_constants.cpp
)

target_link_libraries(topdownshooter sfml-system sfml-window sfml-graphics sfml-audio)

