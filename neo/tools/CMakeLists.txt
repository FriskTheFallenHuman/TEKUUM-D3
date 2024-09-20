project( tools CXX )

set(TOOLS_INCLUDES edit_public.h)
set(TOOLS_SOURCES ${CMAKE_SOURCE_DIR}/idlib/precompiled.cpp)

file(GLOB TOOLS_COMAFX_INCLUDES comafx/*.h)
file(GLOB TOOLS_COMAFX_SOURCES comafx/*.cpp)

file(GLOB TOOLS_COMMON_INCLUDES common/*.h)
file(GLOB TOOLS_COMMON_SOURCES common/*.cpp)

file(GLOB TOOLS_COMMON_PROPTREE_INCLUDES common/PropTree/*.h)
file(GLOB TOOLS_COMMON_PROPTREE_SOURCES common/PropTree/*.cpp)

file(GLOB EDITOR_AF_INCLUDES af/*.h)
file(GLOB EDITOR_AF_SOURCES af/*.cpp)

file(GLOB EDITOR_DECL_INCLUDES decl/*.h)
file(GLOB EDITOR_DECL_SOURCES decl/*.cpp)

file(GLOB EDITOR_DEBUGGER_INCLUDES debugger/*.h)
file(GLOB EDITOR_DEBUGGER_SOURCES debugger/*.cpp)

file(GLOB EDITOR_GUIED_INCLUDES guied/*.h)
file(GLOB EDITOR_GUIED_SOURCES guied/*.cpp)

file(GLOB EDITOR_MATERIALEDITOR_INCLUDES materialeditor/*.h)
file(GLOB EDITOR_MATERIALEDITOR_SOURCES materialeditor/*.cpp)

file(GLOB EDITOR_PARTICLE_INCLUDES particle/*.h)
file(GLOB EDITOR_PARTICLE_SOURCES particle/*.cpp)

file(GLOB EDITOR_PDA_INCLUDES pda/*.h)
file(GLOB EDITOR_PDA_SOURCES pda/*.cpp)

file(GLOB EDITOR_RADIANT_INCLUDES radiant/*.h)
file(GLOB EDITOR_RADIANT_SOURCES radiant/*.cpp)

file(GLOB EDITOR_SCRIPT_INCLUDES script/*.h)
file(GLOB EDITOR_SCRIPT_SOURCES script/*.cpp)

file(GLOB EDITOR_SOUND_INCLUDES sound/*.h)
file(GLOB EDITOR_SOUND_SOURCES sound/*.cpp)

source_group("" FILES ${TOOLS_INCLUDES})
source_group("ComAfx" FILES ${TOOLS_COMAFX_INCLUDES})
source_group("ComAfx" FILES ${TOOLS_COMAFX_SOURCES})
source_group("Common" FILES ${TOOLS_COMMON_INCLUDES})
source_group("Common" FILES ${TOOLS_COMMON_SOURCES})
source_group("Common\\PropTree" FILES ${TOOLS_COMMON_PROPTREE_INCLUDES})
source_group("Common\\PropTree" FILES ${TOOLS_COMMON_PROPTREE_SOURCES})
source_group("AF" FILES ${EDITOR_AF_INCLUDES})
source_group("AF" FILES ${EDITOR_AF_SOURCES})
source_group("Debugger" FILES ${EDITOR_DEBUGGER_INCLUDES})
source_group("Debugger" FILES ${EDITOR_DEBUGGER_SOURCES})
source_group("Decl" FILES ${EDITOR_DECL_INCLUDES})
source_group("Decl" FILES ${EDITOR_DECL_SOURCES})
source_group("GuiEd" FILES ${EDITOR_GUIED_INCLUDES})
source_group("GuiEd" FILES ${EDITOR_GUIED_SOURCES})
source_group("MaterialEditor" FILES ${EDITOR_MATERIALEDITOR_INCLUDES})
source_group("MaterialEditor" FILES ${EDITOR_MATERIALEDITOR_SOURCES})
source_group("Particle" FILES ${EDITOR_PARTICLE_INCLUDES})
source_group("Particle" FILES ${EDITOR_PARTICLE_SOURCES})
source_group("PDA" FILES ${EDITOR_PDA_INCLUDES})
source_group("PDA" FILES ${EDITOR_PDA_SOURCES})
source_group("Radiant" FILES ${EDITOR_RADIANT_INCLUDES})
source_group("Radiant" FILES ${EDITOR_RADIANT_SOURCES})
source_group("Script" FILES ${EDITOR_SCRIPT_INCLUDES})
source_group("Script" FILES ${EDITOR_SCRIPT_SOURCES})
source_group("Sound" FILES ${EDITOR_SOUND_INCLUDES})
source_group("Sound" FILES ${EDITOR_SOUND_SOURCES})
	
set(TOOL_INCLUDES
	${TOOLS_INCLUDES}
	${TOOLS_COMAFX_INCLUDES}
	${TOOLS_COMMON_INCLUDES}
	${TOOLS_COMMON_PROPTREE_INCLUDES}
	${EDITOR_AF_INCLUDES}
	${EDITOR_DEBUGGER_INCLUDES}
	${EDITOR_DECL_INCLUDES}
	${EDITOR_GUIED_INCLUDES}
	${EDITOR_MATERIALEDITOR_INCLUDES}
	${EDITOR_PARTICLE_INCLUDES}
	${EDITOR_PDA_INCLUDES}
	${EDITOR_RADIANT_INCLUDES}
	${EDITOR_SCRIPT_INCLUDES}
	${EDITOR_SOUND_INCLUDES})

set(TOOL_SOURCES
	${TOOLS_COMAFX_SOURCES}
	${TOOLS_COMMON_SOURCES}
	${TOOLS_COMMON_PROPTREE_SOURCES}
	${EDITOR_AF_SOURCES}
	${EDITOR_DEBUGGER_SOURCES}
	${EDITOR_DECL_SOURCES}
	${EDITOR_GUIED_SOURCES}
	${EDITOR_MATERIALEDITOR_SOURCES}
	${EDITOR_PARTICLE_SOURCES}
	${EDITOR_PDA_SOURCES}
	${EDITOR_RADIANT_SOURCES}
	${EDITOR_SCRIPT_SOURCES}
	${EDITOR_SOUND_SOURCES})

if( MSVC )
	if( USE_PRECOMPILED_HEADERS )
		set( TOOLS_PRECOMPILED_SOURCES ${TOOL_SOURCES} )

		set_source_files_properties(
			${TEKUUM_PRECOMPILED_SOURCES}
			PROPERTIES
			COMPILE_FLAGS "/Yuprecompiled.h"
			OBJECT_DEPENDS "precompiled.pch"
		)

		set_source_files_properties(
			${CMAKE_SOURCE_DIR}/idlib/precompiled.cpp
			PROPERTIES
			COMPILE_FLAGS "/Ycprecompiled.h"
			OBJECT_OUTPUTS "precompiled.pch"
		)
	endif()

	add_library( ${PROJECT_NAME} ${TOOL_SOURCES} ${TOOL_INCLUDES} )

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

target_compile_definitions( ${PROJECT_NAME} PRIVATE ID_ALLOW_TOOLS )
target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/idlib ${CMAKE_CURRENT_SOURCE_DIR} )