project(external C CXX)

add_subdirectory( moc )

file(GLOB BINKDEC_INCLUDES libbinkdec/includes/*.h)
file(GLOB BINKDEC_SOURCES libbinkdec/src/*.cpp libbinkdec/src/*.c)

file(GLOB MINIZ_INCLUDES miniz/*.h)
file(GLOB MINIZ_SOURCES miniz/*.cpp miniz/*.c)

file(GLOB MINIZIP_INCLUDES minizip/*.h)
file(GLOB MINIZIP_SOURCES minizip/*.cpp minizip/*.c)

file(GLOB MIKKTSPACE_INCLUDES mikktspace/*.h)
file(GLOB MIKKTSPACE_SOURCES mikktspace/*.c)

set(EXTERNAL_INCLUDES_ALL
	${MIKKTSPACE_INCLUDES}
	${MINIZ_INCLUDES}
	${MINIZIP_INCLUDES})

set(EXTERNAL_SOURCES_ALL
	${MIKKTSPACE_SOURCES}
	${MINIZ_SOURCES}
	${MINIZIP_SOURCES})
	
if( BINKDEC )
	list( APPEND EXTERNAL_INCLUDES_ALL
		${BINKDEC_INCLUDES})

	list( APPEND EXTERNAL_SOURCES_ALL
		${BINKDEC_SOURCES})
endif()

source_group("Binkdec" FILES ${BINKDEC_INCLUDES})
source_group("Binkdec" FILES ${BINKDEC_SOURCES})
source_group("Mikktspace" FILES ${MIKKTSPACE_SOURCES})
source_group("Mikktspace" FILES ${MIKKTSPACE_SOURCES})
source_group("Miniz" FILES ${MINIZ_INCLUDES})
source_group("Miniz" FILES ${MINIZ_SOURCES})
source_group("MiniZip" FILES ${MINIZIP_INCLUDES})
source_group("MiniZip" FILES ${MINIZIP_SOURCES})

add_library(${PROJECT_NAME} ${EXTERNAL_INCLUDES_ALL} ${EXTERNAL_SOURCES_ALL})
set_target_properties( ${PROJECT_NAME} PROPERTIES FOLDER "libs" )
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/idlib ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/miniz ${CMAKE_CURRENT_SOURCE_DIR}/minizip ${CMAKE_CURRENT_SOURCE_DIR}/libbinkdec/include )