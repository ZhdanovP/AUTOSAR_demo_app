# This command sets the AUTOSAR xml file name which contains the description
# of Software Component and Executable. Please note that filename shall be equal
# to the directory name
set(SWC_ARXML functionaltests.arxml)

# This command sets the fully qualified name of Software Component which acts as
# Root Software Component of the Executable.
# It shall be defined in the AUOTSAR xml file mentioned above
set(SOFTWARE_COMPONENT /ara/demo/applications/functionaltests)

# This command adds all the source files where the functionality of application
# has been implemented

set(APPLICATION_SOURCES
    src/main.cpp
    src/FunctionalTestEnvironment.cpp
    src/WeatherProviderFunctionalTests.cpp
    Mocks/WeatherClientMock.cpp
    Mocks/TemperatureProviderMock.cpp
)

set(APPLICATION_INCLUDE_DIRS
    src/
    Mocks/
)

# Normally you should not edit the commands below

get_filename_component(APPLICATION_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

add_middleware_interfaces(
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/gen
    TARGET ${APPLICATION_NAME}_MiddlewareInterfaces
    SWC_FQN ${SOFTWARE_COMPONENT}
    ARXMLS
        ${INTERFACES_ARXMLS}
        ${DEPLOYMENT_MANIFESTS}
        ${SWC_ARXML}
)
set(SOURCES  ${APPLICATION_SOURCES})

add_library(${APPLICATION_NAME}.software_component OBJECT EXCLUDE_FROM_ALL ${SOURCES})
add_dependencies(${APPLICATION_NAME}.software_component ${APPLICATION_NAME}_MiddlewareInterfaces)

target_include_directories(
    ${APPLICATION_NAME}.software_component
    PUBLIC
        ${APPLICATION_INCLUDE_DIRS}
        ${${APPLICATION_NAME}_MiddlewareInterfaces_INCLUDE_DIRS}
)

target_link_libraries(
    ${APPLICATION_NAME}.software_component
    INTERFACE
    ara::core
    ara::log
    ara::exec_execution_client
    gtest
    gmock
    )
