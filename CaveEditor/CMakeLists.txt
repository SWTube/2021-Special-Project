if (WIN32)
	file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Public/*.h")
	file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Private/*.cpp")

	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
	include_directories($ENV{UniversalCRTSdkDir}/Include/10.18362.0/ucrt)
	include_directories($ENV{UniversalCRTSdkDir}/Include/10.18362.0/um)

	add_executable(Editor WIN32 ${HEADER_FILES} ${SOURCE_FILES})

	if(MSVC)
		target_compile_options(Editor PRIVATE /W4 /WX)
	else()
		target_compile_options(Editor PRIVATE -Wall -Wextra -pedantic -Werror)
	endif()


	target_include_directories(Editor PUBLIC 
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>" 
	"$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>")
endif (WIN32)