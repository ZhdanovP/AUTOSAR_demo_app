# This command adds all the source files where the functionality of application
# has been implemented

set(APPLICATION_SOURCES
    src/main.cpp
    src/execution_client_wrapper.cpp
    src/state_client_wrapper.cpp
)

# This command adds the directories that contain the required header files
set(APPLICATION_INCLUDE_DIRS
    include/
)

# Normally you should not edit the commands below

get_filename_component(APPLICATION_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

set(SOURCES  ${APPLICATION_SOURCES})

add_library(${APPLICATION_NAME}.software_component OBJECT EXCLUDE_FROM_ALL ${SOURCES})
target_include_directories(
    ${APPLICATION_NAME}.software_component
    PUBLIC
        ${APPLICATION_INCLUDE_DIRS}
)

target_link_libraries(
    ${APPLICATION_NAME}.software_component
    INTERFACE
    ara::core
    ara::exec_execution_client
    ara::exec_state_client
    ara::log
)
