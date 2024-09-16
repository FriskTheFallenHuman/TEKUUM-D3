project( cmdtools CXX )

file(GLOB COMPILER_INCLUDES *.h)
set(COMPILER_SOURCES ${CMAKE_SOURCE_DIR}/idlib/precompiled.cpp)

file(GLOB COMPILER_AAS_INCLUDES aas/*.h)
file(GLOB COMPILER_AAS_SOURCES aas/*.cpp)

file(GLOB COMPILER_DMAP_INCLUDES dmap/*.h)
file(GLOB COMPILER_DMAP_SOURCES dmap/*.cpp)

file(GLOB COMPILER_ROQVQ_INCLUDES roqvq/*.h)
file(GLOB COMPILER_ROQVQ_SOURCES roqvq/*.cpp)

source_group("" FILES ${COMPILER_INCLUDES})
source_group("AAS" FILES ${COMPILER_AAS_INCLUDES})
source_group("AAS" FILES ${COMPILER_AAS_SOURCES})
source_group("DMap" FILES ${COMPILER_DMAP_INCLUDES})
source_group("DMap" FILES ${COMPILER_DMAP_SOURCES})
source_group("RoqVQ" FILES ${COMPILER_ROQVQ_INCLUDES})
source_group("RoqVQ" FILES ${COMPILER_ROQVQ_SOURCES})

set(CMDTOOLS_INCLUDES
	${COMPILER_INCLUDES}
	${COMPILER_AAS_INCLUDES}
	${COMPILER_DMAP_INCLUDES}
	${COMPILER_ROQVQ_INCLUDES})

set(CMDTOOLS_SOURCES
	${COMPILER_SOURCES}
	${COMPILER_AAS_SOURCES}
	${COMPILER_DMAP_SOURCES}
	${COMPILER_ROQVQ_SOURCES})

if( MSVC )
	add_library( ${PROJECT_NAME} ${CMDTOOLS_SOURCES} ${CMDTOOLS_INCLUDES} )
	set_target_properties( ${PROJECT_NAME} PROPERTIES FOLDER "libs" )
else()
	if ( USE_PRECOMPILED_HEADERS )
		set( CMD_PRECOMPILED_SOURCES ${CMDTOOLS_SOURCES} )

		foreach( src_file ${CMD_PRECOMPILED_SOURCES} )
			set_source_files_properties(
				${src_file}
				PROPERTIES
				COMPILE_FLAGS "-include ${CMAKE_CURRENT_SOURCE_DIR}/precompiled.h"
			)
		endforeach()

		# precompiled magic for GCC/clang, adapted from https://gist.github.com/573926
		STRING( TOUPPER "CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}" _flags_var_name )
		SET( _compiler_FLAGS "${${_flags_var_name}} -std=c++${CMAKE_CXX_STANDARD}" )
		GET_DIRECTORY_PROPERTY( _directory_flags INCLUDE_DIRECTORIES )
		FOREACH( item ${_directory_flags} )
			LIST( APPEND _compiler_FLAGS " -I${item}" )
		ENDFOREACH( item )

		GET_DIRECTORY_PROPERTY( _directory_flags DEFINITIONS )
		LIST( APPEND _compiler_FLAGS ${_directory_flags} )

		SEPARATE_ARGUMENTS( _compiler_FLAGS )

		add_custom_target(
			precomp_header_cmdtools ALL
			COMMAND ${CMAKE_CXX_COMPILER} ${_compiler_FLAGS} -x c++-header precompiled.h -o precompiled.h.gch
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
			COMMENT "Creating precompiled.h.gch for cmdtools"
		)
	endif()
	
	add_library( ${PROJECT_NAME} ${CMDTOOLS_SOURCES} ${CMDTOOLS_INCLUDES} )

	if ( USE_PRECOMPILED_HEADERS )
		add_dependencies( ${PROJECT_NAME} precomp_header_cmdtools )

		if( WIN32 )
			set( remove_command "del" )
			set( idlib_dir_slash "${CMAKE_SOURCE_DIR}\\idlib\\" )
		else()
			set( remove_command "rm" )
			set( idlib_dir_slash "${CMAKE_SOURCE_DIR}/idlib/" )
		endif()

		# SRS - delete precompiled header file after executable is compiled: command line build case
		if( CMAKE_GENERATOR MATCHES "Makefiles" OR CMAKE_GENERATOR MATCHES "Ninja" )
			add_custom_target(rm_precomp_header_cmdtools ALL
				COMMAND ${remove_command} "precompiled.h.gch"
				WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
				COMMENT "remove precompiled.h.gch"
			)
			add_dependencies( rm_precomp_header_cmdtools ${PROJECT_NAME} )

		# SRS - delete precompiled header files after executable is compiled: IDE build case (e.g. Xcode)
		else()
			add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD
				# SRS - added wildcards to remove tmp files from cmake ZERO_CHECK regeneration
				COMMAND ${remove_command} "${idlib_dir_slash}precompiled.h*.gch*"
				COMMAND ${remove_command} "precompiled.h*.gch*"
				WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
				COMMENT "remove idlib/precompiled.h.gch and precompiled.h.gch"
			)
		endif()
	endif()
endif()

target_compile_definitions( ${PROJECT_NAME} PRIVATE ID_ALLOW_CMD_TOOLS )
target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/idlib ${CMAKE_CURRENT_SOURCE_DIR} )