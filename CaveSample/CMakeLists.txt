# CMakeList.txt : CMake project for CMakeProject2, include source and define
# project specific logic here.

# Add source to this project's executable.
add_executable(Template "Main.cpp")

if(MSVC)
	target_compile_options(Template PRIVATE /W4 /WX)
else()
	target_compile_options(Template PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

target_link_libraries(Template PUBLIC Core)
target_link_libraries(Template PUBLIC Gameplay)
target_link_libraries(Template PUBLIC Graphics)

target_include_directories(Template PUBLIC "${PROJECT_SOURCE_DIR}/CaveEngine/Core/Public")
target_link_directories(Template PUBLIC "${PROJECT_SOURCE_DIR}/CaveEngine/Core")

target_include_directories(Template PUBLIC "${PROJECT_SOURCE_DIR}/CaveEngine/Gameplay/Public")
target_link_directories(Template PUBLIC "${PROJECT_SOURCE_DIR}/CaveEngine/Gameplay")
target_include_directories(Template PUBLIC "${PROJECT_SOURCE_DIR}/CaveEngine/Graphics/Public")
target_link_directories(Template PUBLIC "${PROJECT_SOURCE_DIR}/CaveEngine/Core")
target_link_directories(Template PUBLIC "${PROJECT_SOURCE_DIR}/CaveEngine/Graphics")
