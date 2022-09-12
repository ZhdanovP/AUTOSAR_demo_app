#include "execution_client_wrapper.h"
#include "state_client_wrapper.h"

#include "ara/exec/execution_client.h"
#include "ara/log/logging.h"
#include "ara/core/initialization.h"
#include "ara/core/string.h"

#include <cstdlib>

namespace {

const std::string machineArg{"--machine="};

void GetMachineAddrFromCommandLine(int argc, char const* argv[], ara::core::String& str)
{
    for (int i = 0; i < argc; ++i) {
        std::string arg{argv[i]};
        if (arg.substr(0, machineArg.size()) == machineArg) {
            str = arg.substr(machineArg.size()).c_str();
        }
    }
}

}

int main(int argc, char const* argv[]) {
    using namespace ara::log;

    if (!ara::core::Initialize()) {
        // No interaction with ARA is possible here since initialization failed
        std::cerr << "ara runtime initialization failed";
        return EXIT_FAILURE;
    }

    auto& logger{CreateLogger("MAIN", "Demo state manager main context", ara::log::LogLevel::kVerbose)};

    try{
        ara::core::String machineName{"/ara/demo/machines/MainEcu"};
        GetMachineAddrFromCommandLine(argc, argv, machineName);

        ExecutionClientWrapper exec_client_wrapper;
        StateClientWrapper state_client_wrapper;

        if ( !state_client_wrapper.ChangeState("MachineState", "Driving") ){
            logger.LogError() << "Application failed to change MachineState state to Driving";
        }
        if ( !state_client_wrapper.ChangeState(machineName + "/AdaptiveAppGroup", "On") ){
            logger.LogError() << "Application failed to change AdaptiveAppGroup state to On";
        }
    }catch(const std::exception& e){
        logger.LogFatal() << "Application failed: " << e.what();
    }
    logger.LogInfo() << "Application exits";

    if (!ara::core::Deinitialize()) {
        // No interaction with ARA is possible here since some ARA resources can be destroyed already
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
