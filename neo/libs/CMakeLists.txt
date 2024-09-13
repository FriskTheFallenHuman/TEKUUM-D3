project(external C CXX)

file(GLOB IRRXML_INCLUDES irrxml/src/*.h)
file(GLOB IRRXML_SOURCES irrxml/src/*.cpp)

file(GLOB MINIZ_INCLUDES miniz/*.h)
file(GLOB MINIZ_SOURCES miniz/*.cpp miniz/*.c)

file(GLOB MINIZIP_INCLUDES minizip/*.h)
file(GLOB MINIZIP_SOURCES minizip/*.cpp minizip/*.c)

file(GLOB MIKKTSPACE_INCLUDES mikktspace/*.h)
file(GLOB MIKKTSPACE_SOURCES mikktspace/*.c)

set(EXTERNAL_INCLUDES_ALL
	${IRRXML_INCLUDES}
	${MIKKTSPACE_INCLUDES}
	${MINIZ_INCLUDES}
	${MINIZIP_INCLUDES})

set(EXTERNAL_SOURCES_ALL
	${IRRXML_SOURCES}
	${MIKKTSPACE_SOURCES}
	${MINIZ_SOURCES}
	${MINIZIP_SOURCES})

source_group("Irrxml" FILES ${IRRXML_INCLUDES})
source_group("Irrxml" FILES ${IRRXML_SOURCES})
source_group("Mikktspace" FILES ${MIKKTSPACE_SOURCES})
source_group("Mikktspace" FILES ${MIKKTSPACE_SOURCES})
source_group("Miniz" FILES ${MINIZ_INCLUDES})
source_group("Miniz" FILES ${MINIZ_SOURCES})
source_group("MiniZip" FILES ${MINIZIP_INCLUDES})
source_group("MiniZip" FILES ${MINIZIP_SOURCES})

add_library(${PROJECT_NAME} ${EXTERNAL_INCLUDES_ALL} ${EXTERNAL_SOURCES_ALL})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/idlib ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/glew/include ${CMAKE_CURRENT_SOURCE_DIR}/miniz ${CMAKE_CURRENT_SOURCE_DIR}/minizip )