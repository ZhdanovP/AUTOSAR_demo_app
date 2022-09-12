#include "state_client_wrapper.h"
#include <sys/types.h>
#include <sys/stat.h>

using ara::exec::FunctionGroup;
using ara::exec::FunctionGroupState;

StateClientWrapper::StateClientWrapper()
{

    mkdir("./run", S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IWOTH | S_IROTH);
    mkfifo("./run/sysstate-client-fifo", S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IWOTH | S_IROTH);
    state_client_ = std::make_unique<ara::exec::StateClient>([this](auto&){
        logger_.LogInfo() << "FunctionGroup changes its state unexpectedly to an Undefined Function Group State";
    });

    state_client_->GetInitialMachineStateTransitionResult().GetResult().ValueOrThrow();
    logger_.LogInfo() << "Initial transition for MachineState finished successfully";
}

bool StateClientWrapper::ChangeState(const ara::core::String& fg, const ara::core::String& state)
{
    logger_.LogInfo() << "Attempting to change the current state of  Function Group " << fg << " to " << state;

    auto groupTokenResult = FunctionGroup::Preconstruct(fg);
    FunctionGroup::CtorToken groupToken(groupTokenResult.ValueOrThrow());
    FunctionGroup group(std::move(groupToken));

    auto stateTokenResult = FunctionGroupState::Preconstruct(group, state);
    FunctionGroupState::CtorToken stateToken(stateTokenResult.ValueOrThrow());
    FunctionGroupState groupState(std::move(stateToken));

    auto setStateFuture = state_client_->SetState(groupState);
    auto setStateResult = setStateFuture.GetResult();
    if (setStateResult) {
        logger_.LogInfo() << "Transition" << fg << "to state" << state << " finished successfully";
        return true;
    }
    logger_.LogError() << "Transition" << fg << "to state" << state << "failed" << setStateResult.Error().Message();
    return false;
}
