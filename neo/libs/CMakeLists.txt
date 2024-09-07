project(external C CXX)

file(GLOB IRRXML_INCLUDES irrxml/src/*.h)
file(GLOB IRRXML_SOURCES irrxml/src/*.cpp)
	
set(JPEG_INCLUDES
	jpeg-6/jchuff.h
	jpeg-6/jconfig.h
	jpeg-6/jdct.h
	jpeg-6/jdhuff.h
	jpeg-6/jerror.h
	jpeg-6/jinclude.h
	jpeg-6/jmemsys.h
	jpeg-6/jmorecfg.h
	jpeg-6/jpegint.h
	jpeg-6/jpeglib.h
	jpeg-6/jversion.h)
	
set(JPEG_SOURCES
	jpeg-6/jcapimin.c
	jpeg-6/jcapistd.c
	jpeg-6/jccoefct.c
	jpeg-6/jccolor.c
	jpeg-6/jcdctmgr.c
	jpeg-6/jchuff.c
	jpeg-6/jcinit.c
	jpeg-6/jcmainct.c
	jpeg-6/jcmarker.c
	jpeg-6/jcmaster.c
	jpeg-6/jcomapi.c
	jpeg-6/jcparam.c
	jpeg-6/jcphuff.c
	jpeg-6/jcprepct.c
	jpeg-6/jcsample.c
	jpeg-6/jctrans.c
	jpeg-6/jdapimin.c
	jpeg-6/jdapistd.c
	jpeg-6/jdatadst.c
	jpeg-6/jdatasrc.c
	jpeg-6/jdcoefct.c
	jpeg-6/jdcolor.c
	jpeg-6/jddctmgr.c
	jpeg-6/jdhuff.c
	jpeg-6/jdinput.c
	jpeg-6/jdmainct.c
	jpeg-6/jdmarker.c
	jpeg-6/jdmaster.c
	jpeg-6/jdmerge.c
	jpeg-6/jdphuff.c
	jpeg-6/jdpostct.c
	jpeg-6/jdsample.c
	jpeg-6/jdtrans.c
	jpeg-6/jerror.c
	jpeg-6/jfdctflt.c
	jpeg-6/jfdctfst.c
	jpeg-6/jfdctint.c
	jpeg-6/jidctflt.c
	jpeg-6/jidctfst.c
	jpeg-6/jidctint.c
	jpeg-6/jidctred.c
	#jpeg-6/jload.c
	jpeg-6/jmemmgr.c
	jpeg-6/jmemnobs.c
	#jpeg-6/jpegtran.c
	jpeg-6/jquant1.c
	jpeg-6/jquant2.c
	jpeg-6/jutils.c)

file(GLOB PNG_INCLUDES png/*.h)
file(GLOB PNG_SOURCES png/*.c)

file(GLOB ZLIB_INCLUDES zlib/*.h)
file(GLOB ZLIB_SOURCES zlib/*.c)

set(ETC1_INCLUDES etc1/etc1.h)
set(ETC1_SOURCES etc1/etc1.cpp)
	
set(GLEW_INCLUDES
	glew/include/GL/glew.h)
	
if(WIN32)
	set(GLEW_INCLUDES ${GLEW_INCLUDES} glew/include/GL/wglew.h)
else()
	set(GLEW_INCLUDES ${GLEW_INCLUDES} glew/include/GL/glxew.h)
endif()
	
set(GLEW_SOURCES
	glew/src/glew.c)
	
set(FREETYPE_SOURCES
	freetype/src/autofit/autofit.c
	freetype/src/bdf/bdf.c
	freetype/src/cff/cff.c
	freetype/src/base/ftbase.c
	freetype/src/base/ftbitmap.c
	freetype/src/cache/ftcache.c
	freetype/src/base/ftdebug.c
	freetype/src/base/ftgasp.c
	freetype/src/base/ftglyph.c
	freetype/src/gzip/ftgzip.c
	freetype/src/base/ftinit.c
	freetype/src/lzw/ftlzw.c
	freetype/src/base/ftstroke.c
	freetype/src/base/ftsystem.c
	freetype/src/smooth/smooth.c
	freetype/src/base/ftbbox.c
	freetype/src/base/ftmm.c
	freetype/src/base/ftpfr.c
	freetype/src/base/ftsynth.c
	freetype/src/base/fttype1.c
	freetype/src/base/ftwinfnt.c
	freetype/src/pcf/pcf.c
	freetype/src/pfr/pfr.c
	freetype/src/psaux/psaux.c
	freetype/src/pshinter/pshinter.c
	freetype/src/psnames/psmodule.c
	freetype/src/raster/raster.c
	freetype/src/sfnt/sfnt.c
	freetype/src/truetype/truetype.c
	freetype/src/type1/type1.c
	freetype/src/cid/type1cid.c
	freetype/src/type42/type42.c
	freetype/src/winfonts/winfnt.c)

set(OGGVORBIS_INCLUDES
	oggvorbis/ogg/ogg.h
	oggvorbis/ogg/os_types.h)
	
set(OGGVORBIS_SOURCES
	oggvorbis/oggsrc/bitwise.c
	oggvorbis/oggsrc/framing.c
	oggvorbis/vorbissrc/mdct.c
	oggvorbis/vorbissrc/smallft.c
	oggvorbis/vorbissrc/block.c
	oggvorbis/vorbissrc/envelope.c
	oggvorbis/vorbissrc/windowvb.c
	oggvorbis/vorbissrc/lsp.c
	oggvorbis/vorbissrc/lpc.c
	oggvorbis/vorbissrc/analysis.c
	oggvorbis/vorbissrc/synthesis.c
	oggvorbis/vorbissrc/psy.c
	oggvorbis/vorbissrc/info.c
	oggvorbis/vorbissrc/floor1.c
	oggvorbis/vorbissrc/floor0.c
	oggvorbis/vorbissrc/res0.c
	oggvorbis/vorbissrc/mapping0.c
	oggvorbis/vorbissrc/registry.c
	oggvorbis/vorbissrc/codebook.c
	oggvorbis/vorbissrc/sharedbook.c
	oggvorbis/vorbissrc/lookup.c
	oggvorbis/vorbissrc/bitrate.c
	oggvorbis/vorbissrc/vorbisfile.c)

set(LUA_INCLUDES
	lua/src/lapi.h
	lua/src/lauxlib.h
	lua/src/lcode.h
	lua/src/lctype.h
	lua/src/ldebug.h
	lua/src/ldo.h
	lua/src/lfunc.h
	lua/src/lgc.h
	lua/src/llex.h
	lua/src/llimits.h
	lua/src/lmem.h
	lua/src/lobject.h
	lua/src/lopcodes.h
	lua/src/lparser.h
	lua/src/lstate.h
	lua/src/lstring.h
	lua/src/ltable.h
	lua/src/ltm.h
	lua/src/lua.h
	lua/src/luaconf.h
	lua/src/lualib.h
	lua/src/lundump.h
	lua/src/lvm.h
	lua/src/lzio.h)

set(LUA_SOURCES
	lua/src/lapi.c
	lua/src/lauxlib.c
	lua/src/lbaselib.cpp
	lua/src/lbitlib.c
	lua/src/lcode.c
	lua/src/lcorolib.c
	lua/src/lctype.c
	lua/src/ldblib.c
	lua/src/ldebug.c
	lua/src/ldo.c
	lua/src/ldump.c
	lua/src/lfunc.c
	lua/src/lgc.c
	#linit.c
	lua/src/liolib.c
	lua/src/llex.c
	lua/src/lmathlib.c
	lua/src/lmem.c
	lua/src/loadlib.c
	lua/src/lobject.c
	lua/src/lopcodes.c
	lua/src/loslib.c
	lua/src/lparser.c
	lua/src/lstate.c
	lua/src/lstring.c
	lua/src/lstrlib.c
	lua/src/ltable.c
	lua/src/ltablib.c
	lua/src/ltm.c
	#lua/src/lua.c
	lua/src/lundump.c
	lua/src/lvm.c
	lua/src/lzio.c)

set(LUAWRAPPER_INCLUDES
	luawrapper/luawrapper.hpp
	luawrapper/luawrapperutil.hpp)
	
file(GLOB LUASOCKET_INCLUDES luasocket/src/*.h)

set(LUASOCKET_SOURCES
	luasocket/src/luasocket.c
	luasocket/src/timeout.c
	luasocket/src/buffer.c
	luasocket/src/io.c
	luasocket/src/auxiliar.c
	luasocket/src/options.c
	luasocket/src/inet.c
	luasocket/src/tcp.c
	luasocket/src/udp.c
	luasocket/src/except.c
	luasocket/src/select.c
	# Modules belonging mime-core
	#$(COMPAT)/compat-5.1.o \
	luasocket/src/mime.c
	luasocket/src/buffer.c
	luasocket/src/auxiliar.c
	luasocket/src/options.c
	luasocket/src/timeout.c
	luasocket/src/io.c)
	
if(WIN32)
	list(APPEND LUASOCKET_SOURCES luasocket/src/wsocket.c)
else()
	list(APPEND LUASOCKET_SOURCES luasocket/src/unix.c)
	list(APPEND LUASOCKET_SOURCES luasocket/src/usocket.c)
endif()

set(EXTERNAL_INCLUDES_ALL
	${IRRXML_INCLUDES}
	${JPEG_INCLUDES}
	${PNG_INCLUDES}
	${ZLIB_INCLUDES}
	${ETC1_INCLUDES}
	${GLEW_INCLUDES}
	${FREETYPE_INCLUDES}
	${OGGVORBIS_INCLUDES}
	${LUA_INCLUDES}
	${LUAWRAPPER_INCLUDES}
	${LUASOCKET_INCLUDES})
			
set(EXTERNAL_SOURCES_ALL
	${IRRXML_SOURCES}
	${JPEG_SOURCES}
	${PNG_SOURCES}
	${ZLIB_SOURCES}
	${ETC1_SOURCES}
	${GLEW_SOURCES}
	${FREETYPE_SOURCES}
	${OGGVORBIS_SOURCES}
	${LUA_SOURCES}
	${LUASOCKET_SOURCES})

if(ANGLE)
	add_subdirectory(angle)
endif()

source_group("Irrxml" FILES ${IRRXML_INCLUDES})
source_group("Irrxml" FILES ${IRRXML_SOURCES})
source_group("Jpeg-6" FILES ${JPEG_INCLUDES})
source_group("Jpeg-6" FILES ${JPEG_SOURCES})
source_group("LibPNG" FILES ${PNG_INCLUDES})
source_group("LibPNG" FILES ${PNG_SOURCES})
source_group("Etc1" FILES ${ETC1_INCLUDES})
source_group("Etc1" FILES ${ETC1_SOURCES})
source_group("Zlib" FILES ${ZLIB_INCLUDES})
source_group("Zlib" FILES ${ZLIB_SOURCES})
source_group("Glew" FILES ${GLEW_INCLUDES})
source_group("Glew" FILES ${GLEW_SOURCES})
source_group("FreeType" FILES ${FREETYPE_SOURCES})
source_group("OggVorbis" FILES ${OGGVORBIS_INCLUDES})
source_group("OggVorbis" FILES ${OGGVORBIS_SOURCES})
source_group("Lua" FILES ${LUA_INCLUDES})
source_group("Lua" FILES ${LUA_SOURCES})
source_group("LuaWrapper" FILES ${LUAWRAPPER_INCLUDES})
source_group("LuaSocket" FILES ${LUASOCKET_INCLUDES})
source_group("LuaSocket" FILES ${LUASOCKET_SOURCES})


add_library(${PROJECT_NAME} ${EXTERNAL_INCLUDES_ALL} ${EXTERNAL_SOURCES_ALL})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/idlib ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/lua/src ${CMAKE_CURRENT_SOURCE_DIR}/luawrapper ${CMAKE_CURRENT_SOURCE_DIR}/curl/include ${CMAKE_CURRENT_SOURCE_DIR}/glew/include ${CMAKE_CURRENT_SOURCE_DIR}/freetype/include ${CMAKE_CURRENT_SOURCE_DIR}/zlib)
target_compile_definitions(${PROJECT_NAME} PRIVATE GLEW_STATIC FT2_BUILD_LIBRARY)