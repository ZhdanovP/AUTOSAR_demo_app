# This command sets the fully qualified name of the Machine (Adaptive ECU)
# It shall be defined in the AUOTSAR xml file mentioned below
set(MACHINE /ara/demo/machines/MainEcu)

# This command extracts short name from fully qualified name
string(REGEX MATCH "[^/]*$" MACHINE_NAME ${MACHINE})

# This command sets the AUTOSAR xml file name which contains the description
# of Machine and corresponding MachineDesign
set(MACHINE_MANIFEST ${CMAKE_CURRENT_LIST_DIR}/machine_manifest.arxml)

# Do not edit this command unless you want to change the subfolder which
# contains the processes mapped to the machine
add_subdirectory(processes)

# Add the target name for the process to the list of dependencies
# in the following form: ${MACHINE_NAME}.<process.ShortName>_process
set(PROCESS_TARGETS
    temperatureprovider_instance.process
    weatherclient_instance.process
    weatherprovider_instance.process
    statemanager_instance.process
    functionaltests_instance.process
    # <process.ShortName>.process
)

# Normally you should not edit the commands below
message(STATUS "Processing Machine: ${MACHINE_NAME}")

list(TRANSFORM PROCESS_TARGETS PREPEND ${MACHINE_NAME}.)

set(MACHINE_MANIFEST_MAKE_TARGET ${MACHINE_NAME}.machine_artifacts)
set(GEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen)


file(GLOB_RECURSE ALL_PROCESSES_ARXMLS CONFIGURE_DEPENDS "./processes/*.arxml")
file(GLOB_RECURSE ALL_EXECUTABLES_ARXMLS CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/applications/*.arxml)

set(ALL_MACHINE_ARXMLS
    ${MACHINE_MANIFEST}
    ${DEPLOYMENT_MANIFESTS}
    ${COMMON_DESIGN_ARXMLS}
    ${INTERFACES_ARXMLS}
    ${ALL_EXECUTABLES_ARXMLS}
    ${ALL_PROCESSES_ARXMLS}
)

add_machine_artifacts(
    DESTINATION ${GEN_OUTPUT_DIR}
    TARGET ${MACHINE_MANIFEST_MAKE_TARGET}
    MACHINE_FQN ${MACHINE}
    ARXMLS
        ${ALL_MACHINE_ARXMLS}
)

add_custom_target(${MACHINE_NAME} DEPENDS ${MACHINE_MANIFEST_MAKE_TARGET} ${PROCESS_TARGETS})

install(
    FILES ${${MACHINE_MANIFEST_MAKE_TARGET}_MACHINE_MANIFEST}
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
    DESTINATION etc/system/
    COMPONENT ${MACHINE_NAME}.machine_manifest
    RENAME machine_manifest.json
    EXCLUDE_FROM_ALL
)
install(
    FILES ${GEN_OUTPUT_DIR}/machines/vsomeip/${MACHINE_NAME}_vsomeip.json
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
    DESTINATION etc/
    COMPONENT ${MACHINE_NAME}.vsomeipjson
    RENAME vsomeip.json
    EXCLUDE_FROM_ALL
)
