#-------------------------------------------------------------------
# This file is part of the CMake build system for CAVE_ENGINE
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

############################################################
# CaveEngine graphics library
############################################################

# # define header and source files for the library
file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Public/*.h")
# list(APPEND HEADER_FILES
#     ${PROJECT_BINARY_DIR}/include/OgreBuildSettings.h
#     ${PROJECT_BINARY_DIR}/include/OgreComponents.h
#     ${PROJECT_BINARY_DIR}/include/OgreExports.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/include/Ogre.i)

file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Private/*.cpp")

file(GLOB DEVICE_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Public/Device/*.h")
file(GLOB RENDERER_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Public/Renderer/*.h")
file(GLOB SHADER_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Public/Shader/*.h")
file(GLOB SPRITE_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Public/Sprite/*.h")

file(GLOB DEVICE_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Private/Device/*.cpp")
file(GLOB RENDERER_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Private/Renderer/*.cpp")
file(GLOB SHADER_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Private/Shader/*.cpp")
file(GLOB SPRITE_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Private/Sprite/*.cpp")

# # Remove optional header files
# list(REMOVE_ITEM HEADER_FILES
#     "${CMAKE_CURRENT_SOURCE_DIR}/include/OgreDDSCodec.h"
#     "${CMAKE_CURRENT_SOURCE_DIR}/include/OgrePVRTCCodec.h"
#     "${CMAKE_CURRENT_SOURCE_DIR}/include/OgreETCCodec.h"
#     "${CMAKE_CURRENT_SOURCE_DIR}/include/OgreZip.h"
# )

# # Remove optional source files
# list(REMOVE_ITEM SOURCE_FILES
#     "${CMAKE_CURRENT_SOURCE_DIR}/src/OgreFileSystemLayerNoOp.cpp"
#     "${CMAKE_CURRENT_SOURCE_DIR}/src/OgreDDSCodec.cpp"
#     "${CMAKE_CURRENT_SOURCE_DIR}/src/OgrePVRTCCodec.cpp"
#     "${CMAKE_CURRENT_SOURCE_DIR}/src/OgreETCCodec.cpp"
#     "${CMAKE_CURRENT_SOURCE_DIR}/src/OgreZip.cpp"
#     "${CMAKE_CURRENT_SOURCE_DIR}/src/OgreSearchOps.cpp"
# )

# # Add platform specific files
# if (EMSCRIPTEN)
#   set(PLATFORM_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/OgreFileSystemLayerNoOp.cpp")
# elseif (WIN32)
#   file(GLOB PLATFORM_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/WIN32/*.cpp")
# elseif (APPLE)
#   if (APPLE_IOS)
#     include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/iOS")
#     file(GLOB PLATFORM_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/iOS/*.cpp"
#       "${CMAKE_CURRENT_SOURCE_DIR}/src/iOS/*.mm")

#     set(PLATFORM_LIBS "")
#   else ()
#     include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/OSX")
#     file(GLOB PLATFORM_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/OSX/*.cpp"
#       "${CMAKE_CURRENT_SOURCE_DIR}/src/OSX/*.mm")
#     set(PLATFORM_LIBS "-framework CoreFoundation -framework Foundation")
#   endif ()
# elseif(ANDROID)
#   # required by OgrePlatformInformation.cpp
#   include_directories(${ANDROID_NDK}/sources/android/cpufeatures)

#   file(GLOB PLATFORM_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/Android/*.cpp")
#   # Remove optional platform source files
#   list(REMOVE_ITEM PLATFORM_SOURCE_FILES
#     "${CMAKE_CURRENT_SOURCE_DIR}/src/Android/OgreAPKZipArchive.cpp"
#   )
# elseif (UNIX)
#   file(GLOB PLATFORM_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/GLX/*.cpp")

#   set(PLATFORM_LIBS pthread)
# endif()

# if(NOT WIN32)
#     list(APPEND PLATFORM_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/OgreSearchOps.cpp")
# endif()

# # Configure threading files
# file(GLOB THREAD_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/Threading/*.h")
# include_directories("include/Threading" "src/")

# if (OGRE_THREAD_PROVIDER EQUAL 0)
# 	list(APPEND THREAD_HEADER_FILES
# 		include/Threading/OgreThreadDefinesNone.h
# 		include/Threading/OgreDefaultWorkQueueStandard.h
# 	)
# 	set(THREAD_SOURCE_FILES
# 		src/Threading/OgreDefaultWorkQueueStandard.cpp
# 	)
# elseif (OGRE_THREAD_PROVIDER EQUAL 1)
#   include_directories(${Boost_INCLUDE_DIRS})
#   link_directories(${Boost_LIBRARY_DIRS})

# 	list(APPEND THREAD_HEADER_FILES
# 		include/Threading/OgreThreadDefinesBoost.h
# 		include/Threading/OgreThreadHeadersBoost.h
# 		include/Threading/OgreDefaultWorkQueueStandard.h
# 	)
# 	set(THREAD_SOURCE_FILES
# 		src/Threading/OgreDefaultWorkQueueStandard.cpp
# 	)
# elseif (OGRE_THREAD_PROVIDER EQUAL 2)
# 	list(APPEND THREAD_HEADER_FILES
# 		include/Threading/OgreThreadDefinesPoco.h
# 		include/Threading/OgreThreadHeadersPoco.h
# 		include/Threading/OgreDefaultWorkQueueStandard.h
# 	)
# 	set(THREAD_SOURCE_FILES
# 		src/Threading/OgreDefaultWorkQueueStandard.cpp
# 	)
# elseif (OGRE_THREAD_PROVIDER EQUAL 3)
# 	list(APPEND THREAD_HEADER_FILES
# 		include/Threading/OgreThreadDefinesTBB.h
# 		include/Threading/OgreThreadHeadersTBB.h
# 		include/Threading/OgreDefaultWorkQueueTBB.h
# 	)
# 	set(THREAD_SOURCE_FILES
# 		src/Threading/OgreDefaultWorkQueueTBB.cpp
# 	)
# elseif (OGRE_THREAD_PROVIDER EQUAL 4)
# 	list(APPEND THREAD_HEADER_FILES
# 		include/Threading/OgreThreadDefinesSTD.h
# 		include/Threading/OgreThreadHeadersSTD.h
# 		include/Threading/OgreDefaultWorkQueueStandard.h
# 	)
# 	list(APPEND THREAD_SOURCE_FILES
# 		src/Threading/OgreDefaultWorkQueueStandard.cpp
# 	)
# endif ()

# list(APPEND HEADER_FILES ${THREAD_HEADER_FILES})

# set(LIBRARIES)

# if (OGRE_CONFIG_THREADS)
#   list(APPEND LIBRARIES ${OGRE_THREAD_LIBRARIES})
# endif ()

# if (OGRE_CONFIG_ENABLE_DDS)
#   list(APPEND HEADER_FILES include/OgreDDSCodec.h)
#   list(APPEND SOURCE_FILES src/OgreDDSCodec.cpp)
# endif ()

# if (OGRE_CONFIG_ENABLE_PVRTC)
#   list(APPEND HEADER_FILES include/OgrePVRTCCodec.h)
#   list(APPEND SOURCE_FILES src/OgrePVRTCCodec.cpp)
# endif ()

# if (OGRE_CONFIG_ENABLE_ETC)
#   list(APPEND HEADER_FILES include/OgreETCCodec.h)
#   list(APPEND SOURCE_FILES src/OgreETCCodec.cpp)
# endif ()

# if (OGRE_CONFIG_ENABLE_ASTC)
#   list(APPEND HEADER_FILES include/OgreASTCCodec.h)
#   list(APPEND SOURCE_FILES src/OgreASTCCodec.cpp)
# endif ()

# if (OGRE_CONFIG_ENABLE_ZIP)
#   include_directories("${ZZip_INCLUDE_DIRS}")
#   list(APPEND HEADER_FILES include/OgreZip.h)
#   list(APPEND SOURCE_FILES src/OgreZip.cpp)

#   if(ANDROID)
#     ADD_DEFINITIONS(-DZZIP_OMIT_CONFIG_H)
#     list(APPEND PLATFORM_SOURCE_FILES src/Android/OgreAPKZipArchive.cpp)
#   endif()

#   list(APPEND LIBRARIES "${ZZip_LIBRARIES}")
#   list(APPEND LIBRARIES ZLIB::ZLIB)
# endif ()

# if(OGRE_CONFIG_FILESYSTEM_UNICODE)
#   set_source_files_properties(src/OgreFileSystem.cpp
#     PROPERTIES COMPILE_DEFINITIONS _OGRE_FILESYSTEM_ARCHIVE_UNICODE)
#   set_source_files_properties(src/OgreZip.cpp
#     PROPERTIES COMPILE_DEFINITIONS _OGRE_FILESYSTEM_ARCHIVE_UNICODE)
# endif()

# if(OGRE_PROFILING_REMOTERY_PATH)
#   list(APPEND SOURCE_FILES ${OGRE_PROFILING_REMOTERY_PATH}/Remotery.c)
#   set_source_files_properties(${OGRE_PROFILING_REMOTERY_PATH}/Remotery.c PROPERTIES LANGUAGE CXX)
#   set_source_files_properties(src/OgreProfiler.cpp PROPERTIES COMPILE_DEFINITIONS USE_REMOTERY)
# endif()

# set (TARGET_LINK_FLAGS "")

# # setup OgreMain target

# get_native_precompiled_header(OgreMain)
add_library(Graphics SHARED ${HEADER_FILES} ${SOURCE_FILES} ${DEVICE_HEADER_FILES} ${DEVICE_SOURCE_FILES} ${RENDERER_HEADER_FILES} ${RENDERER_SOURCE_FILES} ${SHADER_HEADER_FILES} ${SHADER_SOURCE_FILES} ${SPRITE_HEADER_FILES} ${SPRITE_SOURCE_FILES})

if(MSVC)
	target_compile_options(Graphics PRIVATE /W4 /WX)
else()
	target_compile_options(Graphics PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
# add_library(OgreMain ${OGRE_LIB_TYPE} ${OgreMain_pch} ${HEADER_FILES} ${SOURCE_FILES} ${PLATFORM_HEADERS} ${PLATFORM_SOURCE_FILES} ${THREAD_HEADER_FILES} ${THREAD_SOURCE_FILES})

# if (WINDOWS_STORE OR WINDOWS_PHONE)
# 	set_target_properties(OgreMain PROPERTIES VS_WINRT_COMPONENT "true")
# endif ()

# generate_export_header(OgreMain 
#     EXPORT_MACRO_NAME _OgreExport
#     NO_EXPORT_MACRO_NAME _OgrePrivate
#     DEPRECATED_MACRO_NAME OGRE_DEPRECATED
#     EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/OgreExports.h)

file (GLOB_RECURSE resources "${CMAKE_CURRENT_SOURCE_DIR}/Resource/*.*")
foreach(resource ${resources})
	get_filename_component(filename ${resource} NAME)
	get_filename_component(dir ${resource} DIRECTORY)
	get_filename_component(dirname ${dir} NAME)

	# set (output "")
 
	# while(NOT ${dirname} STREQUAL resources)
	# 	get_filename_component(path_component ${dir} NAME)

	# 	set (output "${path_component}/${output}")
	# 	get_filename_component(dir ${dir} DIRECTORY)
	# 	get_filename_component(dirname ${dir} NAME)
	# endwhile()

	set(output "${CMAKE_CURRENT_BINARY_DIR}/Resource/${filename}")

	configure_file(${resource} ${output} COPYONLY)

	# add_custom_command(
	# 	COMMENT "Moving updated resource-file '${filename}'"
	# 	OUTPUT ${output}
	# 	DEPENDS ${resource}
	# 	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	# 	${resource}
	# 	${output}
	# )
	# add_custom_target(${filename} ALL DEPENDS ${resource} ${output})
endforeach()

target_link_libraries(Graphics PUBLIC Core)
target_link_libraries(Graphics PUBLIC Lodepng)
target_link_libraries(Graphics PUBLIC ResourceManager)

if (UNIX)
	# set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
	# set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	# set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

	# set(CMAKE_FIND_DEBUG_MODE TRUE)
	# find_package(glfw3 3.3 REQUIRED PATH /usr/lib/x86_64-linux-gnu)
	
	set(OpenGL_GL_PREFERENCE "GLVND")
	find_package(OpenGL REQUIRED)
	set(CMAKE_FIND_DEBUG_MODE FALSE)
	target_link_libraries(Graphics PUBLIC OpenGL::GL)
	target_link_libraries(Graphics PUBLIC Glfw)
	target_link_libraries(Graphics PUBLIC Gl3w)
	target_link_libraries(Graphics PUBLIC glm)
	target_link_libraries(Graphics PUBLIC ${CMAKE_DL_LIBS})
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

target_precompile_headers(Graphics PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Public/GraphicsApiPch.h")

target_include_directories(Graphics PUBLIC
	"${PROJECT_BINARY_DIR}"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Shader>" 
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Resource>"
	"$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>"
	$<INSTALL_INTERFACE:include/CAVE_ENGINE>)
# target_include_directories(OgreMain PUBLIC 
#   "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>" 
#   "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>"
#   $<INSTALL_INTERFACE:include/OGRE>
#   PRIVATE "${OGRE_PROFILING_REMOTERY_PATH}")

# # In visual studio 2010 - 64 bit we get this error: "LINK : fatal error LNK1210: exceeded internal ILK size limit; link with /INCREMENTAL:NO"
# if(WIN32 AND MSVC10 AND CMAKE_CL_64)
#   set_target_properties(OgreMain PROPERTIES 
#                         VERSION ${OGRE_SOVERSION}
#                         LINK_FLAGS "/INCREMENTAL:NO"
#                       )
# else()
#   set_target_properties(OgreMain PROPERTIES VERSION ${OGRE_SOVERSION} SOVERSION ${OGRE_SOVERSION})
# endif()

# if (APPLE)
#   if (APPLE_IOS)
#     set_target_properties(OgreMain PROPERTIES INSTALL_NAME_DIR "OGRE")
#   else()
#     if(OGRE_BUILD_LIBS_AS_FRAMEWORKS)
#       set(OGRE_OSX_BUILD_CONFIGURATION "$ENV{PLATFORM_NAME}/$ENV{CONFIGURATION}")
#       add_custom_command(TARGET OgreMain POST_BUILD
#           COMMAND mkdir ARGS -p ${PROJECT_BINARY_DIR}/lib/${OGRE_OSX_BUILD_CONFIGURATION}/Ogre.framework/Headers/Threading
#           COMMAND ditto
#           ${PROJECT_SOURCE_DIR}/OgreMain/include/Threading/*.h ${PROJECT_BINARY_DIR}/lib/${OGRE_OSX_BUILD_CONFIGURATION}/Ogre.framework/Headers/Threading
#           COMMAND cd ${PROJECT_BINARY_DIR}/lib/${OGRE_OSX_BUILD_CONFIGURATION}/Ogre.framework/Headers
#           )

#       foreach(HEADER_PATH ${THREAD_HEADER_FILES})
#           get_filename_component(HEADER_FILE ${HEADER_PATH} NAME)
#           set(FWK_HEADER_PATH ${PROJECT_BINARY_DIR}/lib/${OGRE_OSX_BUILD_CONFIGURATION}/Ogre.framework/Headers/${HEADER_FILE})
#           add_custom_command(TARGET OgreMain POST_BUILD
#               COMMAND rm -f ${FWK_HEADER_PATH}
#               )
#       endforeach()
#     endif()

#     ogre_config_framework(OgreMain)
#   endif ()

#   if(OGRE_BUILD_LIBS_AS_FRAMEWORKS)
#     # Framework is called 'Ogre'
#     set_target_properties(OgreMain PROPERTIES	OUTPUT_NAME Ogre)
#   endif()
# endif ()
# target_link_libraries(OgreMain PUBLIC ${PLATFORM_LIBS} PRIVATE ${LIBRARIES} ${CMAKE_DL_LIBS})

# # specify a precompiled header to use
# add_native_precompiled_header(OgreMain "${CMAKE_CURRENT_SOURCE_DIR}/src/OgreStableHeaders.h")

# install Engine
# cave_engine_config_lib(Engine TRUE)
# install(FILES ${HEADER_FILES}
#   DESTINATION include/OGRE
# )
# install(FILES ${PLATFORM_HEADERS}
#   DESTINATION include/OGRE/${PLATFORM_HEADER_INSTALL}
# )
# install(FILES ${THREAD_HEADER_FILES}
# 	DESTINATION include/OGRE/Threading
# )