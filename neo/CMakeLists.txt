cmake_minimum_required(VERSION 2.20)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

project(Tekuum C CXX)

option(USE_CMDLINE_TOOLS "Compile the built-in command line based tools" OFF)
option(USE_MFC_TOOLS "Compile the built-in MFC based tools" OFF)
option(USE_QT_TOOLS "Compile the built-in Qt based tools" OFF)
option(USE_QT_WINDOWING "Run game as Qt app" OFF)
option(STANDALONE "Skip Doom 3 base/ folder" OFF)
option(FORCE_COLOR_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." OFF)
option(COMPILE_COMMANDS "Generate compile_commands.json" OFF)
option(USE_PRECOMPILED_HEADERS "Use precompiled headers during build" ON)
option(GLES2 "Only use OpenGL ES 2.0 functions" OFF)
option(ANGLE "Only use OpenGL ES 2.0 functions through ANGLE" OFF)
option(MESA "Only use OpenGL ES 3.0 functions and turn off GPU skeletal animation" OFF)
option(DEBUG_MEMORY "Enables memory logging to file" OFF)
option(ONATIVE "Optimize for the host CPU" OFF)
option(RETAIL "Strip certain developer features and cheats from shipping builds" OFF)

#if(USE_QT_TOOLS)
#	set(USE_QT_WINDOWING TRUE)
#endif()

# if(ANGLE OR GLES2)
	# set(USE_CMDLINE_TOOLS FALSE)
# endif()

set( CPU_TYPE "" CACHE STRING "When set, passes this string as CPU-ID which will be embedded into the binary." )

# SRS - Turn on optimization when cross-compiling from Apple arm64 to x86_64
if ( CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)" OR CMAKE_OSX_ARCHITECTURES MATCHES "x86_64" )
	set( CPU_OPTIMIZATION "-mmmx -msse -msse2" CACHE STRING "Which CPU specific optimitations should be used beside the compiler's default?" )
endif()

# SRS - Turn off MMX/SSE intrinsics when cross-compiling from Apple x86_64 to arm64
if( CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)|(e2k)|(E2K)" AND NOT CMAKE_OSX_ARCHITECTURES MATCHES "arm64" )
	option( USE_INTRINSICS_SSE "Compile using SSE intrinsics (e.g mmx, sse, msse2)" ON )
else()
	option( USE_INTRINSICS_SSE "Compile using SSE intrinsics (e.g mmx, sse, msse2)" OFF )
endif()

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

	if( CPU_TYPE )
		add_definitions( -DCPUSTRING="${CPU_TYPE}" )
	endif()
	if ( CPU_OPTIMIZATION )
		add_definitions( ${CPU_OPTIMIZATION} )
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

	# SRS - Make sure OSX can find system headers and add support for minimum OSX runtime version
	if( APPLE )
		# SRS - Also add -fasm-blocks otherwise Xcode complains and -Qunused-arguments to silence MMX/SSE unused arg warnings when compiling for Apple arm64
		add_definitions( -fasm-blocks -Qunused-arguments )
		if( CMAKE_OSX_SYSROOT )
			add_definitions( -isysroot "${CMAKE_OSX_SYSROOT}" )
			message( STATUS "Using macOS sysroot: " ${CMAKE_OSX_SYSROOT} )
		endif()
		if( CMAKE_OSX_DEPLOYMENT_TARGET )
			add_definitions( -mmacosx-version-min="${CMAKE_OSX_DEPLOYMENT_TARGET}" )
		endif()
	endif()

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

if( DEBUG_MEMORY)
	add_definitions( -DID_DEBUG_MEMORY )
	add_definitions( -DID_REDIRECT_NEWDELETE )
endif()

if( RETAIL )
	add_definitions( -DID_RETAIL )
endif()

# SRS - on Apple set find_package() to prefer dylibs over macOS frameworks and xcframeworks
#     - required for cmake >= 3.29 and MoltenVK, also prefers openal-soft over Apple OpenAL
if( APPLE )
	set( CMAKE_FIND_FRAMEWORK LAST )
endif()

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
file(GLOB RENDERER_OPENGL_INCLUDES renderer/OpenGL/*.h)
file(GLOB RENDERER_OPENGL_SOURCES renderer/OpenGL/*.cpp)

set(SOUND_INCLUDES
	sound/snd_local.h
	sound/sound.h)

set(SOUND_SOURCES
	sound/snd_cache.cpp
	sound/snd_decoder.cpp
	#sound/snd_efxfile.cpp
	sound/snd_emitter.cpp
	sound/snd_shader.cpp
	sound/snd_system.cpp
	sound/snd_wavefile.cpp
	sound/snd_world.cpp)

file(GLOB SYS_INCLUDES sys/*.h)
file(GLOB SYS_SOURCES sys/*.cpp)

file(GLOB UI_INCLUDES ui/*.h)
file(GLOB UI_SOURCES ui/*.cpp)

file(GLOB UI_LUA_INCLUDES ui-lua/*.h)
file(GLOB UI_LUA_SOURCES ui-lua/*.cpp)

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

if(USE_QT_WINDOWING)

	set(WIN32_INCLUDES
		sys/win32/win_local.h
		sys/win32/rc/doom_resource.h
		)

	set(WIN32_SOURCES
		sys/win32/win_cpu.cpp
		sys/win32/win_main.cpp
		sys/win32/win_net.cpp
		sys/win32/win_shared.cpp
		sys/win32/win_snd.cpp
		#sys/win32/win_syscon.cpp
		sys/win32/win_taskkeyhook.cpp
		)

	set(WIN32_RESOURCES sys/win32/rc/res/doom.ico)

else()
	file(GLOB_RECURSE WIN32_INCLUDES sys/win32/*.h)
	file(GLOB_RECURSE WIN32_SOURCES sys/win32/*.cpp)

	if(ANGLE)
		list(REMOVE_ITEM WIN32_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/sys/win32/win_glimp.cpp)
	else()
		list(REMOVE_ITEM WIN32_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/sys/win32/win_egl.cpp)
	endif()

	set(WIN32_RESOURCES
		sys/win32/rc/res/BEVEL.BMP
		sys/win32/rc/res/BITMAP2.BMP
		sys/win32/rc/res/BMP00001.BMP
		sys/win32/rc/res/BMP0002.BMP
		sys/win32/rc/res/DEFTEX.WAL
		sys/win32/rc/res/ENDCAP.BMP
		sys/win32/rc/res/GetString.htm
		sys/win32/rc/res/IBEVEL.BMP
		sys/win32/rc/res/IENDCAP.BMP
		sys/win32/rc/res/MEFileToolbar.bmp
		sys/win32/rc/res/MEtoolbar.bmp
		sys/win32/rc/res/MaterialEditor.ico
		sys/win32/rc/res/PropTree.rc2
		sys/win32/rc/res/Q.BMP
		sys/win32/rc/res/RADIANT3.GIF
		sys/win32/rc/res/Radiant.ico
		sys/win32/rc/res/RadiantDoc.ico
		sys/win32/rc/res/TOOLBAR1.BMP
		sys/win32/rc/res/TOOLBAR2.BMP
		sys/win32/rc/res/Toolbar.bmp
		sys/win32/rc/res/VIEWDEFA.BMP
		sys/win32/rc/res/VIEWOPPO.BMP
		sys/win32/rc/res/bmp00002.bmp
		sys/win32/rc/res/bmp00003.bmp
		sys/win32/rc/res/bmp00004.bmp
		sys/win32/rc/res/bmp00005.bmp
		sys/win32/rc/res/cchsb.bmp
		sys/win32/rc/res/ccrgb.bmp
		sys/win32/rc/res/dbg_back.bmp
		sys/win32/rc/res/dbg_breakpoint.ico
		sys/win32/rc/res/dbg_current.ico
		sys/win32/rc/res/dbg_currentline.ico
		sys/win32/rc/res/dbg_empty.ico
		sys/win32/rc/res/dbg_open.bmp
		sys/win32/rc/res/dbg_toolbar.bmp
		sys/win32/rc/res/doom.ico
		sys/win32/rc/res/fpoint.cur
		sys/win32/rc/res/fxed_link.ico
		sys/win32/rc/res/fxed_toolbar.bmp
		sys/win32/rc/res/fxeditor.ico
		sys/win32/rc/res/guied.ico
		sys/win32/rc/res/guied_collapse.ico
		sys/win32/rc/res/guied_expand.ico
		sys/win32/rc/res/guied_hand.cur
		sys/win32/rc/res/guied_nav_visible.ico
		sys/win32/rc/res/guied_nav_visibledisabled.ico
		sys/win32/rc/res/guied_scripts.ico
		sys/win32/rc/res/guied_scripts_white.ico
		sys/win32/rc/res/guied_viewer_toolbar.bmp
		sys/win32/rc/res/icon2.ico
		sys/win32/rc/res/logo_sm3dfx.bmp
		sys/win32/rc/res/matedtree.bmp
		sys/win32/rc/res/me_disabled_icon.ico
		sys/win32/rc/res/me_enabled.ico
		sys/win32/rc/res/me_off_icon.ico
		sys/win32/rc/res/me_on_icon.ico
		sys/win32/rc/res/qe3.ico
		sys/win32/rc/res/shaderbar.bmp
		sys/win32/rc/res/shaderdoc.ico
		sys/win32/rc/res/shaderframe.ico
		sys/win32/rc/res/spliter.cur
		)
endif()


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


if(USE_QT_TOOLS)

	#set(QT_USE_QTOPENGL TRUE)
	#set(QT_USE_IMPORTED_TARGETS TRUE)

	set(CMAKE_INCLUDE_CURRENT_DIR ON)
	#set(CMAKE_AUTOMOC ON)

	if(MSVC)
		#set(CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.2.1-src\\5.2.1\\msvc2010_opengl")
		set(CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.2.1-src\\qtbase")

		if(CMAKE_CL_64)
			set(CMAKE_LIBRARY_PATH "C:\\Program Files (x86)\\Windows Kits\\8.0\\Lib\\win8\\um\\x64")
		else()
			#set(CMAKE_LIBRARY_PATH "C:\\Program Files (x86)\\Windows Kits\\8.0\\Lib\\win8\\um\\x86")
			set(CMAKE_LIBRARY_PATH "C:\\Program Files (x86)\\Microsoft SDKs\\Windows\\v7.1A\\Lib")
		endif()
	endif()

	find_package(Qt5Core REQUIRED)
	find_package(Qt5Widgets REQUIRED)
	find_package(Qt5OpenGL REQUIRED)

	include_directories(${Qt5Core_INCLUDE_DIRS})
	add_definitions(${Qt5Core_DEFINITIONS})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")

	include_directories(${Qt5Widgets_INCLUDE_DIRS})
	add_definitions(${Qt5Widgets_DEFINITIONS})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

	include_directories(${Qt5OpenGL_INCLUDE_DIRS})
	add_definitions(${Qt5OpenGL_DEFINITIONS})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5OpenGL_EXECUTABLE_COMPILE_FLAGS}")

	set(QT_LIBRARIES
		Qt5::Widgets
		Qt5::OpenGL)


	set(EDITOR_QTSTRING_INCLUDES
		tools/qtstringeditor/QtStringEditor.h)

	set(EDITOR_QTSTRING_SOURCES
		tools/qtstringeditor/main.cpp)

	set(EDITOR_QTSTRING_RESOURCES
		tools/qtstringeditor/QtStringEditor.qrc)

	set(EDITOR_QTSTRING_FORMS
		tools/qtstringeditor/QtStringEditor.ui)

	qt5_wrap_cpp(EDITOR_QTSTRING_INCLUDES_MOC ${EDITOR_QTSTRING_INCLUDES})
	qt5_wrap_ui(EDITOR_QTSTRING_FORMS_INCLUDES ${EDITOR_QTSTRING_FORMS})
	qt5_add_resources(EDITOR_QTSTRING_RESOURCES_RCC ${EDITOR_QTSTRING_RESOURCES})

	source_group("tools\\qtstringeditor\\Header Files" FILES ${EDITOR_QTSTRING_INCLUDES})
	source_group("tools\\qtstringeditor\\Source Files" FILES ${EDITOR_QTSTRING_SOURCES})
	source_group("tools\\qtstringeditor\\Resource Files" FILES ${EDITOR_QTSTRING_RESOURCES})
	source_group("tools\\qtstringeditor\\Form Files" FILES ${EDITOR_QTSTRING_FORMS})

	source_group("tools\\qtstringeditor\\Generated Files" FILES ${EDITOR_QTSTRING_INCLUDES_MOC})
	source_group("tools\\qtstringeditor\\Generated Files" FILES ${EDITOR_QTSTRING_FORMS_INCLUDES})
	source_group("tools\\qtstringeditor\\Generated Files" FILES ${EDITOR_QTSTRING_RESOURCES_RCC})



	file(GLOB EDITOR_QTRADIANT_INCLUDES	tools/qtradiant/src/*.h)
	file(GLOB EDITOR_QTRADIANT_SOURCES tools/qtradiant/src/*.cpp)

	#file(GLOB EDITOR_QTRADIANT_RESOURCES tools/qtradiant/resources/icons/*.png)
	set(EDITOR_QTRADIANT_RESOURCES tools/qtradiant/QtRadiant.qrc)
	file(GLOB EDITOR_QTRADIANT_FORMS tools/qtradiant/src/*.ui)

	qt5_wrap_cpp(EDITOR_QTRADIANT_INCLUDES_MOC ${EDITOR_QTRADIANT_INCLUDES})
	qt5_wrap_ui(EDITOR_QTRADIANT_FORMS_INCLUDES ${EDITOR_QTRADIANT_FORMS})
	qt5_add_resources(EDITOR_QTRADIANT_RESOURCES_RCC ${EDITOR_QTRADIANT_RESOURCES})

	source_group("tools\\qtradiant\\Header Files" FILES ${EDITOR_QTRADIANT_INCLUDES})
	source_group("tools\\qtradiant\\Source Files" FILES ${EDITOR_QTRADIANT_SOURCES})
	source_group("tools\\qtradiant\\Resource Files" FILES ${EDITOR_QTRADIANT_RESOURCES})
	source_group("tools\\qtradiant\\Form Files" FILES ${EDITOR_QTRADIANT_FORMS})

	source_group("tools\\qtradiant\\Generated Files" FILES ${EDITOR_QTRADIANT_INCLUDES_MOC})
	source_group("tools\\qtradiant\\Generated Files" FILES ${EDITOR_QTRADIANT_FORMS_INCLUDES})
	source_group("tools\\qtradiant\\Generated Files" FILES ${EDITOR_QTRADIANT_RESOURCES_RCC})

	if(USE_QT_WINDOWING)
		add_definitions(-DUSE_QT_WINDOWING)

		file(GLOB QTTEKUUM_INCLUDES sys/qt/*.h)
		file(GLOB QTTEKUUM_SOURCES sys/qt/*.cpp)

		set(QTTEKUUM_RESOURCES sys/qt/Tekuum.qrc)
		file(GLOB QTTEKUUM_FORMS sys/qt/*.ui)

		qt5_wrap_cpp(QTTEKUUM_INCLUDES_MOC ${QTTEKUUM_INCLUDES})
		qt5_wrap_ui(QTTEKUUM_FORMS_INCLUDES ${QTTEKUUM_FORMS})
		qt5_add_resources(QTTEKUUM_RESOURCES_RCC ${QTTEKUUM_RESOURCES})

		source_group("sys\\qt\\Header Files" FILES ${QTTEKUUM_INCLUDES})
		source_group("sys\\qt\\Source Files" FILES ${QTTEKUUM_SOURCES})
		source_group("sys\\qt\\Resource Files" FILES ${QTTEKUUM_RESOURCES})
		source_group("sys\\qt\\Form Files" FILES ${QTTEKUUM_FORMS})

		source_group("sys\\qt\\Generated Files" FILES ${QTTEKUUM_INCLUDES_MOC})
		source_group("sys\\qt\\Generated Files" FILES ${QTTEKUUM_FORMS_INCLUDES})
		source_group("sys\\qt\\Generated Files" FILES ${QTTEKUUM_RESOURCES_RCC})
	endif()

endif(USE_QT_TOOLS)

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
source_group("Sound" FILES ${SOUND_INCLUDES})
source_group("Sound" FILES ${SOUND_SOURCES})
source_group("UI" FILES ${UI_INCLUDES})
source_group("UI" FILES ${UI_SOURCES})
source_group("UI-Lua" FILES ${UI_LUA_INCLUDES})
source_group("UI-Lua" FILES ${UI_LUA_SOURCES})
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
#if(USE_QT_WINDOWING)
#	source_group("Sys\\Win32" FILES ${WIN32_INCLUDES})
#	source_group("Sys\\QT\\Win32" FILES ${WIN32_SOURCES})
#	source_group("Sys\\QT\\Win32\\Resources" FILES ${WIN32_RESOURCES})
#else()
	source_group("Sys\\Win32" FILES ${WIN32_INCLUDES})
	source_group("Sys\\Win32" FILES ${WIN32_SOURCES})
	source_group("Sys\\Win32\\Resources" FILES ${WIN32_RESOURCES})
#endif()

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


if(ANGLE OR GLES2)
	if(ANGLE)
		add_definitions(-DUSE_ANGLE)
		include_directories(libs/angle/include)
	endif()

	add_definitions(-DUSE_GLES3)
endif()

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
	${RENDERER_OPENGL_INCLUDES}
	${SOUND_INCLUDES})

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
	${SOUND_SOURCES})

if(STANDALONE)
	list(APPEND TEKUUM_INCLUDES
		${UI_LUA_INCLUDES}
		${LUA_INCLUDES})

	list(APPEND TEKUUM_SOURCES
		${UI_LUA_SOURCES})

	add_definitions(-DUSE_LUA)

	include_directories(
			libs/lua/src
			libs/luawrapper)
else()
	list(APPEND TEKUUM_INCLUDES ${UI_INCLUDES})
	list(APPEND TEKUUM_SOURCES ${UI_SOURCES})
endif()

if(NOT ANGLE)
	list(APPEND TEKUUM_INCLUDES ${GLEW_INCLUDES})
	list(APPEND TEKUUM_SOURCES ${GLEW_SOURCES})
endif()

add_definitions(-D__DOOM__
				-D__DOOM_DLL__
				-DGLEW_STATIC
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
			libs/curl/include
			#libs/openal/include
			libs/glew/include
			libs/freetype/include
			libs/zlib)

link_directories(
			${CMAKE_CURRENT_SOURCE_DIR}/libs/curl/lib
			${CMAKE_CURRENT_SOURCE_DIR}/libs/openal/lib)

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

		# find_package(ATL REQUIRED)
		# if(ATL_FOUND)
			# include_directories(${ATL_INCLUDE_DIR})
			# link_directories(${ATL_LIBRARY_DIR})
			# add_definitions(-DUSE_MFC6_WITH_ATL7)
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

	elseif(USE_QT_TOOLS)

		add_definitions(-DUSE_QT_TOOLS) # ${Qt5_DEFINITIONS})
		#include(${QT_USE_FILE})

		# As all intermediate files are generated outside the source tree,
		# the header file generated by uic will not be located by the compiler.
		# In order to handle this, the build directory needs to be added to the list of include directories.
		#include_directories(${CMAKE_CURRENT_BINARY_DIR})

		list(APPEND TEKUUM_INCLUDES
			${TOOLS_INCLUDES}

			${QTTEKUUM_INCLUDES}
			${QTTEKUUM_SOURCES}
			${QTTEKUUM_RESOURCES}

			${EDITOR_QTRADIANT_INCLUDES}
			${EDITOR_QTRADIANT_SOURCES}
			${EDITOR_QTRADIANT_RESOURCES}
			${EDITOR_QTRADIANT_FORMS}

			${EDITOR_QTRADIANT_INCLUDES_MOC}
			${EDITOR_QTRADIANT_FORMS_INCLUDES}
			${EDITOR_QTRADIANT_RESOURCES_RCC}

			${EDITOR_QTSTRING_INCLUDES}
			${EDITOR_QTSTRING_SOURCES}
			${EDITOR_QTSTRING_RESOURCES}
			${EDITOR_QTSTRING_FORMS}

			${EDITOR_QTSTRING_INCLUDES_MOC}
			${EDITOR_QTSTRING_FORMS_INCLUDES}
			${EDITOR_QTSTRING_RESOURCES_RCC}
			)

		if(USE_QT_WINDOWING)
			list(APPEND TEKUUM_SOURCES
				${QTTEKUUM_INCLUDES}
				${QTTEKUUM_SOURCES}
				${QTTEKUUM_RESOURCES}
				${QTTEKUUM_FORMS}

				${QTTEKUUM_INCLUDES_MOC}
				${QTTEKUUM_FORMS_INCLUDES}
				${QTTEKUUM_RESOURCES_RCC})
		endif(USE_QT_WINDOWING)
	endif()

	list(APPEND TEKUUM_INCLUDES
		${SYS_INCLUDES}
		${WIN32_INCLUDES})

	list(APPEND TEKUUM_SOURCES
		${SYS_SOURCES}
		${WIN32_SOURCES})


	list( REMOVE_DUPLICATES TEKUUM_SOURCES )

	if( USE_PRECOMPILED_HEADERS )
		set( TEKUUM_PRECOMPILED_SOURCES ${TEKUUM_SOURCES} )
		list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/game/gamesys/TypeInfo.cpp )
		list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/ui-lua/Lua_init.cpp )
		list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/DXT/DXTDecoder.cpp )
		list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/DXT/DXTEncoder.cpp )
		list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/DXT/DXTEncoder_SSE2.cpp )
		list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/jobs/dynamicshadowvolume/DynamicShadowVolume.cpp )
		list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/jobs/prelightshadowvolume/PreLightShadowVolume.cpp )
		list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/jobs/staticshadowvolume/StaticShadowVolume.cpp )
		list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/jobs/ShadowShared.cpp )
		list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer/RenderLog.cpp )

		if( USE_QT_WINDOWING )
			list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${QTTEKUUM_SOURCES} )
			list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${QTTEKUUM_INCLUDES_MOC} )
			list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${QTTEKUUM_RESOURCES} )
			list( REMOVE_ITEM TEKUUM_PRECOMPILED_SOURCES ${QTTEKUUM_RESOURCES_RCC} )
		endif()

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

	if(USE_QT_WINDOWING)
		add_executable(Tekuum ${TEKUUM_INCLUDES} ${TEKUUM_SOURCES} ${QTTEKUUM_INCLUDES} ${QTTEKUUM_SOURCES} ${QTTEKUUM_RESOURCES})
	else()
		add_executable(Tekuum WIN32 ${TEKUUM_INCLUDES} ${TEKUUM_SOURCES} ${WIN32_RESOURCES})
	endif()

	if(ANGLE)
		add_dependencies(Tekuum idlib external libANGLE)
		target_link_libraries(Tekuum
			idlib
			external
			libANGLE
			${DirectX_LIBRARIES}
			${QT_LIBRARIES}
			#libcurl
			#openal32
			#opengl32
			glu32
			dbghelp
			eaxguid
			iphlpapi
			winmm
			ws2_32.lib)
	else()
		add_dependencies(Tekuum idlib external)
			target_link_libraries(Tekuum
			idlib
			external
			${DirectX_LIBRARIES}
			${QT_LIBRARIES}
			#libcurl
			#openal32
			opengl32
			glu32
			dbghelp
			eaxguid
			iphlpapi
			winmm
			ws2_32.lib)
	endif()

	# set MSVC default debug directory on executable
	if( CMAKE_MAJOR_VERSION EQUAL 3 AND CMAKE_MINOR_VERSION GREATER_EQUAL 8 )
		set_target_properties( Tekuum PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/.." )
	endif()

elseif(UNIX)

    if(SDL2)
        find_package(SDL2 REQUIRED)
        include_directories(${SDL2_INCLUDE_DIR})
        set(SDLx_LIBRARY ${SDL2_LIBRARY})
    else()
        find_package(SDL REQUIRED)
        include_directories(${SDL_INCLUDE_DIR})
        set(SDLx_LIBRARY ${SDL_LIBRARY})
    endif()

    add_definitions(-DUSE_SDL)
    add_definitions(-DUSE_SDL_ASYNC)
    add_definitions(-DUSE_SOUND_PULSE)
	add_definitions(-DUSE_EXCEPTIONS)

	if(MESA)
		add_definitions(-DUSE_GLES3)
		add_definitions(-DUSE_MESA)
	endif()

    #find_package(PulseAudio REQUIRED)

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

	add_executable( tekuum ${TEKUUM_INCLUDES} ${TEKUUM_SOURCES} )

	if ( USE_PRECOMPILED_HEADERS )
		# make sure precompiled header is created before executable is compiled
		add_dependencies( tekuum precomp_header_tekuum )

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
			add_dependencies( rm_precomp_header tekuum )

		# delete precompiled header file after executable is compiled: IDE build case (e.g. Xcode)
		else()
			add_custom_command( TARGET tekuum POST_BUILD
					# SRS - added wildcards to remove tmp files from cmake ZERO_CHECK regeneration
					COMMAND ${remove_command} "idlib/precompiled.h*.gch*"
					WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
					COMMENT "remove idlib/precompiled.h.gch" )
		endif()
	endif()

	target_link_libraries(tekuum
        idlib
		external
        GL
        pthread
        dl
        rt
        pulse
        pulse-simple
        ${SDLx_LIBRARY})
endif()
