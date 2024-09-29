project( Game CXX )

file( GLOB GAME_INCLUDES *.h )
file( GLOB GAME_SOURCES *.cpp )
file( GLOB GAME_AI_INCLUDES ai/*.h )
file( GLOB GAME_AI_SOURCES ai/*.cpp )
file( GLOB GAME_ANIM_INCLUDES anim/*.h )
file( GLOB GAME_ANIM_SOURCES anim/*.cpp)
if ( NOT MAYA_IMPORT )
	list( REMOVE_ITEM GAME_ANIM_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/anim/Anim_Import.cpp )
endif()
file( GLOB GAME_GAMESYS_INCLUDES gamesys/*.h )
set( GAME_GAMESYS_SOURCES
	gamesys/Class.cpp
	gamesys/Event.cpp
	gamesys/SaveGame.cpp
	gamesys/SysCmds.cpp
	gamesys/SysCvar.cpp
	gamesys/TypeInfo.cpp )
file( GLOB GAME_PHYSICS_INCLUDES physics/*.h )
file( GLOB GAME_PHYSICS_SOURCES physics/*.cpp )
file( GLOB GAME_SCRIPT_INCLUDES script/*.h )
file( GLOB GAME_SCRIPT_SOURCES script/*.cpp )

set( GAME_INCLUDES_ALL
	${GAME_INCLUDES}
	${GAME_AI_INCLUDES}
	${GAME_ANIM_INCLUDES}
	${GAME_GAMESYS_INCLUDES}
	${GAME_PHYSICS_INCLUDES}
	${GAME_SCRIPT_INCLUDES} )

set( GAME_SOURCES_ALL
	${GAME_SOURCES}
	${GAME_AI_SOURCES}
	${GAME_ANIM_SOURCES}
	${GAME_GAMESYS_SOURCES}
	${GAME_PHYSICS_SOURCES}
	${GAME_SCRIPT_SOURCES} )

source_group( "" FILES ${GAME_INCLUDES} )
source_group( "" FILES ${GAME_SOURCES} )
source_group( "AI" FILES ${GAME_AI_INCLUDES} )
source_group( "AI" FILES ${GAME_AI_SOURCES} )
source_group( "Anim" FILES ${GAME_ANIM_INCLUDES} )
source_group( "Anim" FILES ${GAME_ANIM_SOURCES} )
source_group( "GameSys" FILES ${GAME_GAMESYS_INCLUDES} )
source_group( "GameSys" FILES ${GAME_GAMESYS_SOURCES} )
source_group( "Physics" FILES ${GAME_PHYSICS_INCLUDES} )
source_group( "Physics" FILES ${GAME_PHYSICS_SOURCES} )
source_group( "Script" FILES ${GAME_SCRIPT_INCLUDES} )
source_group( "Script" FILES ${GAME_SCRIPT_SOURCES} )

if( MSVC )
	add_library( ${PROJECT_NAME} SHARED ${GAME_SOURCES_ALL} ${GAME_INCLUDES_ALL} )
	target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/idlib )
	add_dependencies( ${PROJECT_NAME} idlib )
	target_link_libraries( ${PROJECT_NAME} idlib )
	set_target_properties( ${PROJECT_NAME} PROPERTIES FOLDER "dlls" )

	#GK: Make sure the name is right
	set_target_properties( ${PROJECT_NAME} PROPERTIES OUTPUT_NAME "game" )
	set_target_properties( ${PROJECT_NAME} PROPERTIES PREFIX "" )
	if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
		set_target_properties( ${PROJECT_NAME} PROPERTIES SUFFIX "x86_64.dll" )
	else()
		set_target_properties( ${PROJECT_NAME} PROPERTIES SUFFIX "x86.dll" )
	endif()
else()
	add_compile_options( -fPIC )
	add_compile_options( -ldl )
	if( USE_PRECOMPILED_HEADERS ) 	
		set( GAME_PRECOMPILED_SOURCES ${GAME_SOURCES_ALL} )

		foreach( src_file ${GAME_PRECOMPILED_SOURCES} )
			set_source_files_properties(
				${src_file}
				PROPERTIES
				COMPILE_FLAGS "-include ${CMAKE_CURRENT_SOURCE_DIR}/../idlib/precompiled.h" )
		endforeach()

		# precompiled magic for GCC/clang, adapted from https://gist.github.com/573926
		STRING( TOUPPER "CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}" _flags_var_name )
		SET( _compiler_FLAGS ${${_flags_var_name}} )
		GET_DIRECTORY_PROPERTY( _directory_flags INCLUDE_DIRECTORIES )
		FOREACH( item ${_directory_flags} )
			LIST( APPEND _compiler_FLAGS " -I${item}" )
		ENDFOREACH( item )
	endif()

	#GK: fix for mingw and recent versions of cmake
	cmake_policy( SET CMP0059 NEW )

	GET_DIRECTORY_PROPERTY( _directory_flags DEFINITIONS )
	LIST( APPEND _compiler_FLAGS ${_directory_flags} )
	SEPARATE_ARGUMENTS( _compiler_FLAGS )
	
	if( USE_PRECOMPILED_HEADERS )
		# we need to recreate the precompiled header for Kroom3
		# (i.e. can't use the one created for idlib before)
		# because some definitions (e.g. -D__IDLIB__ -D__DOOM_DLL__) differ
		add_custom_target( precomp_header_game ALL
				COMMAND ${CMAKE_CXX_COMPILER} ${_compiler_FLAGS} -x c++-header ../idlib/precompiled.h -o ../idlib/precompiled.h.gch
				WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
				COMMENT "Creating ../idlib/precompiled.h.gch for game" )
	endif()

	target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/idlib )

	add_library( ${PROJECT_NAME} SHARED ${GAME_SOURCES_ALL} ${GAME_INCLUDES_ALL} )

	if ( USE_PRECOMPILED_HEADERS )
		# make sure precompiled header is created before executable is compiled
		add_dependencies( ${PROJECT_NAME} precomp_header_game )
	endif()
	target_link_libraries( ${PROJECT_NAME} idlib )

	#GK: Make sure the name is right
	set_target_properties( ${PROJECT_NAME} PROPERTIES OUTPUT_NAME "game" )
	set_target_properties( ${PROJECT_NAME} PROPERTIES PREFIX "" )
	if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
		set_target_properties( ${PROJECT_NAME} PROPERTIES SUFFIX "x86_64.so" )
	else()
		set_target_properties( ${PROJECT_NAME} PROPERTIES SUFFIX "x86.so" )
	endif()
endif()

target_compile_definitions( ${PROJECT_NAME} PRIVATE GAME_DLL )

set_target_properties( ${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY $<1:>${CMAKE_SOURCE_DIR}/../base LIBRARY_OUTPUT_DIRECTORY $<1:>${CMAKE_SOURCE_DIR}/../base )