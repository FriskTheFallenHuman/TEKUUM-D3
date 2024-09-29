project( MayaImport CXX )

file( GLOB MAYA_INCLUDES *.h )
file( GLOB MAYA_SOURCES *.cpp )

set(MAYA_INCLUDES_ALL
	${MAYA_INCLUDES})

set(MAYA_SOURCES_ALL
	${MAYA_SOURCES})

source_group( "" FILES ${MAYA_INCLUDES} )
source_group( "" FILES ${MAYA_SOURCES} )

if( MSVC )
	add_library( ${PROJECT_NAME} SHARED ${MAYA_SOURCES_ALL} ${MAYA_INCLUDES_ALL} )
	target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/idlib ${CMAKE_SOURCE_DIR}/libs/MayaSDKDevKit/include )
	add_dependencies( ${PROJECT_NAME} idlib external MayaDevKit )
	target_link_directories( ${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/libs/MayaSDKDevKit/lib" )
	target_link_libraries( ${PROJECT_NAME} idlib external Foundation OpenMaya OpenMayaAnim )
	set_target_properties( ${PROJECT_NAME} PROPERTIES FOLDER "dlls" )

	#GK: Make sure the name is right
	set_target_properties( ${PROJECT_NAME} PROPERTIES OUTPUT_NAME "mayaimport" )
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
		set( MAYA_PRECOMPILED_SOURCES ${MAYA_SOURCES_ALL} )

		foreach( src_file ${MAYA_PRECOMPILED_SOURCES} )
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
		add_custom_target( precomp_header_mayaimport ALL
				COMMAND ${CMAKE_CXX_COMPILER} ${_compiler_FLAGS} -x c++-header ../idlib/precompiled.h -o ../idlib/precompiled.h.gch
				WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
				COMMENT "Creating ../idlib/precompiled.h.gch for game" )
	endif()

	target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/idlib )

	add_library( ${PROJECT_NAME} SHARED ${MAYA_SOURCES_ALL} ${MAYA_INCLUDES_ALL} )

	if ( USE_PRECOMPILED_HEADERS )
		# make sure precompiled header is created before executable is compiled
		add_dependencies( ${PROJECT_NAME} precomp_header_mayaimport )
	endif()
	target_link_libraries( ${PROJECT_NAME} idlib )

	#GK: Make sure the name is right
	set_target_properties( ${PROJECT_NAME} PROPERTIES OUTPUT_NAME "mayaimport" )
	set_target_properties( ${PROJECT_NAME} PROPERTIES PREFIX "" )
	if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
		set_target_properties( ${PROJECT_NAME} PROPERTIES SUFFIX "x86_64.so" )
	else()
		set_target_properties( ${PROJECT_NAME} PROPERTIES SUFFIX "x86.so" )
	endif()
endif()

if ( MSVC )
	set_target_properties( ${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS "ID_MAYA_IMPORT;ID_MAYA_IMPORT_API=__declspec(dllexport)" )
else()
	set_target_properties( ${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS "ID_MAYA_IMPORT;ID_MAYA_IMPORT_API=__attribute__ ((visibility (\"default\")))" )
endif()

set_target_properties( ${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY $<1:>${CMAKE_SOURCE_DIR}/.. LIBRARY_OUTPUT_DIRECTORY $<1:>${CMAKE_SOURCE_DIR}/.. )