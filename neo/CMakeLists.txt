cmake_minimum_required(VERSION 2.25)

# Check if vcpkg exist in our source folder
if ( EXISTS "${CMAKE_SOURCE_DIR}/../vcpkg/scripts/buildsystems/vcpkg.cmake" )
	set( CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/../vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file" )
else()
	message( FATAL_ERROR "Vcpkg wasn't not found, aborting, make sure to git clone this with all submodules!")
endif()

set_property( GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER  "" )
set_property( GLOBAL PROPERTY USE_FOLDERS ON )

set( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules" )

#Krispy: Use 2 configurations Debug,Release
if( NOT CMAKE_CONFIGURATION_TYPES ) 
	set( CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE ) 
endif()

project(kroom3 C CXX)

include( CheckCXXCompilerFlag )
include( GNUInstallDirs OPTIONAL RESULT_VARIABLE GNUINSTALLDIRS )

option(ENGINE "Build the main executable." ON)
option(DEDICATED "Build the dedicated server executable." OFF)
option(USE_CMDLINE_TOOLS "Compile the built-in command line based tools" ON)
option(USE_MFC_TOOLS "Compile the built-in MFC based tools" ON)
option(MAYA_IMPORT "Build the include maya import tool (Requires MayaSDK)." OFF)
option(COMPILE_COMMANDS "Generate compile_commands.json" OFF)
option(USE_PRECOMPILED_HEADERS "Use precompiled headers during build" ON)
option(ONATIVE "Optimize for the host CPU" OFF)
option(REPRODUCIBLE_BUILD "Replace __DATE__ and __TIME__ by hardcoded values for reproducible builds." OFF)
option(LINUX_RELEASE_BINS "Set RPATH to \$ORIGIN/libs/ for Linux binary releases" OFF)
option(FFMPEG "Use FMPEG to render Bink videos" OFF)
option(BINKDEC "Use included libbinkdec to render Bink videos" OFF)
option(RETAIL "Strip certain developer features and cheats from shipping builds" OFF)
option(FREETYPE "Enable Freetype support." OFF)
option(USE_VULKAN "Use Vulkan instead of OpenGL" OFF)
option(SPIRV_SHADERC "Compile SPIR-V shader byte code using shaderc instead of using Glslang directly" OFF)
option(USE_MOLTENVK "Use MoltenVK library directly instead of Vulkan loader on macOS" OFF)
if( NOT MSVC ) # GCC/clang or compatible, hopefully
	option( FORCE_COLOR_OUTPUT	"Always produce ANSI-colored compiler warnings/errors (GCC/Clang only; esp. useful with ninja)." OFF )
	option( ASAN					"Enable GCC/Clang Adress Sanitizer (ASan)" OFF) # TODO: MSVC might also support this, somehow?
	option( UBSAN					"Enable GCC/Clang Undefined Behavior Sanitizer (UBSan), implies HARDLINK_GAME" OFF )

	#if( UBSAN AND NOT HARDLINK_GAME )
	#	message( STATUS "UBSAN requires linking the gamecode into the executable, will enable HARDLINK_GAME" )
	#	set( HARDLINK_GAME ON )
	#endif()
endif()

if ( LINUX_RELEASE_BINS )
	message( STATUS "Setting RPATH to \$ORIGIN/libs/ so you can put dependencies in there" )
	set( CMAKE_SKIP_RPATH OFF CACHE BOOL "Skip RPATH" FORCE )
	set( CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE )
	set( CMAKE_BUILD_WITH_INSTALL_RPATH TRUE )
	set( CMAKE_INSTALL_RPATH "$ORIGIN/libs" )
	set( UNIX_FLAGS "${UNIX_FLAGS} -Wl,--large-address-aware,-z,origin" ) # not sure this is even needed, but shouldn't hurt
else()
	set( CMAKE_SKIP_RPATH ON CACHE BOOL "Skip RPATH" FORCE )
endif()

if( FFMPEG AND BINKDEC )
	message( FATAL_ERROR "Only one of FFMPEG and BINKDEC (or neither) can be enabled at a time" )
endif()

# Not supported at the moment
if ( WIN32 AND USE_VULKAN )
	set( USE_MFC_TOOLS OFF )
endif()

# MayaImport currently doesn't work in Linux/Mac
if ( NOT WIN32 OR NOT USE_MFC_TOOLS )
	set( MAYA_IMPORT OFF )
endif()

# Define our cpu variable
include( CPUDefine )

if( UNIX )
	# SRS - Disable precompiled headers when cross-compiling on OSX, e.g. when creating universal binaries
	#     - Also disable precompiled headers when using the Ninja Multi-Config generator on linux or macOS
	if ( ( CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "x86_64" AND CMAKE_OSX_ARCHITECTURES MATCHES "arm64" ) OR
	   ( CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "arm64"  AND CMAKE_OSX_ARCHITECTURES MATCHES "x86_64" ) OR
	   ( CMAKE_GENERATOR MATCHES "Ninja Multi-Config" ) )
		set( USE_PRECOMPILED_HEADERS OFF )
	endif()

	# SRS - For single-config dev builds, define NO_MULTI_CONFIG to search for basepath up 1 dir level vs 2
	if( ( CMAKE_GENERATOR MATCHES "Makefiles" ) OR
	   ( CMAKE_GENERATOR MATCHES "Ninja" AND NOT CMAKE_GENERATOR MATCHES "Multi-Config" ) )
		add_definitions( -DNO_MULTI_CONFIG )
	endif()
endif()

if( COMPILE_COMMANDS )
	set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
endif()

set( bindir		"${CMAKE_INSTALL_FULL_BINDIR}" )
set( libdir		"${CMAKE_INSTALL_FULL_LIBDIR}/${PROJECT_NAME}" )
set( datadir	"${CMAKE_INSTALL_FULL_DATADIR}/${PROJECT_NAME}" )

if( NOT MSVC )
	message( STATUS "Building ${CMAKE_BUILD_TYPE} for ${os}-${cpu}" )
endif()

if( NOT WIN32 )
	message( STATUS "The install target will use the following directories:" )
	message( STATUS "  Binary directory:  ${bindir}" )
	message( STATUS "  Library directory: ${libdir}" )
	message( STATUS "  Data directory:    ${datadir}" )
endif()

if( CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID STREQUAL "Clang" )
	add_definitions(-pipe)
	#add_definitions(-Wall)
	add_definitions(-Werror=format-security)
	add_definitions(-Werror=format)
	add_definitions(-Wno-format-zero-length)
	add_definitions(-Wno-nonnull)

	if( FORCE_COLORED_OUTPUT )
		# Enforced colored output
		if ( CMAKE_COMPILER_IS_GNUCC )
			add_compile_options( -fdiagnostics-color=always )
		elseif( CMAKE_C_COMPILER_ID MATCHES "Clang" )
			add_compile_options( -fcolor-diagnostics )
		endif ()
	endif()
	
	# No Tools for linux, sorry.
	set( USE_MFC_TOOLS OFF )
	
	if( NOT WIN32 )
		# libbacktrace support - TODO: might work with MinGW? we don't have a crash handler for win32 though..
		include( CheckCSourceCompiles )
		set( CMAKE_REQUIRED_LIBRARIES backtrace )
		check_c_source_compiles( "#include <backtrace.h>
		int main() { backtrace_create_state(NULL, 0, NULL, NULL); return 0; }" HAVE_LIBBACKTRACE )
		unset( CMAKE_REQUIRED_LIBRARIES )

		if( HAVE_LIBBACKTRACE )
			set( sys_libs ${sys_libs} backtrace )
			add_definitions( -DD3_HAVE_LIBBACKTRACE )
			message( STATUS "Using libbacktrace" )
		else()
			message( WARNING "libbacktrace wasn't found. It's not required but recommended, because it provides useful backtraces if engine/game crashes" )
		endif()
	endif() # NOT WIN32

	# Compiler check (needs -std=c++17 flag)
	include( CheckCXXCompilerFlag )
	CHECK_CXX_COMPILER_FLAG( "-std=c++17" COMPILER_SUPPORTS_CXX17 )
	if( COMPILER_SUPPORTS_CXX17 )
		set( CMAKE_CXX_STANDARD 17 )
		set( CMAKE_CXX_STANDARD_REQUIRED ON )
		set( CMAKE_CXX_EXTENSIONS OFF )
	else()
		message( FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++17 support. Please use a different C++ compiler." )
	endif()

	if( WIN32 )
		# require msvcr70.dll or newer for _aligned_malloc etc
		# I think it is from Visual C++ .NET 2002, so it should be available on any remotely modern system.
		add_definitions( -D__MSVCRT_VERSION__=0x0700 )

		#find_package( DirectX REQUIRED )
		include_directories( libs/mingw-hacks )
		#include_directories( ${DirectX_INCLUDE_DIR} )
		#add_definitions( -DUSE_XINPUT )
	endif()

	add_compile_options( -Wno-pragmas -Wno-unused-variable -Wno-switch -Wno-unused-value -Winvalid-pch -Wno-multichar )
	if( CMAKE_C_COMPILER_ID MATCHES "Clang" )
		# add clang-specific settings for warnings (the second one make sure clang doesn't complain
		# about unknown -W flags, like -Wno-unused-but-set-variable)
		# SRS - Add -Wno-expansion-to-defined, Wno-nullability-completeness and -Wno-shorten-64-to-32 to list of warning settings
		add_definitions( -Wno-local-type-template-args -Wno-unknown-warning-option -Wno-inline-new-delete -Wno-switch-enum -Wno-expansion-to-defined -Wno-nullability-completeness -Wno-shorten-64-to-32 )
	endif()

	if( NOT CMAKE_CROSSCOMPILING AND ONATIVE )
		if( CMAKE_SYSTEM_PROCESSOR MATCHES "((powerpc|ppc)64le)|(mips64)" )
			add_definitions( -mcpu=native )
		else()
			add_definitions( -march=native )
		endif()
	endif()

	add_compile_options( -fno-strict-aliasing )
	
	# dear idiot compilers, don't fuck up math code with useless FMA "optimizations"
	# (https://gcc.gnu.org/bugzilla/show_bug.cgi?id=100839)
	CHECK_CXX_COMPILER_FLAG( "-ffp-contract=off" cxx_has_fp-contract )
	if( cxx_has_fp-contract )
		add_compile_options( -ffp-contract=off )
	endif()
	
	if( ASAN )
		# if this doesn't work, ASan might not be available on your platform, don't set ASAN then..
		add_compile_options( -fsanitize=address )
		set( sys_libs ${sys_libs} -fsanitize=address )
	endif()

	if( UBSAN )
		# if this doesn't work, UBSan might not be available on your platform, don't set UBSAN then..
		add_compile_options( -fsanitize=undefined )
		set( sys_libs ${sys_libs} -fsanitize=undefined )
	endif()
	
	CHECK_CXX_COMPILER_FLAG( "-fvisibility=hidden" cxx_has_fvisibility )
	if( NOT cxx_has_fvisibility )
		message( FATAL_ERROR "Compiler does not support -fvisibility" )
	endif()
	add_compile_options( -fvisibility=hidden )
	
	# TODO fix these warnings
	add_compile_options( -Wno-sign-compare )
	add_compile_options( -Wno-switch )
	
	CHECK_CXX_COMPILER_FLAG( "-Woverloaded-virtual" cxx_has_Woverload_virtual )
	if( cxx_has_Woverload_virtual )
		set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Woverloaded-virtual" )
	endif()
	
	# shut up about using memcpy() on classes, in the cases doom3 uses it it seems to be fine
	CHECK_CXX_COMPILER_FLAG( "-Wno-class-memaccess" cxx_has_Wno-class-memaccess )
	if( cxx_has_Wno-class-memaccess )
		set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-class-memaccess" )
	endif()

	# ignore warnings about variables named "requires" for now (in C++20 it's a keyword,
	#  but currently we don't even use C++11 features)
	CHECK_CXX_COMPILER_FLAG( "-Wno-c++20-compat" cxx_has_Wno-cpp20-compat )
	if( cxx_has_Wno-cpp20-compat )
		set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++20-compat" )
	endif()
	
	if ( NOT MSVC )
		find_package( OpenSSL REQUIRED )
	endif()

	set( SYS_LIBS pthread )

	if( WIN32 )
		set( UNIX_FLAGS "${UNIX_FLAGS} -static-libgcc -static-libstdc++ -Wl, --large-address-aware --stack,16777216" )
	elseif( os STREQUAL "linux" )
		set( SYS_LIBS ${SYS_LIBS} dl OpenSSL::SSL OpenSSL::Crypto rt )
	elseif( os STREQUAL "freebsd" )
		find_library( EXECINFO_LIBRARIES execinfo /usr/lib /usr/local/lib )
		set( SYS_LIBS ${SYS_LIBS} OpenSSL::SSL OpenSSL::Crypto rt ${EXECINFO_LIBRARIES} )
	endif()

	set( CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG -Og -rdynamic -ggdb -fno-strict-aliasing ${my_warn_flags}" )
	if( RETAIL )
		set( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s -O3 -DNDEBUG -ffast-math -fno-strict-aliasing -Werror -Wno-error=invalid-pch -Wno-error=invalid-offsetof ${my_warn_flags}" )
	else()
		set( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -DNDEBUG -ffast-math -fno-unsafe-math-optimizations -fomit-frame-pointer -fno-strict-aliasing ${my_warn_flags}" )
	endif()

	set( CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG} )
	set( CMAKE_CXX_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE} )
	
	set( CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} ${UNIX_FLAGS}" )
	set( CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} ${UNIX_FLAGS}" )
elseif( MSVC )
	# Multiprocessor support for compiling
	# /MP

	# /MT  use multithreaded, static runtime library
	#      (=> binary won't need MSVCP100D.dll or similar)
	# /MTd the same with debug symbols
	# /MD and /MDd - the same with dynamic runtime lib
	#     (=> MSVCP100D.dll or similar is needed to run binary)

	# Inline Function Expansion: Any Suitable (/Ob2)
	# "/Ob2",

	# enable Intrinsic Functions
	# "/Oi",

	# Omit Frame Pointers
	# "/Oy",

	#GK: Fix performance isse on classic doom for the debug build
	set( CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /D _DEBUG /MP /Ob2 /Oi /Oy /MDd /std:c17" )
	set( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MP /Oi /Oy /MD /std:c17" )

	set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MP /Ob2 /Oi /Oy /MDd /std:c++17 /std:c17 /W3" )
	set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MP /Oi /Oy /MD /std:c++17 /std:c17 /W3 /sdl" )

	# RB: without /SAFESEH:NO we can't link against ffmpeg libs and VS2013 or we have to build our own
	# libs for newer compilers
	set( CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /SAFESEH:NO /DEBUG:FULL /STACK:16777216" )
	set( CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /SAFESEH:NO /STACK:16777216 /OPT:REF /OPT:ICF" )
	
	# Ignore warnings for Dedicate server build
	set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ignore:4286 /ignore:4217" )
	set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /ignore:4286 /ignore:4217" )
	set( CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} /ignore:4286 /ignore:4217" )
	
	add_definitions( -D_ALLOW_KEYWORD_MACROS ) # because of the "#define private public" and "#define protected public" in TypeInfo.cpp

	add_definitions( -DWIN32
					-DNOMINMAX
					-D_WINDOWS
					-D_CRT_SECURE_NO_DEPRECATE
					-D_CRT_NONSTDC_NO_DEPRECATE
					-D_CRT_SECURE_NO_WARNINGS
					-D_WINSOCK_DEPRECATED_NO_WARNINGS
					-D_MBCS )

	set(SYS_LIBS
		${SYS_LIBS}
		dbghelp
		iphlpapi
		winmm
		ws2_32)

	#if( WINDOWS10 ) # Windows RT
	#	add_definitions( -DUSE_WINRT )
	#endif()

	if( NOT CMAKE_CL_64 )
		add_definitions( -D_USE_32BIT_TIME_T )
	endif()
	
	if ( MAYA_IMPORT )
		include( ExternalProject )

		# Locate and download MayaSDK, change MAYASDK_URL to one of the urls listed here: https://aps.autodesk.com/developer/overview/maya
		# ( Rigth click in anyone > Copy Url)
		set( MAYASDK_URL "https://autodesk-adn-transfer.s3-us-west-2.amazonaws.com/ADN+Extranet/M%26E/Maya/devkit+2019/Autodesk_Maya_2019_3_Update_DEVKIT_Windows.zip" CACHE STRING "" )
	
		# The downloaded Hash & MD5 of the MAYASDK_URL download
		# These ones are from 2019 make sure to generate one for newer releases!
		set( MAYASDK_HASH "2332C6C79B8C7974A5645523E395962BBB19901289AAAEE129D3BF2B630829C7" CACHE STRING "" )
		set( MAYASDK_MD5 "6BC28A0047709DEECC3DBB0D4A0B4D14" CACHE STRING "" )
	
		ExternalProject_Add( MayaDevKit
			URL ${MAYASDK_URL}
			URL_HASH SHA256=${MAYASDK_HASH}
			URL_MD5 ${MAYASDK_MD5}
			DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
			SOURCE_DIR ${CMAKE_SOURCE_DIR}/libs/MayaSDKDevKit
			CONFIGURE_COMMAND ""
			BUILD_COMMAND ""
			INSTALL_COMMAND ""
		)

		add_definitions( -DID_MAYA_IMPORT_TOOL )

		set_target_properties( MayaDevKit PROPERTIES FOLDER "libs" )
	endif()

	if( USE_MFC_TOOLS )
		# This is required for tools on windows.
		find_package( MFC )
		if( NOT MFC_FOUND )
			message( WARNING "MFC ('Microsoft Foundation Classes for C++') couldn't be found, but is needed for TOOLS!" )
			message( WARNING "If you're using VS2013, you'll also need the 'Multibyte MFC Library for Visual Studio 2013': https://www.microsoft.com/en-us/download/details.aspx?id=40770 (VS2015 and 2017 include that in the default MFC package)" )
		else()
			set( ALLOWRESOURCES ON )

			add_definitions( -D_AFXDLL )
			add_definitions( -DID_ALLOW_TOOLS )

			# 0: Use Standard Windows Libraries
			# 1: Use MFC in a Static Library
			# 2: Use MFC in a Shared DLL
			set( CMAKE_MFC_FLAG 2 )
		endif()
	endif()

	if ( MSVC_VERSION GREATER 1919 AND NOT USE_VULKAN )
		add_definitions( -D_ITERATOR_DEBUG_LEVEL=1 )
	endif()
endif()

# CMake only used for desktop builds
add_definitions( -DID_PC )

if( REPRODUCIBLE_BUILD )
	# don't use __DATE__ and __TIME__ macros so builds are reproducible
	add_definitions( -DID_REPRODUCIBLE_BUILD )
endif()

if ( USE_INTRINSICS_SSE )
	add_definitions( -DUSE_INTRINSICS_SSE )
endif()

if( RETAIL )
	set( REPRODUCIBLE_BUILD ON )
	add_definitions( -DID_RETAIL )
endif()

# Nicely tied our thirdparty libraries here
set( SYS_THIRDPARTY )

find_package( Stb REQUIRED )
include_directories( ${Stb_INCLUDE_DIR} PUBLIC )

find_package( OpenAL REQUIRED )
include_directories( ${OPENAL_INCLUDE_DIR} PUBLIC )
list( APPEND SYS_THIRDPARTY ${OPENAL_LIBRARY} )

if( UNIX )
	find_package( SDL2 REQUIRED )
	include_directories( ${SDL2_INCLUDE_DIR} )
	add_definitions( -DUSE_SDL )
	list( APPEND SYS_THIRDPARTY SDL2::SDL2 )
endif()

if( NOT USE_VULKAN )
	find_package( OpenGL REQUIRED )
	include_directories( ${OPENGL_INCLUDE_DIRS} )
	list( APPEND SYS_THIRDPARTY OpenGL::GL )
	if( WIN32 )
		list( APPEND SYS_THIRDPARTY OpenGL::GLU )
	endif()

	find_package( GLEW REQUIRED )
	include_directories( ${GLEW_INCLUDE_DIRS} PUBLIC )
	list( APPEND SYS_THIRDPARTY GLEW::GLEW )
else()
	# RB: moved this above the general Vulkan part so glslang does not include Vulkan SDK headers
	# which causes all kinds of weird segmentation faults because struct sizes don't match
	macro( SET_OPTION option value )
	  set( ${option} ${value} CACHE "" INTERNAL FORCE )
	endmacro()

	# Use FindVulkan module added with CMAKE 3.7
	if( NOT CMAKE_VERSION VERSION_LESS 3.7.0 )
		message( STATUS "Using module to find Vulkan and components" )
		list( APPEND VK_COMPONENTS dxc )
		if( SPIRV_SHADERC )
			list( APPEND VK_COMPONENTS shaderc_combined )
		else()
			# override Glslang build options
		
			# options required by November 2018 release
			set( ENABLE_SPVREMAPPER ON CACHE BOOL "" FORCE )
			set( ENABLE_GLSLANG_BINARIES OFF CACHE BOOL "" FORCE )
			set( ENABLE_HLSL OFF CACHE BOOL "" FORCE )
			set( ENABLE_OPT ON CACHE BOOL "" FORCE )
			set( SPIRV_SKIP_EXECUTABLES ON CACHE BOOL "" FORCE )
			
			# options required by July 2020 release
			set( ENABLE_PCH OFF CACHE BOOL "" FORCE )
			set( ENABLE_CTEST OFF CACHE BOOL "" FORCE )
			set( BUILD_EXTERNAL OFF CACHE BOOL "" FORCE )
			set( SKIP_GLSLANG_INSTALL ON CACHE BOOL "" FORCE )

			set( GLSLANG_TESTS OFF CACHE BOOL "" FORCE )
			set( ALLOW_EXTERNAL_SPIRV_TOOLS ON CACHE BOOL "" FORCE )
	
			set( GLSLANG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/glslang )
			add_subdirectory( ${GLSLANG_DIR} )
	
			include_directories( ${GLSLANG_DIR} )
			list( APPEND SYS_THIRDPARTY glslang )
			set_target_properties( glslang PROPERTIES FOLDER "libs" )
		endif()
		find_package( Vulkan OPTIONAL_COMPONENTS ${VK_COMPONENTS} )
		list( APPEND SYS_THIRDPARTY Vulkan::Vulkan )
	endif()

	if( NOT Vulkan_FOUND )
		message( FATAL_ERROR "Could not find Vulkan library!" )
	else()
		add_definitions( -DUSE_VULKAN )
		include_directories( ${Vulkan_INCLUDE_DIRS} PUBLIC )
		set( VULKAN_SDK $ENV{VULKAN_SDK} )
		include_directories( ${VULKAN_SDK}/include PUBLIC )

		if( SPIRV_SHADERC )
			list( APPEND SYS_THIRDPARTY Vulkan::shaderc_combined )
			message( STATUS "Using Vulkan ShaderC: " ${Vulkan_shaderc_combined_LIBRARY} )
		endif()

		message( STATUS "Using Vulkan: " ${Vulkan_LIBRARY} )
	endif()
	
	# Eric: For use with SDL2/Vulkan
	if( UNIX )
		# SRS - For UNIX-like target OSs use the generic SDL Vulkan platform
		add_definitions( -DVULKAN_USE_PLATFORM_SDL )
	elseif( WIN32 )
		add_definitions( -DVK_USE_PLATFORM_WIN32_KHR )
	endif()

	# SRS - Add support for vma allocator.
	#find_package( VulkanMemoryAllocator CONFIG REQUIRED )
	#find_path( VMA_INCLUDES_DIR "vk_mem_alloc.h" PATH_SUFFIXES "vma" )
	#include_directories( ${VMA_INCLUDES_DIR} )
	#list( APPEND SYS_THIRDPARTY GPUOpen::VulkanMemoryAllocator )
endif()

if ( FREETYPE )
	find_package( Freetype REQUIRED )
	find_path( FREETYPE_INCLUDE_DIR "fttypes.h" PATH_SUFFIXES "freetype" )
	include_directories( ${FREETYPE_INCLUDE_DIR} )
	add_definitions( -DID_BUILD_FREETYPE )
	list( APPEND SYS_THIRDPARTY Freetype::Freetype )
endif()

find_package( mikktspace CONFIG REQUIRED )
set( MIKKTSPACE_LIBRARY  mikktspace::mikktspace )
list( APPEND SYS_THIRDPARTY mikktspace::mikktspace )

if( FFMPEG )
	find_package( FFMPEG REQUIRED )
	include_directories( ${FFMPEG_INCLUDE_DIRS} )
	link_directories( ${FFMPEG_LIBRARY_DIRS} PUBLIC )
	add_definitions( -DUSE_FFMPEG )
	list( APPEND SYS_THIRDPARTY ${FFMPEG_LIBRARIES} )
endif()

find_package( CURL QUIET )
if( CURL_FOUND )
	add_definitions( -DID_ENABLE_CURL )
	include_directories( ${CURL_INCLUDE_DIR} )
	message( STATUS "libcurl found and enabled" )
	list( APPEND SYS_THIRDPARTY ${CURL_LIBRARY} )
else()
	message( WARNING "libcurl not found, server downloads won't be available (apart from that the engine will work)" )
endif()

if( BINKDEC )
	add_definitions( -DUSE_BINKDEC )
	include_directories( libs/libbinkdec/include PUBLIC )
endif()

add_subdirectory(game)
add_subdirectory(idlib)
add_subdirectory(libs)
if( MAYA_IMPORT )
	add_subdirectory(MayaImport)
endif()
list( APPEND SYS_THIRDPARTY MaskedOcclusionCulling )
if( USE_CMDLINE_TOOLS )
	add_subdirectory(tools/compilers)
endif()
if( USE_MFC_TOOLS )
	add_subdirectory(tools)
endif()

file(GLOB AAS_INCLUDES aas/*.h)
file(GLOB AAS_SOURCES aas/*.cpp)

file(GLOB_RECURSE CM_INCLUDES cm/*.h)
file(GLOB_RECURSE CM_SOURCES cm/*.cpp)

file(GLOB FRAMEWORK_INCLUDES framework/*.h)
file(GLOB FRAMEWORK_SOURCES framework/*.cpp)

file(GLOB FRAMEWORK_ASYNC_INCLUDES framework/async/*.h)
file(GLOB FRAMEWORK_ASYNC_SOURCES framework/async/*.cpp)

file(GLOB RENDERER_INCLUDES renderer/*.h)
file(GLOB RENDERER_SOURCES renderer/*.cpp)

if ( NOT MAYA_IMPORT )
	list(REMOVE_ITEM RENDERER_INCLUDES ${CMAKE_SOURCE_DIR}/renderer/Model_ma.h)
	list(REMOVE_ITEM RENDERER_SOURCES ${CMAKE_SOURCE_DIR}/renderer/Model_ma.cpp)
endif()

file(GLOB RENDERER_COLOR_INCLUDES renderer/Color/*.h)
file(GLOB RENDERER_COLOR_SOURCES renderer/Color/*.cpp)
file(GLOB RENDERER_DXT_INCLUDES renderer/DXT/*.h)
file(GLOB RENDERER_DXT_SOURCES renderer/DXT/*.cpp)
file(GLOB RENDERER_JOBS_INCLUDES renderer/jobs/*.h)
file(GLOB RENDERER_JOBS_SOURCES renderer/jobs/*.cpp)
file(GLOB RENDERER_JOBS_DYNAMICSHADOWVOLUME_INCLUDES renderer/jobs/dynamicshadowvolume/*.h)
file(GLOB RENDERER_JOBS_DYNAMICSHADOWVOLUME_SOURCES renderer/jobs/dynamicshadowvolume/*.cpp)
file(GLOB RENDERER_JOBS_PRELIGHTSHADOWVOLUME_INCLUDES renderer/jobs/prelightshadowvolume/*.h)
file(GLOB RENDERER_JOBS_PRELIGHTSHADOWVOLUME_SOURCES renderer/jobs/prelightshadowvolume/*.cpp)
file(GLOB RENDERER_JOBS_STATICSHADOWVOLUME_INCLUDES renderer/jobs/staticshadowvolume/*.h)
file(GLOB RENDERER_JOBS_STATICSHADOWVOLUME_SOURCES renderer/jobs/staticshadowvolume/*.cpp)

file(GLOB RENDERER_SMAA_INCLUDES renderer/SMAA/*.h)

file(GLOB RENDERER_OPENGL_INCLUDES renderer/OpenGL/*.h)
file(GLOB RENDERER_OPENGL_SOURCES renderer/OpenGL/*.cpp)

file(GLOB RENDERER_VULKAN_INCLUDES renderer/Vulkan/*.h)
file(GLOB RENDERER_VULKAN_SOURCES renderer/Vulkan/*.cpp)

file(GLOB SHADERS ${CMAKE_SOURCE_DIR}/${CMAKE_SOURCE_DIR}/../base/renderprogs/*.hlsl ${CMAKE_SOURCE_DIR}/../base/renderprogs/*.inc )
file(GLOB SHADERS_BUILTIN ${CMAKE_SOURCE_DIR}/../base/renderprogs/builtin/*.hlsl )
file(GLOB SHADERS_BUILTIN_DEBUG ${CMAKE_SOURCE_DIR}/../base/renderprogs/builtin/debug/*.hlsl )
file(GLOB SHADERS_BUILTIN_FOG ${CMAKE_SOURCE_DIR}/../base/renderprogs/builtin/fog/*.hlsl )
file(GLOB SHADERS_BUILTIN_LEGACY ${CMAKE_SOURCE_DIR}/../base/renderprogs/builtin/legacy/*.hlsl )
file(GLOB SHADERS_BUILTIN_LIGHTING ${CMAKE_SOURCE_DIR}/../base/renderprogs/builtin/lighting/*.hlsl )
file(GLOB SHADERS_BUILTIN_POST ${CMAKE_SOURCE_DIR}/../base/renderprogs/builtin/post/*.hlsl )
file(GLOB SHADERS_BUILTIN_SSAO ${CMAKE_SOURCE_DIR}/../base/renderprogs/builtin/SSAO/*.hlsl )
file(GLOB SHADERS_BUILTIN_SSGI ${CMAKE_SOURCE_DIR}/../base/renderprogs/builtin/SSGI/*.hlsl )
file(GLOB SHADERS_BUILTIN_VIDEO ${CMAKE_SOURCE_DIR}/../base/renderprogs/builtin/video/*.hlsl )
file(GLOB SHADERS_BUILTIN_VR ${CMAKE_SOURCE_DIR}/../base/renderprogs/builtin/VR/*.hlsl )

file(GLOB SOUND_INCLUDES sound/*.h)
file(GLOB SOUND_SOURCES sound/*.cpp)

file(GLOB SYS_INCLUDES sys/*.h)
file(GLOB SYS_SOURCES sys/*.cpp)

file(GLOB UI_INCLUDES ui/*.h)
file(GLOB UI_SOURCES ui/*.cpp)

file(GLOB_RECURSE WIN32_INCLUDES sys/win32/*.h)
file(GLOB_RECURSE WIN32_SOURCES sys/win32/*.cpp)

file(GLOB_RECURSE WIN32_NATVIS sys/win32/natvis/*.natvis)
file(GLOB_RECURSE WIN32_MANIFEST sys/win32/manifest/*.manifest)

if( USE_MFC_TOOLS )
	file(GLOB_RECURSE WIN32_RESOURCES sys/win32/rc/res/*.BMP sys/win32/rc/res/*.ico sys/win32/rc/res/*.cur)
	list(APPEND WIN32_RESOURCES sys/win32/rc/doom.rc)
	list(REMOVE_ITEM WIN32_INCLUDES ${CMAKE_SOURCE_DIR}/sys/win32/rc/kroom3_resource.h)
	list(REMOVE_ITEM WIN32_RESOURCES ${CMAKE_SOURCE_DIR}/sys/win32/rc/kroom3.rc)
else()
	set(WIN32_RESOURCES sys/win32/rc/kroom3_resource.h sys/win32/rc/kroom3.rc sys/win32/rc/res/game.ico)
	list(REMOVE_ITEM WIN32_INCLUDES ${CMAKE_SOURCE_DIR}/sys/win32/rc/resource.h)
endif()

file(GLOB STUB_SOURCES sys/stub/openal_stub.cpp)

file(GLOB POSIX_INCLUDES sys/posix/*.h)
file(GLOB POSIX_SOURCES sys/posix/*.cpp)

file(GLOB LINUX_INCLUDES sys/linux/*.h)
file(GLOB LINUX_SOURCES sys/linux/*.cpp)

file(GLOB SDL_INCLUDES sys/sdl/*.h)
file(GLOB SDL_SOURCES sys/sdl/*.cpp)

# Eric: Utilize either the Vulkan or GL implementation of SDL
if( UNIX )
	if( USE_VULKAN )
		get_filename_component(sdl_glimp_cpp_full_path ${CMAKE_CURRENT_SOURCE_DIR}/sys/sdl/sdl_glimp.cpp ABSOLUTE)
		list(REMOVE_ITEM SDL_SOURCES "${sdl_glimp_cpp_full_path}")
	else()
		get_filename_component(sdl_vkimp_cpp_full_path ${CMAKE_CURRENT_SOURCE_DIR}/sys/sdl/sdl_vkimp.cpp ABSOLUTE)
		list(REMOVE_ITEM SDL_SOURCES "${sdl_vkimp_cpp_full_path}")
	endif()
endif()

source_group("AAS" FILES ${AAS_INCLUDES})
source_group("AAS" FILES ${AAS_SOURCES})
source_group("CM" FILES ${CM_INCLUDES})
source_group("CM" FILES ${CM_SOURCES})
source_group("Framework" FILES ${FRAMEWORK_INCLUDES})
source_group("Framework" FILES ${FRAMEWORK_SOURCES})
source_group("Framework\\Async" FILES ${FRAMEWORK_ASYNC_INCLUDES})
source_group("Framework\\Async" FILES ${FRAMEWORK_ASYNC_SOURCES})
source_group("Renderer" FILES ${RENDERER_INCLUDES})
source_group("Renderer" FILES ${RENDERER_SOURCES})
source_group("Renderer\\Color" FILES ${RENDERER_COLOR_INCLUDES})
source_group("Renderer\\Color" FILES ${RENDERER_COLOR_SOURCES})
source_group("Renderer\\DXT" FILES ${RENDERER_DXT_INCLUDES})
source_group("Renderer\\DXT" FILES ${RENDERER_DXT_SOURCES})
source_group("Renderer\\Jobs" FILES ${RENDERER_JOBS_INCLUDES})
source_group("Renderer\\Jobs" FILES ${RENDERER_JOBS_SOURCES})
source_group("Renderer\\Jobs\\DynamicShadowVolume" FILES ${RENDERER_JOBS_DYNAMICSHADOWVOLUME_INCLUDES})
source_group("Renderer\\Jobs\\DynamicShadowVolume" FILES ${RENDERER_JOBS_DYNAMICSHADOWVOLUME_SOURCES})
source_group("Renderer\\Jobs\\PreLightShadowVolume" FILES ${RENDERER_JOBS_PRELIGHTSHADOWVOLUME_INCLUDES})
source_group("Renderer\\Jobs\\PreLightShadowVolume" FILES ${RENDERER_JOBS_PRELIGHTSHADOWVOLUME_SOURCES})
source_group("Renderer\\Jobs\\StaticShadowVolume" FILES ${RENDERER_JOBS_STATICSHADOWVOLUME_INCLUDES})
source_group("Renderer\\Jobs\\StaticShadowVolume" FILES ${RENDERER_JOBS_STATICSHADOWVOLUME_SOURCES})
source_group("Renderer\\OpenGL" FILES ${RENDERER_OPENGL_INCLUDES})
source_group("Renderer\\OpenGL" FILES ${RENDERER_OPENGL_SOURCES})
source_group("Renderer\\Vulkan" FILES ${RENDERER_VULKAN_INCLUDES})
source_group("Renderer\\Vulkan" FILES ${RENDERER_VULKAN_SOURCES})
source_group("Renderer\\SMAA" FILES ${RENDERER_SMAA_INCLUDES})
source_group("Renderer\\Shaders" FILES ${SHADERS})
source_group("Renderer\\Shaders\\builtin" FILES ${SHADERS_BUILTIN})
source_group("Renderer\\Shaders\\builtin\\debug" FILES ${SHADERS_BUILTIN_DEBUG})
source_group("Renderer\\Shaders\\builtin\\fog" FILES ${SHADERS_BUILTIN_FOG})
source_group("Renderer\\Shaders\\builtin\\legacy" FILES ${SHADERS_BUILTIN_LEGACY})
source_group("Renderer\\Shaders\\builtin\\lighting" FILES ${SHADERS_BUILTIN_LIGHTING})
source_group("Renderer\\Shaders\\builtin\\post" FILES ${SHADERS_BUILTIN_POST})
source_group("Renderer\\Shaders\\builtin\\SSAO" FILES ${SHADERS_BUILTIN_SSAO})
source_group("Renderer\\Shaders\\builtin\\SSGI" FILES ${SHADERS_BUILTIN_SSGI})
source_group("Renderer\\Shaders\\builtin\\video" FILES ${SHADERS_BUILTIN_VIDEO})
source_group("Renderer\\Shaders\\builtin\\VR" FILES ${SHADERS_BUILTIN_VR})
source_group("Sound" FILES ${SOUND_INCLUDES})
source_group("Sound" FILES ${SOUND_SOURCES})
source_group("UI" FILES ${UI_INCLUDES})
source_group("UI" FILES ${UI_SOURCES})
source_group("Sys" FILES ${SYS_INCLUDES})
source_group("Sys" FILES ${SYS_SOURCES})
source_group("Sys\\Stub" FILES ${STUB_SOURCES})
source_group("Sys\\Posix" FILES ${POSIX_INCLUDES})
source_group("Sys\\Posix" FILES ${POSIX_SOURCES})
source_group("Sys\\Common" FILES ${COMMON_INCLUDES})
source_group("Sys\\Common" FILES ${COMMON_SOURCES})
source_group("Sys\\SDL" FILES ${SDL_INCLUDES})
source_group("Sys\\SDL" FILES ${SDL_SOURCES})
source_group("Sys\\Win32" FILES ${WIN32_INCLUDES})
source_group("Sys\\Win32" FILES ${WIN32_SOURCES})
source_group("Sys\\Win32\\Resources" FILES ${WIN32_RESOURCES})
source_group("Sys\\Win32\\NatVis" FILES ${WIN32_NATVIS})
source_group("Sys\\Win32\\Manifest" FILES ${WIN32_MANIFEST})

set(KROOM3_INCLUDES
	${AAS_INCLUDES}
	${CM_INCLUDES}
	${FRAMEWORK_INCLUDES}
	${FRAMEWORK_ASYNC_INCLUDES}
	${RENDERER_INCLUDES}
	${RENDERER_COLOR_INCLUDES}
	${RENDERER_DXT_INCLUDES}
	${RENDERER_JOBS_INCLUDES}
	${RENDERER_JOBS_DYNAMICSHADOWVOLUME_INCLUDES}
	${RENDERER_JOBS_PRELIGHTSHADOWVOLUME_INCLUDES}
	${RENDERER_JOBS_STATICSHADOWVOLUME_INCLUDES}
	${RENDERER_SMAA_INCLUDES}
	${SHADERS}
	${SHADERS_BUILTIN}
	${SHADERS_BUILTIN_DEBUG}
	${SHADERS_BUILTIN_FOG}
	${SHADERS_BUILTIN_LEGACY}
	${SHADERS_BUILTIN_LIGHTING}
	${SHADERS_BUILTIN_POST}
	${SHADERS_BUILTIN_SSAO}
	${SHADERS_BUILTIN_SSGI}
	${SHADERS_BUILTIN_VIDEO}
	${SHADERS_BUILTIN_VR}
	${SOUND_INCLUDES}
	${UI_INCLUDES})

set(KROOM3_SOURCES
	${AAS_SOURCES}
	${CM_SOURCES}
	${FRAMEWORK_SOURCES}
	${FRAMEWORK_ASYNC_SOURCES}
	${RENDERER_SOURCES}
	${RENDERER_COLOR_SOURCES}
	${RENDERER_DXT_SOURCES}
	${RENDERER_JOBS_SOURCES}
	${RENDERER_JOBS_DYNAMICSHADOWVOLUME_SOURCES}
	${RENDERER_JOBS_PRELIGHTSHADOWVOLUME_SOURCES}
	${RENDERER_JOBS_STATICSHADOWVOLUME_SOURCES}
	${SOUND_SOURCES}
	${UI_SOURCES})

add_definitions(-D__DOOM__
				-D__DOOM_DLL__)

if( USE_CMDLINE_TOOLS )
	add_definitions( -DID_ALLOW_CMD_TOOLS )
endif()

include_directories(
			idlib
			libs/miniz
			libs/minizip
			PUBLIC)

if ( ENGINE )
	if( MSVC )
		 set(DX_LIBRARIES
			 dinput8
			 dsound
			 dxguid
			 Xinput)

		list (APPEND SYS_LIBS
			${DX_LIBRARIES})

		if( USE_VULKAN )
			list( APPEND KROOM3_INCLUDES ${RENDERER_VULKAN_INCLUDES} )
			list( APPEND KROOM3_SOURCES ${RENDERER_VULKAN_SOURCES} )		
		else()
			list( APPEND KROOM3_INCLUDES ${RENDERER_OPENGL_INCLUDES} )
			list( APPEND KROOM3_SOURCES ${RENDERER_OPENGL_SOURCES} )
		endif()

		list(APPEND KROOM3_INCLUDES
			${SYS_INCLUDES}
			${WIN32_INCLUDES})

		list(APPEND KROOM3_SOURCES
			${SYS_SOURCES}
			${WIN32_SOURCES}
			${WIN32_NATVIS}
			${WIN32_MANIFEST})

		set_source_files_properties( ${SHADERS} PROPERTIES VS_TOOL_OVERRIDE "None" )
		set_source_files_properties( ${SHADERS_BUILTIN} PROPERTIES VS_TOOL_OVERRIDE "None" )
		set_source_files_properties( ${SHADERS_BUILTIN_DEBUG} PROPERTIES VS_TOOL_OVERRIDE "None" )
		set_source_files_properties( ${SHADERS_BUILTIN_FOG} PROPERTIES VS_TOOL_OVERRIDE "None" )
		set_source_files_properties( ${SHADERS_BUILTIN_LEGACY} PROPERTIES VS_TOOL_OVERRIDE "None" )
		set_source_files_properties( ${SHADERS_BUILTIN_LIGHTING} PROPERTIES VS_TOOL_OVERRIDE "None" )
		set_source_files_properties( ${SHADERS_BUILTIN_POST} PROPERTIES VS_TOOL_OVERRIDE "None" )
		set_source_files_properties( ${SHADERS_BUILTIN_SSAO} PROPERTIES VS_TOOL_OVERRIDE "None" )
		set_source_files_properties( ${SHADERS_BUILTIN_SSGI} PROPERTIES VS_TOOL_OVERRIDE "None" )
		set_source_files_properties( ${SHADERS_BUILTIN_VIDEO} PROPERTIES VS_TOOL_OVERRIDE "None" )
		set_source_files_properties( ${SHADERS_BUILTIN_VR} PROPERTIES VS_TOOL_OVERRIDE "None" )

		list( REMOVE_DUPLICATES KROOM3_SOURCES )

		if( USE_PRECOMPILED_HEADERS )
			set( KROOM3_PRECOMPILED_SOURCES ${KROOM3_SOURCES} )
			list( REMOVE_ITEM KROOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/DXT/DXTDecoder.cpp )
			list( REMOVE_ITEM KROOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/DXT/DXTEncoder.cpp )
			list( REMOVE_ITEM KROOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/DXT/DXTEncoder_SSE2.cpp )
			list( REMOVE_ITEM KROOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/jobs/dynamicshadowvolume/DynamicShadowVolume.cpp )
			list( REMOVE_ITEM KROOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/jobs/prelightshadowvolume/PreLightShadowVolume.cpp )
			list( REMOVE_ITEM KROOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/jobs/staticshadowvolume/StaticShadowVolume.cpp )
			list( REMOVE_ITEM KROOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/jobs/ShadowShared.cpp )
			list( REMOVE_ITEM KROOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/RenderLog.cpp )

			set_source_files_properties(
				${KROOM3_PRECOMPILED_SOURCES}
				PROPERTIES
				COMPILE_FLAGS "/Yuprecompiled.h"
				OBJECT_DEPENDS "precompiled.pch"
			)

			set_source_files_properties(
				framework/precompiled.cpp
				PROPERTIES
				COMPILE_FLAGS "/Ycprecompiled.h"
				OBJECT_OUTPUTS "precompiled.pch"
			)
		endif()

		add_executable( ${PROJECT_NAME} WIN32 ${KROOM3_INCLUDES} ${KROOM3_SOURCES} ${WIN32_RESOURCES} )

		add_dependencies(${PROJECT_NAME} idlib Game external)
		target_link_libraries(${PROJECT_NAME}
			idlib
			external
			${SYS_THIRDPARTY}
			${SYS_LIBS})

		# set MSVC default debug directory on executable
		set_target_properties( ${PROJECT_NAME} PROPERTIES VS_STARTUP_PROJECT ${PROJECT_NAME} )
		set_target_properties( ${PROJECT_NAME} PROPERTIES VS_DEBUGGER_COMMAND "${CMAKE_SOURCE_DIR}/../${PROJECT_NAME}.exe" )
		set_target_properties( ${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/.." )
		set_target_properties( ${PROJECT_NAME} PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "+set developer 1 +set r_fullscreen 0" )
		set_target_properties( ${PROJECT_NAME} PROPERTIES FOLDER "exes" )
	elseif( UNIX )
		if( USE_VULKAN )
			list( APPEND RBDOOM3_INCLUDES ${RENDERER_VULKAN_INCLUDES} )
			list( APPEND RBDOOM3_SOURCES ${RENDERER_VULKAN_SOURCES} )
		else()
			list( APPEND RBDOOM3_INCLUDES ${RENDERER_OPENGL_INCLUDES} )
			list( APPEND RBDOOM3_SOURCES ${RENDERER_OPENGL_SOURCES} )
		endif()

		list(APPEND KROOM3_INCLUDES
			${SYS_INCLUDES}
			${POSIX_INCLUDES}
			${SDL_INCLUDES}
			${LINUX_INCLUDES})

		list(APPEND KROOM3_SOURCES
			${SYS_SOURCES}
			${POSIX_SOURCES}
			${SDL_SOURCES}
			${LINUX_SOURCES})

		list( REMOVE_DUPLICATES KROOM3_SOURCES )

		if( USE_PRECOMPILED_HEADERS )
			set( KROOM3_PRECOMPILED_SOURCES ${KROOM3_SOURCES})
			list( REMOVE_ITEM KROOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/DXT/DXTDecoder.cpp )
			list( REMOVE_ITEM KROOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/DXT/DXTEncoder.cpp )
			list( REMOVE_ITEM KROOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/DXT/DXTEncoder_SSE2.cpp )

			foreach( src_file ${KROOM3_PRECOMPILED_SOURCES} )
				#message(STATUS "-include precompiled.h for ${src_file}")
				set_source_files_properties(
					${src_file}
					PROPERTIES
					COMPILE_FLAGS "-include ${CMAKE_CURRENT_SOURCE_DIR}/idlib/precompiled.h")
			endforeach()

			# precompiled magic for GCC/clang, adapted from https://gist.github.com/573926
			STRING( TOUPPER "CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}" _flags_var_name )
			SET( _compiler_FLAGS "${${_flags_var_name}} -std=c++${CMAKE_CXX_STANDARD}" )
			GET_DIRECTORY_PROPERTY( _directory_flags INCLUDE_DIRECTORIES )
			FOREACH( item ${_directory_flags} )
				LIST( APPEND _compiler_FLAGS " -I${item}" )
			ENDFOREACH( item )
		endif()

		GET_DIRECTORY_PROPERTY( _directory_flags DEFINITIONS )
		LIST( APPEND _compiler_FLAGS ${_directory_flags} )
		SEPARATE_ARGUMENTS( _compiler_FLAGS )

		if( USE_PRECOMPILED_HEADERS )
			# we need to recreate the precompiled header for Kroom3
			# (i.e. can't use the one created for idlib before)
			# because some definitions (e.g. -D__IDLIB__ -D__DOOM_DLL__) differ
			add_custom_target( precomp_header_kroom3 ALL
					COMMAND ${CMAKE_CXX_COMPILER} ${_compiler_FLAGS} -x c++-header idlib/precompiled.h -o idlib/precompiled.h.gch
					WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
					COMMENT "Creating idlib/precompiled.h.gch for Kroom3" )

			# make sure this is run after creating idlib
			add_dependencies( precomp_header_kroom3 idlib )
		endif()

		add_executable( ${PROJECT_NAME} ${KROOM3_INCLUDES} ${KROOM3_SOURCES} )

		if ( USE_PRECOMPILED_HEADERS )
			# make sure precompiled header is created before executable is compiled
			add_dependencies( ${PROJECT_NAME} precomp_header_kroom3 )

			if( WIN32 )
				set( remove_command "del" )
			else()
				set( remove_command "rm" )
			endif()

			# delete precompiled header file after executable is compiled: command line build case
			if( CMAKE_GENERATOR MATCHES "Makefiles" OR CMAKE_GENERATOR MATCHES "Ninja" )
				add_custom_target( rm_precomp_header ALL
						COMMAND ${remove_command} "idlib/precompiled.h.gch"
						WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
						COMMENT "remove idlib/precompiled.h.gch" )
				add_dependencies( rm_precomp_header ${PROJECT_NAME} )

			# delete precompiled header file after executable is compiled: IDE build case (e.g. Xcode)
			else()
				add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD
						# SRS - added wildcards to remove tmp files from cmake ZERO_CHECK regeneration
						COMMAND ${remove_command} "idlib/precompiled.h*.gch*"
						WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
						COMMENT "remove idlib/precompiled.h.gch" )
			endif()
		endif()

		target_link_libraries(${PROJECT_NAME}
			idlib
			external
			${SYS_THIRDPARTY}
			${SYS_LIBS})
		
		install(TARGETS ${PROJECT_NAME}
				RUNTIME DESTINATION "${bindir}"
				LIBRARY DESTINATION "${libdir}"
				ARCHIVE DESTINATION "${libdir}"	)
	endif()

	if( USE_CMDLINE_TOOLS )
		target_link_libraries( ${PROJECT_NAME} cmdtools )
	endif()

	if( USE_MFC_TOOLS )
		target_link_libraries( ${PROJECT_NAME} tools )
	endif()

	set_target_properties( ${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY $<1:>${CMAKE_SOURCE_DIR}/.. LIBRARY_OUTPUT_DIRECTORY $<1:>${CMAKE_SOURCE_DIR}/.. )
endif()

if ( DEDICATED )
	if( MSVC )
		 set(DX_LIBRARIES
			 dinput8
			 dsound
			 dxguid
			 Xinput)

		list (APPEND SYS_LIBS
			${DX_LIBRARIES})

		if( USE_VULKAN )
			list( APPEND KROOM3_INCLUDES ${RENDERER_VULKAN_INCLUDES} )
			list( APPEND KROOM3_SOURCES ${RENDERER_VULKAN_SOURCES} )		
		else()
			list( APPEND KROOM3_INCLUDES ${RENDERER_OPENGL_INCLUDES} )
			list( APPEND KROOM3_SOURCES ${RENDERER_OPENGL_SOURCES} )
		endif()

		list(APPEND KROOM3_INCLUDES
			${SYS_INCLUDES}
			${WIN32_INCLUDES})

		list(APPEND KROOM3_SOURCES
			${STUB_SOURCES}
			${SYS_SOURCES}
			${WIN32_SOURCES}
			${WIN32_NATVIS}
			${WIN32_MANIFEST})

		set_source_files_properties( ${SHADERS} PROPERTIES VS_TOOL_OVERRIDE "None" )
		set_source_files_properties( ${SHADERS_BUILTIN} PROPERTIES VS_TOOL_OVERRIDE "None" )
		set_source_files_properties( ${SHADERS_BUILTIN_DEBUG} PROPERTIES VS_TOOL_OVERRIDE "None" )
		set_source_files_properties( ${SHADERS_BUILTIN_FOG} PROPERTIES VS_TOOL_OVERRIDE "None" )
		set_source_files_properties( ${SHADERS_BUILTIN_LEGACY} PROPERTIES VS_TOOL_OVERRIDE "None" )
		set_source_files_properties( ${SHADERS_BUILTIN_LIGHTING} PROPERTIES VS_TOOL_OVERRIDE "None" )
		set_source_files_properties( ${SHADERS_BUILTIN_POST} PROPERTIES VS_TOOL_OVERRIDE "None" )
		set_source_files_properties( ${SHADERS_BUILTIN_SSAO} PROPERTIES VS_TOOL_OVERRIDE "None" )
		set_source_files_properties( ${SHADERS_BUILTIN_SSGI} PROPERTIES VS_TOOL_OVERRIDE "None" )
		set_source_files_properties( ${SHADERS_BUILTIN_VIDEO} PROPERTIES VS_TOOL_OVERRIDE "None" )
		set_source_files_properties( ${SHADERS_BUILTIN_VR} PROPERTIES VS_TOOL_OVERRIDE "None" )

		list( REMOVE_DUPLICATES KROOM3_SOURCES )

		if( USE_PRECOMPILED_HEADERS )
			set( KROOM3_PRECOMPILED_SOURCES ${KROOM3_SOURCES} )
			list( REMOVE_ITEM KROOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/DXT/DXTDecoder.cpp )
			list( REMOVE_ITEM KROOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/DXT/DXTEncoder.cpp )
			list( REMOVE_ITEM KROOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/DXT/DXTEncoder_SSE2.cpp )
			list( REMOVE_ITEM KROOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/jobs/dynamicshadowvolume/DynamicShadowVolume.cpp )
			list( REMOVE_ITEM KROOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/jobs/prelightshadowvolume/PreLightShadowVolume.cpp )
			list( REMOVE_ITEM KROOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/jobs/staticshadowvolume/StaticShadowVolume.cpp )
			list( REMOVE_ITEM KROOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/jobs/ShadowShared.cpp )
			list( REMOVE_ITEM KROOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/RenderLog.cpp )

			set_source_files_properties(
				${KROOM3_PRECOMPILED_SOURCES}
				PROPERTIES
				COMPILE_FLAGS "/Yuprecompiled.h"
				OBJECT_DEPENDS "precompiled.pch"
			)

			set_source_files_properties(
				framework/precompiled.cpp
				PROPERTIES
				COMPILE_FLAGS "/Ycprecompiled.h"
				OBJECT_OUTPUTS "precompiled.pch"
			)
		endif()

		add_executable( ${PROJECT_NAME}-ded WIN32 ${KROOM3_INCLUDES} ${KROOM3_SOURCES} ${WIN32_RESOURCES} )
		add_dependencies( ${PROJECT_NAME}-ded idlib external )

		set_target_properties( ${PROJECT_NAME}-ded PROPERTIES COMPILE_DEFINITIONS "ID_DEDICATED" )

		list( REMOVE_ITEM SYS_THIRDPARTY ${OPENAL_LIBRARY} )

		target_link_libraries(${PROJECT_NAME}-ded
			idlib
			external
			${SYS_THIRDPARTY}
			${SYS_LIBS})

		set_target_properties( ${PROJECT_NAME}-ded PROPERTIES FOLDER "exes" )
	elseif( UNIX )
		if( USE_VULKAN )
			list( APPEND RBDOOM3_INCLUDES ${RENDERER_VULKAN_INCLUDES} )
			list( APPEND RBDOOM3_SOURCES ${RENDERER_VULKAN_SOURCES} )
		else()
			list( APPEND RBDOOM3_INCLUDES ${RENDERER_OPENGL_INCLUDES} )
			list( APPEND RBDOOM3_SOURCES ${RENDERER_OPENGL_SOURCES} )
		endif()

		list(APPEND KROOM3_INCLUDES
			${SYS_INCLUDES}
			${POSIX_INCLUDES}
			${SDL_INCLUDES}
			${LINUX_INCLUDES})

		list(APPEND KROOM3_SOURCES
			${STUB_SOURCES}
			${SYS_SOURCES}
			${POSIX_SOURCES}
			${SDL_SOURCES}
			${LINUX_SOURCES})

		list( REMOVE_DUPLICATES KROOM3_SOURCES )

		if( USE_PRECOMPILED_HEADERS )
			set( KROOM3_PRECOMPILED_SOURCES ${KROOM3_SOURCES})
			list( REMOVE_ITEM KROOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/DXT/DXTDecoder.cpp )
			list( REMOVE_ITEM KROOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/DXT/DXTEncoder.cpp )
			list( REMOVE_ITEM KROOM3_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/DXT/DXTEncoder_SSE2.cpp )

			foreach( src_file ${KROOM3_PRECOMPILED_SOURCES} )
				#message(STATUS "-include precompiled.h for ${src_file}")
				set_source_files_properties(
					${src_file}
					PROPERTIES
					COMPILE_FLAGS "-include ${CMAKE_CURRENT_SOURCE_DIR}/idlib/precompiled.h")
			endforeach()

			# precompiled magic for GCC/clang, adapted from https://gist.github.com/573926
			STRING( TOUPPER "CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}" _flags_var_name )
			SET( _compiler_FLAGS "${${_flags_var_name}} -std=c++${CMAKE_CXX_STANDARD}" )
			GET_DIRECTORY_PROPERTY( _directory_flags INCLUDE_DIRECTORIES )
			FOREACH( item ${_directory_flags} )
				LIST( APPEND _compiler_FLAGS " -I${item}" )
			ENDFOREACH( item )
		endif()

		GET_DIRECTORY_PROPERTY( _directory_flags DEFINITIONS )
		LIST( APPEND _compiler_FLAGS ${_directory_flags} )
		SEPARATE_ARGUMENTS( _compiler_FLAGS )

		if( USE_PRECOMPILED_HEADERS )
			# we need to recreate the precompiled header for Kroom3
			# (i.e. can't use the one created for idlib before)
			# because some definitions (e.g. -D__IDLIB__ -D__DOOM_DLL__) differ
			add_custom_target( precomp_header_kroom3_ded ALL
					COMMAND ${CMAKE_CXX_COMPILER} ${_compiler_FLAGS} -x c++-header idlib/precompiled.h -o idlib/precompiled.h.gch
					WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
					COMMENT "Creating idlib/precompiled.h.gch for Kroom3" )

			# make sure this is run after creating idlib
			add_dependencies( precomp_header_kroom3_ded idlib )
		endif()

		add_executable( ${PROJECT_NAME}-ded ${KROOM3_INCLUDES} ${KROOM3_SOURCES} )

		if ( USE_PRECOMPILED_HEADERS )
			# make sure precompiled header is created before executable is compiled
			add_dependencies( ${PROJECT_NAME}-ded precomp_header_kroom3_ded )

			if( WIN32 )
				set( remove_command "del" )
			else()
				set( remove_command "rm" )
			endif()

			# delete precompiled header file after executable is compiled: command line build case
			if( CMAKE_GENERATOR MATCHES "Makefiles" OR CMAKE_GENERATOR MATCHES "Ninja" )
				add_custom_target( rm_precomp_header_ded ALL
						COMMAND ${remove_command} "idlib/precompiled.h.gch"
						WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
						COMMENT "remove idlib/precompiled.h.gch" )
				add_dependencies( rm_precomp_header_ded ${PROJECT_NAME}-ded )

			# delete precompiled header file after executable is compiled: IDE build case (e.g. Xcode)
			else()
				add_custom_command( TARGET ${PROJECT_NAME}-ded POST_BUILD
						# SRS - added wildcards to remove tmp files from cmake ZERO_CHECK regeneration
						COMMAND ${remove_command} "idlib/precompiled.h*.gch*"
						WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
						COMMENT "remove idlib/precompiled.h.gch" )
			endif()
		endif()

		list( REMOVE_ITEM SYS_THIRDPARTY ${OPENAL_LIBRARY} )

		target_link_libraries(${PROJECT_NAME}-ded
			idlib
			external
			${SYS_STUB}
			${SYS_THIRDPARTY}
			${SYS_LIBS})
		
		install(TARGETS ${PROJECT_NAME}-ded
				RUNTIME DESTINATION "${bindir}"
				LIBRARY DESTINATION "${libdir}"
				ARCHIVE DESTINATION "${libdir}")
	endif()

	set_target_properties( ${PROJECT_NAME}-ded PROPERTIES RUNTIME_OUTPUT_DIRECTORY $<1:>${CMAKE_SOURCE_DIR}/.. LIBRARY_OUTPUT_DIRECTORY $<1:>${CMAKE_SOURCE_DIR}/.. )
endif()