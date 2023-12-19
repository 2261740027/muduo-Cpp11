project(eventLoopLib)

aux_source_directory(. EVENTLOOP_LIB)

message(${EVENTLOOP_LIB})
include_directories(../Inc)

add_library(${PROJECT_NAME} STATIC ${EVENTLOOP_LIB})
