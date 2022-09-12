#include "execution_client_wrapper.h"

ExecutionClientWrapper::ExecutionClientWrapper(){
    if (!exec_client_.ReportExecutionState(ara::exec::ExecutionState::kRunning)) {
        logger_.LogError() << "Failed to report ExecutionState::kRunning";
    } else {
        logger_.LogInfo() << "Successfully reported ExecutionState::kRunning";
    }
}

ExecutionClientWrapper::~ExecutionClientWrapper(){
    if (!exec_client_.ReportExecutionState(ara::exec::ExecutionState::kTerminating)) {
        logger_.LogError() << "Failed to report ExecutionState::kTerminating";
    } else {
        logger_.LogInfo() << "Successfully reported ExecutionState::kTerminating";
    }
}
