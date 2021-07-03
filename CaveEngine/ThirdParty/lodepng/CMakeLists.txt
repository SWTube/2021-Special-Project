file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Public/*.h")
file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Private/*.cpp")

add_library(Lodepng SHARED ${HEADER_FILES} ${SOURCE_FILES})

if(MSVC)
	target_compile_options(Lodepng PRIVATE /W4 /WX)
else()
	target_compile_options(Lodepng PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

target_include_directories(Lodepng PUBLIC 
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>" 
  "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>"
  $<INSTALL_INTERFACE:include/CAVE_ENGINE>)