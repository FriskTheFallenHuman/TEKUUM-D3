--
-- Techyon build configuration script
-- 
solution "Techyon"
	--configurations { "Release", "ReleaseWithSymbols", "Debug" }
	configurations { "Release", "Debug" }
	platforms {"x32", "x64"}
	
	--
	-- Release/Debug Configurations
	--
	configuration "Release"
		defines     "NDEBUG"
		flags      
		{
			"OptimizeSpeed",
			--"EnableSSE",
			--"StaticRuntime"
		}
		
	-- configuration "ReleaseWithSymbols"
		-- defines     "NDEBUG"
		-- flags
		-- {
			-- "OptimizeSpeed",
			-- --"EnableSSE",
			-- "Symbols",
			-- --"StaticRuntime"
		-- }
	
	configuration "Debug"
		defines     "_DEBUG"
		flags
		{
			"Symbols",
			--"StaticRuntime",
			--"NoRuntimeChecks"
		}
	
	
	configuration { "vs*", "Release" }
		buildoptions
		{
			-- Produces a program database (PDB) that contains type information and symbolic debugging information for use with the debugger
			-- /Zi does imply /debug
			"/Zi",
			
			-- turn off Whole Program Optimization
			"/GL-",
			
			-- Inline Function Expansion: Any Suitable (/Ob2)
			"/Ob2",
			
			-- enable Intrinsic Functions
			"/Oi",
			
			-- Omit Frame Pointers
			"/Oy",
		}
		-- linkoptions
		-- {
			-- -- turn off Whole Program Optimization
			-- "/LTCG-",
		-- }
		-- links
		-- { 
			-- "libcmt",
		-- }
	
--
-- Options
--
newoption
{
	trigger = "debug-memory",
	description = "Enables memory logging to file"
}

newoption
{
	trigger = "no-lanaddress",
	description = "Don't recognize any IP as LAN address. This is useful when debugging network	code where LAN / not LAN influences application behaviour"
}

-- newoption
-- {
	-- trigger = "memcheck"
	-- description = "Perform heap consistency checking",
	-- value = "TYPE",
	-- allowed = 
	-- {
		-- { "0", "on in Debug / off in Release" },
		-- { "1", "forces on" },
		-- { "2", "forces off" },
	-- }
-- }

newoption
{
	trigger = "mfc-tools",
	description = "Enable original Doom 3 tools"
}

newoption
{
	trigger = "gtk-tools",
	description = "Enable GTK+ based extra tools"
}

-- newoption
-- {
	-- trigger = "qt-tools",
	-- description = "Enable Qt based extra tools"
-- }

--newoption
--{
--	trigger = "with-freetype",
--	description = "Compile with freetype support"
--}
		
--newoption
--{
--	trigger = "with-openal",
--	value = "TYPE",
--	description = "Specify which OpenAL library",
--	allowed = 
--	{
--		{ "none", "No support for OpenAL" },
--		{ "dlopen", "Dynamically load OpenAL library if available" },
--		{ "link", "Link the OpenAL library as normal" },
--		{ "openal-dlopen", "Dynamically load OpenAL library if available" },
--		{ "openal-link", "Link the OpenAL library as normal" }
--	}
--}

--		
-- Platform specific defaults
--

-- We don't support freetype on VS platform
--if _ACTION and string.sub(_ACTION, 2) == "vs" then
--	_OPTIONS["with-freetype"] = false
--end

-- Default to dlopen version of OpenAL
--if not _OPTIONS["with-openal"] then
--	_OPTIONS["with-openal"] = "dlopen"
--end
--if _OPTIONS["with-openal"] then
--	_OPTIONS["with-openal"] = "openal-" .. _OPTIONS["with-openal"]
--end

function FindDirectXSDK()
	
	configuration {}
	local dxsdkdir = os.getenv("DXSDK_DIR")
	if (dxsdkdir) then
		includedirs { "$(DXSDK_DIR)/include" }
		configuration "x32"
			libdirs {"$(DXSDK_DIR)/lib/x86"}
		configuration "x64"
			libdirs {"$(DXSDK_DIR)/lib/x64"}
		configuration {}
		print("Found DirectX SDK at '" .. dxsdkdir .. "'")
		return true
	end
	
	return false
end

function FindPlatformSDK()
	
	configuration {}
	local platformsdkdir = os.getenv("PLATFORMSDK_DIR")
	if (platformsdkdir) then
		includedirs {
			"$(PLATFORMSDK_DIR)/include/mfc",
			"$(PLATFORMSDK_DIR)/include/atl"			
		}
		configuration "x32"
			libdirs {"$(PLATFORMSDK_DIR)/lib"}
		configuration "x64"
			libdirs {"$(PLATFORMSDK_DIR)/lib/amd64"}
		configuration {}
		print("Found Platform SDK at '" .. platformsdkdir .. "'")
		return true
	end
	
	return false
end

function FindWinDDK()
	
	configuration {}
	local platformsdkdir = os.getenv("WINDDK_DIR")
	if (platformsdkdir) then
		includedirs 
		{
			"$(WINDDK_DIR)/inc/mfc42",
			"$(WINDDK_DIR)/inc/atl71"			
		}
		defines
		{
			"USE_MFC6_WITH_ATL7",
		}
		configuration "x32"
			libdirs 
			{
			"$(WINDDK_DIR)/lib/mfc/i386",
			"$(WINDDK_DIR)/lib/atl/i386"
			}
		configuration "x64"
			libdirs 
			{
			"$(WINDDK_DIR)/lib/mfc/amd64",
			"$(WINDDK_DIR)/lib/atl/amd64"
			}
		configuration {}
		print("Found Windows Driver Development Kit at '" .. platformsdkdir .. "'")
		return true
	end
	
	return false
end

-- function FindQtSDK()
	
	-- configuration {}
	-- local qtsdkdir = os.getenv("QTDIR")
	-- if (qtsdkdir) then
		-- -- includedirs {
			-- -- "$(QTDIR)/include",
			-- -- "$(QTDIR)/include/qtmain",
			-- -- "$(QTDIR)/include/QtCore",
			-- -- "$(QTDIR)/include/QtGui",
			-- -- "$(QTDIR)/include/QtOpenGL",
		-- -- }
		-- -- configuration "x32"
			-- -- libdirs {"$(QTDIR)/lib"}
		
		-- -- FIXME 64 bit support
		-- --configuration "x64"
		-- --	libdirs {"$(QTDIR)/lib/amd64"}
		-- configuration {}
		-- print("Found Qt SDK at '" .. qtsdkdir .. "'")
		-- return true
	-- end
	
	-- return false
-- end

function FindGtkmmSDK()
	
	configuration {}
	local gtkmmsdkdir = os.getenv("GTKMM_BASEPATH")
	if (gtkmmsdkdir) then
		configuration {}
		print("Found Gtkmm SDK at '" .. gtkmmsdkdir .. "'")
		return true
	end
	
	return false
end

if _ACTION == "vs2010" then	
	foundDirectSDK = FindDirectXSDK()
	--foundPlatformSDK = FindPlatformSDK()
	foundWinDDK = FindWinDDK()
	--foundQtSDK = FindQtSDK()
	foundGtkMMSDK = FindGtkmmSDK()
end
	
	
include "idlib"

project "Techyon"
	targetname  "Techyon"
	language    "C++"
	kind        "WindowedApp"
	flags       { "ExtraWarnings" }
	--debugargs	{ "+set com_allowConsole 1 +set fs_game basety" }
	files
	{
		"cm/*.cpp", "cm/*.h",
		"framework/**.cpp", "framework/**.h",
		"renderer/**.c", "renderer/**.cpp", "renderer/**.h",
		
		"libs/glew/src/glew.c",
		"libs/glew/include/GL/glew.h",
		
		"sound/*.cpp", "sound/*.h",
		
		"sound/OggVorbis/ogg/*.h",
		"sound/OggVorbis/oggsrc/bitwise.c",
		"sound/OggVorbis/oggsrc/framing.c",
		
		"sound/OggVorbis/vorbis/*.h",
		"sound/OggVorbis/vorbissrc/mdct.c",
		"sound/OggVorbis/vorbissrc/smallft.c",
		"sound/OggVorbis/vorbissrc/block.c",
		"sound/OggVorbis/vorbissrc/envelope.c",
		"sound/OggVorbis/vorbissrc/windowvb.c",
		"sound/OggVorbis/vorbissrc/lsp.c",
		"sound/OggVorbis/vorbissrc/lpc.c",
		"sound/OggVorbis/vorbissrc/analysis.c",
		"sound/OggVorbis/vorbissrc/synthesis.c",
		"sound/OggVorbis/vorbissrc/psy.c",
		"sound/OggVorbis/vorbissrc/info.c",
		"sound/OggVorbis/vorbissrc/floor1.c",
		"sound/OggVorbis/vorbissrc/floor0.c",
		"sound/OggVorbis/vorbissrc/res0.c",
		"sound/OggVorbis/vorbissrc/mapping0.c",
		"sound/OggVorbis/vorbissrc/registry.c",
		"sound/OggVorbis/vorbissrc/codebook.c",
		"sound/OggVorbis/vorbissrc/sharedbook.c",
		"sound/OggVorbis/vorbissrc/lookup.c",
		"sound/OggVorbis/vorbissrc/bitrate.c",
		"sound/OggVorbis/vorbissrc/vorbisfile.c",
		
		"sys/sys_*.cpp", "sys/sys_*.h",
		
		"tools/compilers/**.cpp", "tools/*.h",
		"ui/*.cpp", "ui/*.h",
	}
	excludes
	{
		"renderer/draw_exp*.cpp",
		"renderer/jpeg-6/jmemdos.c",
		"renderer/jpeg-6/jload.c",
		"renderer/jpeg-6/jpegtran.c",
		
		"tools/common/RenderBumpFlatDialog.*",
		"tools/debugger/*",
		"tools/decl/DialogEntityDefEditor.*",
		"tools/edit_stub.cpp",
		"tools/guied/GEWindowWrapper_stub.cpp",
		"tools/qttest/*",
		"tools/gtktest/*",
	}
	includedirs
	{
		--"../shared",
		--"../libs/zlib",
		"libs/glew/include",
		--"../libs/freetype/include",
		--"../libs/ogg/include",
		"sound/OggVorbis/ogg",
		"sound/OggVorbis/vorbis",
	}
	defines
	{
		"__DOOM__",
		"__DOOM_DLL__",
		"GLEW_STATIC",
	}
	links
	{
		"idlib",
	}
	
	--
	-- Platform Configurations
	-- 	
	configuration "x32"
		files
		{ 
			--"code/qcommon/vm_x86.c",
		}
	
	configuration "x64"
		--targetdir 	"../../bin64"
		files
		{ 
			--"qcommon/vm_x86_64.c",
			--"qcommon/vm_x86_64_assembler.c",
		}
		
	--
	-- Options Configurations
	--
	configuration { "mfc-tools", "vs*", "x32" }
		flags       { "MFC" }
		defines
		{
			"USE_MFC_TOOLS",
		}
		files
		{
			"tools/af/*.cpp", "tools/af/*.h",
			"tools/comafx/*.cpp", "tools/comafx/*.h",
			"tools/decl/*.cpp", "tools/decl/*.h",
			"tools/guied/*.cpp", "tools/guied/*.h",
			"tools/materialeditor/*.cpp", "tools/materialeditor/*.h",
			"tools/particle/*.cpp", "tools/particle/*.h",
			"tools/pda/*.cpp", "tools/pda/*.h",
			"tools/radiant/*.cpp", "tools/radiant/*.h",
			"tools/script/*.cpp", "tools/script/*.h",
			"tools/sound/*.cpp", "tools/sound/*.h",
			
			"sys/win32/rc/doom.rc",
		}
		excludes
		{
			"sys/win32/rc/doom_nomfc.rc",
		}
	
	
	-- configuration "qt-tools"
		-- defines
		-- {
			-- "USE_QT"
		-- }
		-- includedirs
		-- {
			-- --"../libs/qt"
		-- }
		-- files
		-- {
			-- "tools/qttest/*",
		-- }
		
		-- -- Files can be customized at the project scope and below.
		-- customizefile "qttest.h"
			-- filebuilddescription "Generating file using blah blah tool"
			-- filebuildcommands
			-- {
				-- "$(QTDIR)\bin\moc.exe -DUNICODE -DWIN32 -DQT_LARGEFILE_SUPPORT -DQT_CORE_LIB -DQT_GUI_LIB -DQT_OPENGL_LIB  -I. -I.\GeneratedFiles "-I$(QTDIR)\include" "-I.\GeneratedFiles\$(ConfigurationName)\." "-I$(QTDIR)\include\qtmain" "-I$(QTDIR)\include\QtCore" "-I$(QTDIR)\include\QtGui" "-I$(QTDIR)\include\QtOpenGL" "-I." "-I." "-I." "-I." "qttest1.h" -o ".\GeneratedFiles\$(ConfigurationName)\moc_%(Filename).cpp"",
				-- "copy some.file generated.cpp"
			-- }
			-- filebuildadditionaldependencies { "*.h" }
			-- filebuildoutputs { "generated.cpp" }
		
	-- configuration { "qt-tools", "vs*", "x32" }
		-- includedirs
		-- {
			-- "$(QTDIR)/include",
			-- "$(QTDIR)/include/qtmain",
			-- "$(QTDIR)/include/QtCore",
			-- "$(QTDIR)/include/QtGui",
			-- "$(QTDIR)/include/QtOpenGL",
		-- }
		-- libdirs
		-- {
			-- "$(QTDIR)/lib"
		-- }
		
	-- configuration { "qt-tools", "Release" }
		-- links
		-- {
			-- "qtmain",
			-- "QtCore4",
			-- "QtGui4",
			-- "QtOpenGL4",
		-- }
		
	-- configuration { "qt-tools", "Debug" }
		-- links
		-- {
			-- "qtmaind",
			-- "QtCored4",
			-- "QtGuid4",
			-- "QtOpenGLd4",
		-- }
	
	configuration "gtk-tools"
		defines
		{
			"USE_GTK"
		}
		includedirs
		{
			--"../libs/qt"
		}
		files
		{
			"tools/gtktest/*.cpp", "tools/gtktest/*.h",
		}
		
	configuration { "gtk-tools", "vs*", "x32" }
		includedirs
		{
			"$(GTKMM_BASEPATH)/include",
			"$(GTKMM_BASEPATH)/include/gtkmm-2.4",
			"$(GTKMM_BASEPATH)/lib/gtkmm-2.4/include",
			"$(GTKMM_BASEPATH)/include/atkmm-1.6",
			"$(GTKMM_BASEPATH)/include/giomm-2.4",
			"$(GTKMM_BASEPATH)/lib/giomm-2.4/include",
			"$(GTKMM_BASEPATH)/include/pangomm-1.4",
			"$(GTKMM_BASEPATH)/lib/pangomm-1.4/include",
			"$(GTKMM_BASEPATH)/include/gtk-2.0",
			"$(GTKMM_BASEPATH)/include/gdkmm-2.4",
			"$(GTKMM_BASEPATH)/lib/gdkmm-2.4/include",
			"$(GTKMM_BASEPATH)/include/atk-1.0",
			"$(GTKMM_BASEPATH)/include/glibmm-2.4",
			"$(GTKMM_BASEPATH)/lib/glibmm-2.4/include",
			"$(GTKMM_BASEPATH)/include/glib-2.0",
			"$(GTKMM_BASEPATH)/lib/glib-2.0/include",
			"$(GTKMM_BASEPATH)/include/sigc++-2.0",
			"$(GTKMM_BASEPATH)/lib/sigc++-2.0/include",
			"$(GTKMM_BASEPATH)/include/cairomm-1.0",
			"$(GTKMM_BASEPATH)/lib/cairomm-1.0/include",
			"$(GTKMM_BASEPATH)/include/pango-1.0",
			"$(GTKMM_BASEPATH)/include/cairo",
			"$(GTKMM_BASEPATH)/include",
			"$(GTKMM_BASEPATH)/include/freetype2",
			"$(GTKMM_BASEPATH)/include/libpng14",
			"$(GTKMM_BASEPATH)/lib/gtk-2.0/include",
			"$(GTKMM_BASEPATH)/include/gdk-pixbuf-2.0",
		}
		libdirs
		{
			"$(GTKMM_BASEPATH)/lib",
		}
		
	--configuration { "gtk-tools", "vs2010", "Release" }
	configuration { "gtk-tools", "vs2010" }
		links
		{
			"gtkmm-vc100-2_4",
			"atkmm-vc100-1_6",
			"gdkmm-vc100-2_4",
			"giomm-vc100-2_4",
			"pangomm-vc100-1_4",
			"gtk-win32-2.0",
			"glibmm-vc100-2_4",
			"cairomm-vc100-1_0",
			"sigc-vc100-2_0",
			"gdk-win32-2.0",
			"atk-1.0",
			"gio-2.0",
			"pangowin32-1.0",
			"gdi32",
			"pangocairo-1.0",
			"gdk_pixbuf-2.0",
			--"png14",
			"pango-1.0",
			"cairo",
			"gobject-2.0",
			"gmodule-2.0",
			"gthread-2.0",
			"glib-2.0",
			"intl",
		}
		
	-- configuration { "gtk-tools", "vs2010", "Debug" }
		-- links
		-- {
			-- "gtkmm-vc100-d-2_4",
			-- ...
		-- }
	
	-- 
	-- Project Configurations
	-- 
	configuration "vs*"
		flags       { "WinMain" }
		files
		{
			"sys/win32/**.cpp", "sys/win32/**.h",
			--"sys/win32/rc/doom.rc",
			"sys/win32/rc/doom_nomfc.rc",
			--"sys/win32/res/*",
			
			"libs/glew/include/GL/wglew.h",
			
			--"openal/idal.cpp", "openal/idal.h",
			"openal/include/*.h",
		}
		excludes
		{
			"sys/win32/win_gamma.cpp",
			--"sys/win32/win_snd.cpp",
		}
		defines
		{
			--"USE_OPENAL",
			--"ID_ALLOW_TOOLS",
		}
		includedirs
		{
			"curl/include",
			"openal/include",
			--"libs/sdl/include",
		}
		libdirs
		{
			--"openal/lib",
			--"../libs/curl-7.12.2/lib"
		}
		links
		{
			"idlib",
		}
		buildoptions
		{
			--"/MT"
		}
		linkoptions
		{
			"/LARGEADDRESSAWARE",
			"/DYNAMICBASE",
			"/STACK:16777216",
			--"/NODEFAULTLIB:libcmt.lib",
			--"/NODEFAULTLIB:libcmtd.lib"
			--"/NODEFAULTLIB:libc",
		}
		defines
		{
			"WIN32",
			"_WINDOWS",
			"_CRT_SECURE_NO_DEPRECATE",
			"_CRT_NONSTDC_NO_DEPRECATE",
			--"_CRT_SECURE_NO_WARNINGS",
			"_USE_32BIT_TIME_T",
			"_MBCS",
		}
		
		
	-- configuration { "vs*", "Debug" }
		-- links
		-- { 
			-- "libcmtd",
		-- }
		
	configuration { "vs*", "x32" }
		targetdir 	"../bin/win32"
		libdirs
		{
			--"../libs/sdl/lib",
			--"../libs/openal/libs/win32",
			"openal/lib",
			--"../libs/curl-7.12.2/lib"
			"curl/lib"
		}
		links
		{ 
			"libcurl",
			"openal32",
			"opengl32",
			"glu32",
			"dbghelp",
			"dinput8",
			"dsound",
			"dxguid",
			"DxErr",
			"eaxguid",
			"iphlpapi",
			"winmm",
			"ws2_32",
		}
		prebuildcommands
		{
		   "cd curl/lib",
		   "nmake /f Makefile.vc6 CFG=release",
		}
		
	configuration { "vs*", "x64" }
		targetdir 	"../bin/win64"
		libdirs
		{
			--"../libs/sdl/lib64",
			--"../libs/openal/libs/win64",
			"openal/lib",
			--"../libs/curl-7.12.2/lib"
		}
		links
		{ 
			--"libcurl",
			"OpenAL32",
		}

	configuration { "linux", "gmake" }
		buildoptions
		{
			--"`pkg-config --cflags x11`",
			--"`pkg-config --cflags xext`",
			--"`pkg-config --cflags xxf86dga`",
			--"`pkg-config --cflags xxf86vm`",
			"`pkg-config --cflags sdl`",
			"`pkg-config --cflags libcurl`",
		}
		linkoptions
		{
			--"`pkg-config --libs x11`",
			--"`pkg-config --libs xext`",
			--"`pkg-config --libs xxf86dga`",
			--"`pkg-config --libs xxf86vm`",
			"`pkg-config --libs sdl`",
			"`pkg-config --libs libcurl`",
		}
		links
		{ 
			--"libcurl",
			"openal",
		}
	
	configuration "linux"
		targetname  "xreal"
		files
		{
			"sys/sys_main.c",
			"sys/sys_unix.c",
			"sys/con_log.c",
			"sys/con_passive.c",
			"sys/sdl_gamma.c",
			"sys/sdl_glimp.c",
			"sys/sdl_input.c",
			"sys/sdl_snd.c",
			"../libs/glew/src/glew.c",
		}
		--buildoptions
		--{
		--	"-pthread"
		--}
		links
		{
			"GL",
		}
		defines
		{
            "PNG_NO_ASSEMBLER_CODE",
		}

		
include "TypeInfo"
include "game"
