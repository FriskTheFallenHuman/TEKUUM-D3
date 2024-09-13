cmake_minimum_required(VERSION 2.20)

# Check if vcpkg exist in our source folder
if ( EXISTS "${CMAKE_SOURCE_DIR}/../vcpkg/scripts/buildsystems/vcpkg.cmake" )
	set( CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/../vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file" )
else()
	message( FATAL_ERROR "Vcpkg wasn't not found, aborting, make sure to git clone this with all submodules!")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

project(RBDOOM3 C CXX)

option(USE_CMDLINE_TOOLS "Compile the built-in command line based tools" OFF)
option(USE_MFC_TOOLS "Compile the built-in MFC based tools" OFF)
option(STANDALONE "Skip Doom 3 base/ folder" OFF)
option(FORCE_COLOR_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." OFF)
option(COMPILE_COMMANDS "Generate compile_commands.json" OFF)
option(USE_PRECOMPILED_HEADERS "Use precompiled headers during build" ON)
option(ONATIVE "Optimize for the host CPU" OFF)
option(RETAIL "Strip certain developer features and cheats from shipping builds" OFF)

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

# Enforced colored output
if ( CMAKE_COMPILER_IS_GNUCC )
	add_compile_options( -fdiagnostics-color=always )
elseif( CMAKE_C_COMPILER_ID MATCHES "Clang" )
	add_compile_options( -fcolor-diagnostics )
endif ()

if(NOT MSVC)
	message( STATUS CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE} )
endif()

if( CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID STREQUAL "Clang" )
	add_definitions(-pipe)
	#add_definitions(-Wall)
	add_definitions(-Werror=format-security)
	add_definitions(-Werror=format)
	add_definitions(-Wno-format-zero-length)
	add_definitions(-Wno-nonnull)

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

	#if(NOT ANDROID)
		add_definitions( -DUSE_EXCEPTIONS )
	#endif()

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

	set(C MAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ggdb -D_DEBUG -O0" )
	#set( CMAKE_C_FLAGS_DEBUGALL "${CMAKE_C_FLAGS_DEBUGALL} -g -ggdb -D_DEBUG" )
	#set( CMAKE_C_FLAGS_PROFILE "${CMAKE_C_FLAGS_PROFILE} -g -ggdb -D_DEBUG -O1 -fno-omit-frame-pointer" )
	set( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -ffp-contract=off -fomit-frame-pointer -fno-math-errno -fno-trapping-math" )
	set( CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} -g -O3 -ffp-contract=off -fomit-frame-pointer -fno-math-errno -fno-trapping-math" )
	set( CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} -Os -ffp-contract=off -fomit-frame-pointer -fno-math-errno -fno-trapping-math" )

	set( CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG} )
	set( CMAKE_CXX_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE} )
	set( CMAKE_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_C_FLAGS_RELWITHDEBINFO} )
	set( CMAKE_CXX_FLAGS_MINSIZEREL ${CMAKE_C_FLAGS_MINSIZEREL} )
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
	set( CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /D _DEBUG /MP /MDd" )
	set( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MP /Oi /Oy /MD" )
	set( CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} /MP /Oi /MDd" )
	set( CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} /MP /Oi /Oy /MD" )

	set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MP /MDd" )
	set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MP /Oi /Oy /MD" )
	set( CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MP /MDd" )
	set( CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MP /Oi /Oy /MD" )

	# RB: without /SAFESEH:NO we can't link against ffmpeg libs and VS2013 or we have to build our own
	# libs for newer compilers
	set( CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /SAFESEH:NO" )
	set( CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL} /SAFESEH:NO" )
	set( CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /SAFESEH:NO" )
	set( CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO} /SAFESEH:NO" )
	
	add_definitions( -D_ALLOW_KEYWORD_MACROS ) # because of the "#define private public" and "#define protected public" in TypeInfo.cpp

	add_definitions( -DWIN32
					-DNOMINMAX
					-D_WINDOWS
					-D_CRT_SECURE_NO_DEPRECATE
					-D_CRT_NONSTDC_NO_DEPRECATE
					-D_CRT_SECURE_NO_WARNINGS
					-D_MBCS
					-DUSE_EXCEPTIONS )

	#if( WINDOWS10 ) # Windows RT
	#	add_definitions( -DUSE_WINRT )
	#endif()

	if( NOT CMAKE_CL_64 )
		add_definitions( -D_USE_32BIT_TIME_T )
	endif()

	if( USE_MFC_TOOLS )
		add_definitions( -D_AFXDLL )

		# 0: Use Standard Windows Libraries
		# 1: Use MFC in a Static Library
		# 2: Use MFC in a Shared DLL
		set( CMAKE_MFC_FLAG 2 )
	endif()
endif()

# CMake only used for desktop builds
add_definitions(-DID_PC)

if ( USE_INTRINSICS_SSE )
	add_definitions( -DUSE_INTRINSICS_SSE )
endif()

if( STANDALONE )
	add_definitions( -DSTANDALONE )
endif()

if( RETAIL )
	add_definitions( -DID_RETAIL )
endif()

find_package( Stb REQUIRED )
include_directories( ${Stb_INCLUDE_DIR} )

find_package( OpenAL REQUIRED )
include_directories( ${OPENAL_INCLUDE_DIR} )

find_package( GLEW REQUIRED )
include_directories( ${GLEW_INCLUDE_DIRS} )

find_package( Freetype REQUIRED )
find_path( FREETYPE_INCLUDE_DIR "fttypes.h" PATH_SUFFIXES "freetype" )
include_directories( ${FREETYPE_INCLUDE_DIR} )
set( FREETYPE_LIBRARY Freetype::Freetype )

add_subdirectory(game)
add_subdirectory(idlib)
add_subdirectory(libs)

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

file(GLOB COMPILER_INCLUDES tools/compilers/*.h)

file(GLOB COMPILER_AAS_INCLUDES tools/compilers/aas/*.h)
file(GLOB COMPILER_AAS_SOURCES tools/compilers/aas/*.cpp)

file(GLOB COMPILER_DMAP_INCLUDES tools/compilers/dmap/*.h)
file(GLOB COMPILER_DMAP_SOURCES tools/compilers/dmap/*.cpp)

file(GLOB COMPILER_ROQVQ_INCLUDES tools/compilers/roqvq/*.h)
file(GLOB COMPILER_ROQVQ_SOURCES tools/compilers/roqvq/*.cpp)

set(TOOLS_INCLUDES tools/edit_public.h)

file(GLOB TOOLS_COMAFX_INCLUDES tools/comafx/*.h)
file(GLOB TOOLS_COMAFX_SOURCES tools/comafx/*.cpp)

set(TOOLS_COMMON_INCLUDES
	tools/common/ColorButton.h
	tools/common/DialogHelpers.h
	tools/common/MaskEdit.h
	tools/common/OpenFileDialog.h
	tools/common/PropertyGrid.h
	tools/common/RegistryOptions.h
	#tools/common/RenderBumpFlatDialog.h
	tools/common/RollupPanel.h
	tools/common/SpinButton.h)

set(TOOLS_COMMON_SOURCES
	tools/common/AlphaPopup.cpp
	tools/common/ColorButton.cpp
	tools/common/MaskEdit.cpp
	tools/common/OpenFileDialog.cpp
	tools/common/PropertyGrid.cpp
	tools/common/RegistryOptions.cpp
	#tools/common/RenderBumpFlatDialog.cpp
	tools/common/RollupPanel.cpp
	tools/common/SpinButton.cpp)

file(GLOB TOOLS_COMMON_PROPTREE_INCLUDES tools/common/PropTree/*.h)
file(GLOB TOOLS_COMMON_PROPTREE_SOURCES tools/common/PropTree/*.cpp)

file(GLOB EDITOR_AF_INCLUDES tools/af/*.h)
file(GLOB EDITOR_AF_SOURCES tools/af/*.cpp)

set(EDITOR_DECL_INCLUDES
	tools/decl/DialogDeclBrowser.h
	tools/decl/DialogDeclEditor.h
	tools/decl/DialogDeclNew.h)

set(EDITOR_DECL_SOURCES
	tools/decl/DialogDeclBrowser.cpp
	tools/decl/DialogDeclEditor.cpp
	tools/decl/DialogDeclNew.cpp)

file(GLOB EDITOR_DEBUGGER_INCLUDES tools/debugger/*.h)
file(GLOB EDITOR_DEBUGGER_SOURCES tools/debugger/*.cpp)

file(GLOB EDITOR_GUIED_INCLUDES tools/guied/*.h)
set(EDITOR_GUIED_SOURCES
	tools/guied/GEApp.cpp
	tools/guied/GECheckInDlg.cpp
	tools/guied/GEDeleteModifier.cpp
	tools/guied/GEHideModifier.cpp
	tools/guied/GEInsertModifier.cpp
	tools/guied/GEItemPropsDlg.cpp
	tools/guied/GEItemScriptsDlg.cpp
	tools/guied/GEKeyValueModifier.cpp
	tools/guied/GEModifier.cpp
	tools/guied/GEModifierGroup.cpp
	tools/guied/GEModifierStack.cpp
	tools/guied/GEMoveModifier.cpp
	tools/guied/GENavigator.cpp
	tools/guied/GEOptions.cpp
	tools/guied/GEOptionsDlg.cpp
	tools/guied/GEProperties.cpp
	tools/guied/GEPropertyPage.cpp
	tools/guied/GESelectionMgr.cpp
	tools/guied/GESizeModifier.cpp
	tools/guied/GEStateModifier.cpp
	tools/guied/GEStatusBar.cpp
	tools/guied/GETransformer.cpp
	tools/guied/GEViewer.cpp
	tools/guied/GEWindowWrapper.cpp
	#tools/guied/GEWindowWrapper_stub.cpp
	tools/guied/GEWorkspace.cpp
	tools/guied/GEWorkspaceFile.cpp
	tools/guied/GEZOrderModifier.cpp
	tools/guied/guied.cpp)

file(GLOB EDITOR_MATERIALEDITOR_INCLUDES tools/materialeditor/*.h)
file(GLOB EDITOR_MATERIALEDITOR_SOURCES tools/materialeditor/*.cpp)

file(GLOB EDITOR_PARTICLE_INCLUDES tools/particle/*.h)
file(GLOB EDITOR_PARTICLE_SOURCES tools/particle/*.cpp)

file(GLOB EDITOR_PDA_INCLUDES tools/pda/*.h)
file(GLOB EDITOR_PDA_SOURCES tools/pda/*.cpp)

file(GLOB EDITOR_RADIANT_INCLUDES tools/radiant/*.h)
file(GLOB EDITOR_RADIANT_SOURCES tools/radiant/*.cpp)

file(GLOB EDITOR_SCRIPT_INCLUDES tools/script/*.h)
file(GLOB EDITOR_SCRIPT_SOURCES tools/script/*.cpp)

file(GLOB EDITOR_SOUND_INCLUDES tools/sound/*.h)
file(GLOB EDITOR_SOUND_SOURCES tools/sound/*.cpp)

file(GLOB_RECURSE WIN32_INCLUDES sys/win32/*.h)
file(GLOB_RECURSE WIN32_SOURCES sys/win32/*.cpp)

if(ANGLE)
	list(REMOVE_ITEM WIN32_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/sys/win32/win_glimp.cpp)
else()
	list(REMOVE_ITEM WIN32_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/sys/win32/win_egl.cpp)
endif()

file(GLOB_RECURSE WIN32_RESOURCES sys/win32/rc/res/*.BMP sys/win32/rc/res/*.ico sys/win32/rc/res/*.cur)
if(USE_MFC_TOOLS)
	list(APPEND WIN32_RESOURCES sys/win32/rc/doom.rc)
else()
	list(APPEND WIN32_RESOURCES sys/win32/rc/doom_nomfc.rc)
endif()


set(POSIX_INCLUDES
	sys/posix/posix_public.h
	)

set(POSIX_SOURCES
	sys/posix/posix_net.cpp
	sys/posix/posix_main.cpp
	sys/posix/posix_signal.cpp
	sys/posix/posix_threads.cpp
	)

set(LINUX_INCLUDES
	sys/linux/sound.h
	)

set(LINUX_SOURCES
	sys/linux/linux_main.cpp
	sys/linux/sound.cpp
	sys/linux/sound_pulse.cpp
	sys/linux/stack.cpp
	)

set(SDL_INCLUDES
	sys/sdl/sdl_local.h
	)

set(SDL_SOURCES
	sys/sdl/sdl_events.cpp
	sys/sdl/sdl_glimp.cpp
	)

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
source_group("renderer\\Vulkan" FILES ${RENDERER_VULKAN_INCLUDES})
source_group("renderer\\Vulkan" FILES ${RENDERER_VULKAN_SOURCES})
source_group("renderer\\SMAA" FILES ${RENDERER_SMAA_INCLUDES})

source_group("renderer\\Shaders" FILES ${SHADERS})
source_group("renderer\\Shaders\\builtin" FILES ${SHADERS_BUILTIN})
source_group("renderer\\Shaders\\builtin\\debug" FILES ${SHADERS_BUILTIN_DEBUG})
source_group("renderer\\Shaders\\builtin\\fog" FILES ${SHADERS_BUILTIN_FOG})
source_group("renderer\\Shaders\\builtin\\legacy" FILES ${SHADERS_BUILTIN_LEGACY})
source_group("renderer\\Shaders\\builtin\\lighting" FILES ${SHADERS_BUILTIN_LIGHTING})
source_group("renderer\\Shaders\\builtin\\post" FILES ${SHADERS_BUILTIN_POST})
source_group("renderer\\Shaders\\builtin\\SSAO" FILES ${SHADERS_BUILTIN_SSAO})
source_group("renderer\\Shaders\\builtin\\SSGI" FILES ${SHADERS_BUILTIN_SSGI})
source_group("renderer\\Shaders\\builtin\\video" FILES ${SHADERS_BUILTIN_VIDEO})
source_group("renderer\\Shaders\\builtin\\VR" FILES ${SHADERS_BUILTIN_VR})

source_group("Sound" FILES ${SOUND_INCLUDES})
source_group("Sound" FILES ${SOUND_SOURCES})
source_group("UI" FILES ${UI_INCLUDES})
source_group("UI" FILES ${UI_SOURCES})
source_group("Sys" FILES ${SYS_INCLUDES})
source_group("Sys" FILES ${SYS_SOURCES})
#source_group("Sys\\win32" FILES ${WIN32_INCLUDES})
#source_group("Sys\\win32" FILES ${WIN32_SOURCES})
#source_group("Sys\\win32\\Resources" FILES ${WIN32_RESOURCES})
source_group("Sys\\Posix" FILES ${POSIX_INCLUDES})
source_group("Sys\\Posix" FILES ${POSIX_SOURCES})
source_group("Sys\\Common" FILES ${COMMON_INCLUDES})
source_group("Sys\\Common" FILES ${COMMON_SOURCES})
source_group("Sys\\SDL" FILES ${SDL_INCLUDES})
source_group("Sys\\SDL" FILES ${SDL_SOURCES})
source_group("Sys\\Win32" FILES ${WIN32_INCLUDES})
source_group("Sys\\Win32" FILES ${WIN32_SOURCES})
source_group("Sys\\Win32\\Resources" FILES ${WIN32_RESOURCES})

source_group("tools\\compilers" FILES ${COMPILER_INCLUDES})
source_group("tools\\compilers\\aas" FILES ${COMPILER_AAS_INCLUDES})
source_group("tools\\compilers\\aas" FILES ${COMPILER_AAS_SOURCES})
source_group("tools\\compilers\\dmap" FILES ${COMPILER_DMAP_INCLUDES})
source_group("tools\\compilers\\dmap" FILES ${COMPILER_DMAP_SOURCES})
source_group("tools\\compilers\\roqvq" FILES ${COMPILER_ROQVQ_INCLUDES})
source_group("tools\\compilers\\roqvq" FILES ${COMPILER_ROQVQ_SOURCES})
source_group("tools" FILES ${TOOLS_INCLUDES})
source_group("tools\\comafx" FILES ${TOOLS_COMAFX_INCLUDES})
source_group("tools\\comafx" FILES ${TOOLS_COMAFX_SOURCES})
source_group("tools\\common" FILES ${TOOLS_COMMON_INCLUDES})
source_group("tools\\common" FILES ${TOOLS_COMMON_SOURCES})
source_group("tools\\common\\proptree" FILES ${TOOLS_COMMON_PROPTREE_INCLUDES})
source_group("tools\\common\\proptree" FILES ${TOOLS_COMMON_PROPTREE_SOURCES})
source_group("tools\\af" FILES ${EDITOR_AF_INCLUDES})
source_group("tools\\af" FILES ${EDITOR_AF_SOURCES})
source_group("tools\\debugger" FILES ${EDITOR_DEBUGGER_INCLUDES})
source_group("tools\\debugger" FILES ${EDITOR_DEBUGGER_SOURCES})
source_group("tools\\decl" FILES ${EDITOR_DECL_INCLUDES})
source_group("tools\\decl" FILES ${EDITOR_DECL_SOURCES})
source_group("tools\\guied" FILES ${EDITOR_GUIED_INCLUDES})
source_group("tools\\guied" FILES ${EDITOR_GUIED_SOURCES})
source_group("tools\\materialeditor" FILES ${EDITOR_MATERIALEDITOR_INCLUDES})
source_group("tools\\materialeditor" FILES ${EDITOR_MATERIALEDITOR_SOURCES})
source_group("tools\\particle" FILES ${EDITOR_PARTICLE_INCLUDES})
source_group("tools\\particle" FILES ${EDITOR_PARTICLE_SOURCES})
source_group("tools\\pda" FILES ${EDITOR_PDA_INCLUDES})
source_group("tools\\pda" FILES ${EDITOR_PDA_SOURCES})
source_group("tools\\radiant" FILES ${EDITOR_RADIANT_INCLUDES})
source_group("tools\\radiant" FILES ${EDITOR_RADIANT_SOURCES})
source_group("tools\\script" FILES ${EDITOR_SCRIPT_INCLUDES})
source_group("tools\\script" FILES ${EDITOR_SCRIPT_SOURCES})
source_group("tools\\sound" FILES ${EDITOR_SOUND_INCLUDES})
source_group("tools\\sound" FILES ${EDITOR_SOUND_SOURCES})

set(TEKUUM_INCLUDES
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
	${RENDERER_OPENGL_INCLUDES}
	${SOUND_INCLUDES}
	${UI_INCLUDES})

set(TEKUUM_SOURCES
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
	${RENDERER_OPENGL_SOURCES}
	${SOUND_SOURCES}
	${UI_SOURCES})

add_definitions(-D__DOOM__
				-D__DOOM_DLL__
				-DBUILD_FREETYPE
				-DFT2_BUILD_LIBRARY)

if(USE_CMDLINE_TOOLS)
	add_definitions(-DUSE_CMDLINE_TOOLS)

	list(APPEND TEKUUM_INCLUDES
		${COMPILER_INCLUDES}
		${COMPILER_AAS_INCLUDES}
		${COMPILER_DMAP_INCLUDES}
		${COMPILER_ROQVQ_INCLUDES})

	list(APPEND TEKUUM_SOURCES
		${COMPILER_AAS_SOURCES}
		${COMPILER_DMAP_SOURCES}
		${COMPILER_ROQVQ_SOURCES})
endif()

include_directories(
			idlib
			libs/miniz
			libs/minizip
			libs/freetype/include)
if(MSVC)
	# if(MSVC_VERSION EQUAL 1700)
		 set(DirectX_LIBRARIES
			 dinput8
			 dsound
			 dxguid
			 #dxerr
			 Xinput
			)
		# add_definitions(-DUSE_XINPUT)
	# else()
		#find_package(DirectX REQUIRED)
		#include_directories(${DirectX_INCLUDE_DIR})
		#add_definitions(-DUSE_XINPUT)
	# endif()

	if(USE_MFC_TOOLS)
		# find_package(MFC REQUIRED)
		# if(MFC_FOUND)
			# include_directories(${MFC_INCLUDE_DIR})
			# link_directories(${MFC_LIBRARY_DIR})
		# endif()

		include_directories(libs/atlmfc/include)
		if(CMAKE_CL_64)
			link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/atlmfc/lib/amd64)
		else(CMAKE_CL_64)
			link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/atlmfc/lib)
		endif(CMAKE_CL_64)

		add_definitions(-DUSE_MFC_TOOLS)

		list(APPEND TEKUUM_INCLUDES
			${TOOLS_INCLUDES}
			${TOOLS_COMAFX_INCLUDES}
			${TOOLS_COMMON_INCLUDES}
			${TOOLS_COMMON_PROPTREE_INCLUDES}

			${EDITOR_AF_INCLUDES}
			#${EDITOR_DEBUGGER_INCLUDES}
			${EDITOR_DECL_INCLUDES}
			#${EDITOR_GUIED_INCLUDES}
			#${EDITOR_MATERIALEDITOR_INCLUDES}
			${EDITOR_PARTICLE_INCLUDES}
			${EDITOR_PDA_INCLUDES}
			${EDITOR_RADIANT_INCLUDES}
			${EDITOR_SCRIPT_INCLUDES}
			${EDITOR_SOUND_INCLUDES})

		list(APPEND TEKUUM_SOURCES
			${TOOLS_COMAFX_SOURCES}
			${TOOLS_COMMON_SOURCES}
			${TOOLS_COMMON_PROPTREE_SOURCES}

			${EDITOR_AF_SOURCES}
			#${EDITOR_DEBUGGER_SOURCES}
			${EDITOR_DECL_SOURCES}
			#${EDITOR_GUIED_SOURCES}
			#${EDITOR_MATERIALEDITOR_SOURCES}
			${EDITOR_PARTICLE_SOURCES}
			${EDITOR_PDA_SOURCES}
			${EDITOR_RADIANT_SOURCES}
			${EDITOR_SCRIPT_SOURCES}
			${EDITOR_SOUND_SOURCES})
	endif()

	list(APPEND TEKUUM_INCLUDES
		${SYS_INCLUDES}
		${WIN32_INCLUDES})

	list(APPEND TEKUUM_SOURCES
		${SYS_SOURCES}
		${WIN32_SOURCES})

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

	list( REMOVE_DUPLICATES TEKUUM_SOURCES )

	if( USE_PRECOMPILED_HEADERS )
		set( TEKUUM_PRECOMPILED_SOURCES ${TEKUUM_SOURCES} )
		list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/DXT/DXTDecoder.cpp )
		list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/DXT/DXTEncoder.cpp )
		list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/DXT/DXTEncoder_SSE2.cpp )
		list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/jobs/dynamicshadowvolume/DynamicShadowVolume.cpp )
		list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/jobs/prelightshadowvolume/PreLightShadowVolume.cpp )
		list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/jobs/staticshadowvolume/StaticShadowVolume.cpp )
		list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/jobs/ShadowShared.cpp )
		list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/RenderLog.cpp )

		#list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${TOOLS_COMAFX_SOURCES} )
		#list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${TOOLS_COMMON_SOURCES} )
		#list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${TOOLS_COMMON_PROPTREE_SOURCES} )

		# list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${EDITOR_AF_SOURCES} )
		# list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${EDITOR_DEBUGGER_SOURCES} )
		# list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${EDITOR_DECL_SOURCES} )
		# list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${EDITOR_GUIED_SOURCES} )
		# list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${EDITOR_MATERIALEDITOR_SOURCES} )
		# list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${EDITOR_PARTICLE_SOURCES} )
		# list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${EDITOR_PDA_SOURCES} )
		# list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${EDITOR_RADIANT_SOURCES} )
		# list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${EDITOR_SCRIPT_SOURCES} )
		# list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${EDITOR_SOUND_SOURCES} )

		#foreach( src_file ${TEKUUM_PRECOMPILED_SOURCES} )
		#	message(STATUS "-include precompiled.h for ${src_file}")
		#endforeach()

		set_source_files_properties(
			${TEKUUM_PRECOMPILED_SOURCES}
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

	add_executable( ${PROJECT_NAME} WIN32 ${TEKUUM_INCLUDES} ${TEKUUM_SOURCES} ${WIN32_RESOURCES} )

	add_dependencies(${PROJECT_NAME} idlib external)
	target_link_libraries(${PROJECT_NAME}
		idlib
		external
		GLEW::GLEW
		${OPENAL_LIBRARY}
		${FREETYPE_LIBRARY}
		${DirectX_LIBRARIES}
		${QT_LIBRARIES}
		#openal32
		opengl32
		glu32
		dbghelp
		#eaxguid
		iphlpapi
		winmm
		ws2_32)

	# set MSVC default debug directory on executable
	if( CMAKE_MAJOR_VERSION EQUAL 3 AND CMAKE_MINOR_VERSION GREATER_EQUAL 8 )
		set_target_properties( ${PROJECT_NAME} PROPERTIES VS_STARTUP_PROJECT ${PROJECT_NAME} )
		set_target_properties( ${PROJECT_NAME} PROPERTIES VS_DEBUGGER_COMMAND "${CMAKE_SOURCE_DIR}/../${PROJECT_NAME}.exe" )
		set_target_properties( ${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/.." )
		set_target_properties( ${PROJECT_NAME} PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "+set developer 1" )
	endif()

elseif( UNIX )

	if( SDL2 )
		find_package( SDL2 REQUIRED )
		include_directories( ${SDL2_INCLUDE_DIR} )
		set( SDLx_LIBRARY ${SDL2_LIBRARY})
	else()
		find_package( SDL REQUIRED )
		include_directories( ${SDL_INCLUDE_DIR} )
		set( SDLx_LIBRARY ${SDL_LIBRARY} )
	endif()

	add_definitions( -DUSE_SDL )
	add_definitions( -DUSE_SOUND_PULSE )
	add_definitions( -DUSE_EXCEPTIONS )

	list(APPEND TEKUUM_INCLUDES
		${SYS_INCLUDES}
		${POSIX_INCLUDES}
		${SDL_INCLUDES}
		${LINUX_INCLUDES})

	list(APPEND TEKUUM_SOURCES
		${SYS_SOURCES}
		${POSIX_SOURCES}
		${SDL_SOURCES}
		${LINUX_SOURCES})

	list(REMOVE_DUPLICATES TEKUUM_SOURCES)

	if( USE_PRECOMPILED_HEADERS )
		set( TEKUUM_PRECOMPILED_SOURCES ${TEKUUM_SOURCES})
		list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/DXT/DXTDecoder.cpp )
		list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/DXT/DXTEncoder.cpp )
		list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/DXT/DXTEncoder_SSE2.cpp )

		foreach( src_file ${TEKUUM_PRECOMPILED_SOURCES} )
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
		# we need to recreate the precompiled header for Tekuum
		# (i.e. can't use the one created for idlib before)
		# because some definitions (e.g. -D__IDLIB__ -D__DOOM_DLL__) differ
		add_custom_target( precomp_header_tekuum ALL
				COMMAND ${CMAKE_CXX_COMPILER} ${_compiler_FLAGS} -x c++-header idlib/precompiled.h -o idlib/precompiled.h.gch
				WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
				COMMENT "Creating idlib/precompiled.h.gch for Tekuum" )

		# make sure this is run after creating idlib
		add_dependencies( precomp_header_tekuum idlib )
	endif()

	add_executable( ${PROJECT_NAME} ${TEKUUM_INCLUDES} ${TEKUUM_SOURCES} )

	if ( USE_PRECOMPILED_HEADERS )
		# make sure precompiled header is created before executable is compiled
		add_dependencies( ${PROJECT_NAME} precomp_header_tekuum )

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
		GLEW::GLEW
		${OPENAL_LIBRARY}
		${FREETYPE_LIBRARY}
		GL
		pthread
		dl
		rt
		pulse
		pulse-simple
		${SDLx_LIBRARY})
endif()

set_target_properties( ${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY $<1:>${CMAKE_SOURCE_DIR}/.. LIBRARY_OUTPUT_DIRECTORY $<1:>${CMAKE_SOURCE_DIR}/.. )