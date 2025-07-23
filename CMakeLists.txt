cmake_minimum_required(VERSION 3.10)

PROJECT(ct C)

find_package(Threads REQUIRED)

ADD_DEFINITIONS(-Wall -Werror)
ADD_DEFINITIONS(-Os -Wno-unused-parameter)

# OPTION(BUILD_LUA "build Lua plugin" ON)

SET(SOURCES 
    src/osi.c
    src/compat.c
    src/list.c
    src/array.c
    src/buffer.c
    src/allocator.c
    src/allocation_tracker.c
    src/hash_map.c
    src/hash_map_utils.c
    src/hash_functions.c
    src/semaphore.c
    src/mutex.c
    src/future.c
    src/thread.c
    src/ringbuffer.c
    src/socket.c
    src/socket_utils/socket_local_client.c
    src/socket_utils/socket_local_server.c
    src/time.c
    src/reactor.c
    src/fixed_queue.c
    src/data_dispatcher.c
    src/eager_reader.c
    src/config.c
)

include_directories(include)

ADD_LIBRARY(ct SHARED ${SOURCES})
ADD_LIBRARY(ct-static STATIC ${SOURCES})
SET_TARGET_PROPERTIES(ct-static PROPERTIES OUTPUT_NAME ct)
target_link_libraries(ct PRIVATE Threads::Threads rt)

FILE(GLOB headers include/*.h)
LIST(FILTER headers EXCLUDE REGEX "-priv.h$" )
INSTALL(FILES ${headers}
	DESTINATION include/ct
)

INSTALL(TARGETS ct
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
)

IF(UNIT_TESTING)
  ENABLE_TESTING()
  ADD_SUBDIRECTORY(test)
ENDIF()
