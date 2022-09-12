# This command sets the name of the executable that is executed in this process
# This name shall match the directory name for corresponding executable
set(APPLICATION_NAME statemanager)
set(PROCESS_FQN /ara/processes/statemanager_instance)

# This command sets the filenames of AUTOSAR xml files (manifests)
# where the Process is defined
file(GLOB PROCESS_SPECIFIC_MANIFESTS CONFIGURE_DEPENDS "./*.arxml")

# Normally you should not edit the commands below

get_filename_component(PROCESS_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(PROCESS_TARGET_NAME ${MACHINE_NAME}.${PROCESS_NAME})

set(FULL_SET_OF_ARXML
    ${MACHINE_MANIFEST}
    ${COMMON_DESIGN_ARXMLS}
    ${PROCESS_SPECIFIC_MANIFESTS}
    ${PROJECT_SOURCE_DIR}/applications/${APPLICATION_NAME}/${APPLICATION_NAME}.arxml
)

set(GEN_OUTPUT_DIR
    ${CMAKE_CURRENT_BINARY_DIR}/gen
)

add_process_artifacts(
    DESTINATION ${GEN_OUTPUT_DIR}
    TARGET ${PROCESS_TARGET_NAME}
    PROCESS_FQN ${PROCESS_FQN}
    ARXMLS ${FULL_SET_OF_ARXML}
)

set(EXECUTABLE ${PROCESS_TARGET_NAME}.executable)
add_executable(${EXECUTABLE} EXCLUDE_FROM_ALL)

target_link_libraries(${EXECUTABLE} ${APPLICATION_NAME}.software_component)

add_dependencies(${EXECUTABLE} ${PROCESS_TARGET_NAME})

add_custom_target(${PROCESS_TARGET_NAME}.process DEPENDS ${PROCESS_TARGET_NAME} ${APPLICATION_NAME}.software_component ${EXECUTABLE})

#install(TARGETS ${APPLICATION_NAME}_executable bin/)

install(FILES $<TARGET_FILE:${EXECUTABLE}>
        PERMISSIONS OWNER_EXECUTE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ
        DESTINATION ${PROCESS_NAME}/bin/ COMPONENT ${MACHINE_NAME} RENAME ${PROCESS_NAME} EXCLUDE_FROM_ALL)

install(FILES ${GEN_OUTPUT_DIR}/processes/${PROCESS_NAME}_MANIFEST.json
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ DESTINATION ${PROCESS_NAME}/etc COMPONENT ${MACHINE_NAME} RENAME MANIFEST.json EXCLUDE_FROM_ALL)

install(
        FILES ${GEN_OUTPUT_DIR}/processes/${PROCESS_NAME}_SI_MANIFEST.json
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
        DESTINATION ${PROCESS_NAME}/etc
        COMPONENT ${MACHINE_NAME}
        RENAME service_instance_manifest.json
        EXCLUDE_FROM_ALL
)
