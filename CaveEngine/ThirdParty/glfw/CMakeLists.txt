file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Public/*.h")
file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Private/*.cpp")

add_library(Glfw SHARED ${HEADER_FILES} ${SOURCE_FILES})

if(MSVC)
	target_compile_options(Glfw PRIVATE /W4 /WX)
else()
	target_compile_options(Glfw PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

if (UNIX)
	set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

	set(CMAKE_FIND_DEBUG_MODE TRUE)
	find_package(glfw3 3.3 REQUIRED PATH /usr/lib/x86_64-linux-gnu)
	
	set(OpenGL_GL_PREFERENCE "GLVND")
	find_package(OpenGL REQUIRED)
	set(CMAKE_FIND_DEBUG_MODE FALSE)
	target_link_libraries(Glfw PUBLIC OpenGL::GL)
	target_link_libraries(Glfw PUBLIC glfw)
elseif(WIN32)
	#target_link_libraries(Graphics PUBLIC d2d1.lib)
	#
	#
	#target_include_directories(Graphics PUBLIC
	#"C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\um"
	#)
	find_path(DirectX11_INCLUDE_DIR NAMES d2d1.h HINTS "C:/Program Files (x86)/Windows Kits/10/Include/10.0.19041.0/um")
	set(DirectX11_LIBRARY d2d1.lib)
endif()

target_include_directories(Glfw PUBLIC 
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>" 
  "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>"
  $<INSTALL_INTERFACE:include/CAVE_ENGINE>)