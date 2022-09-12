#include "version.h"
#include "weatherprovider.h"

#include "ara/exec/execution_client.h"
#include "ara/log/logging.h"
#include "ara/core/initialization.h"

#include <cstdlib>
#include <csignal>
#include <chrono>
#include <thread>

namespace
{

// Atomic flag for exit after SIGTERM caught
std::atomic_bool continueExecution{true};

void SigTermHandler(int signal)
{
    if (signal == SIGTERM) {
        // set atomic exit flag
        continueExecution = false;
    }
}

bool RegisterSigTermHandler()
{
    struct sigaction sa;
    sa.sa_handler = SigTermHandler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    // register signal handler
    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        // Could not register a SIGTERM signal handler
        return false;
    }
    return true;
}

}   // namespace

int main(void) {
    std::cout << "WeatherProvider main\n";
    using namespace ara::log;

    if (!ara::core::Initialize()) {
        // No interaction with ARA is possible here since initialization failed
        std::cerr << "ara runtime initialization failed";
        return EXIT_FAILURE;
    }

    Logger& logger = CreateLogger("MAIN", "WeatherProvider main", LogLevel::kVerbose);

    ara::exec::ExecutionClient exec_client;
    logger.LogInfo() << "Execution CLient created";

    if (!exec_client.ReportExecutionState(ara::exec::ExecutionState::kRunning)) {
        logger.LogError() << "reporting execution state \"kRunning\" to the EM has failed.";
    }

    if (!RegisterSigTermHandler()) {
        logger.LogError() << "Unable to register signal handler";
    }

    logger.LogInfo() << "WeatherProvider version" << g_appVersion;

    ara::demo::WeatherProvider weatherProvider;
    weatherProvider.Init();
    std::thread temperatureSensor(&ara::demo::WeatherProvider::monitorTemperature, &weatherProvider);

    while (continueExecution) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    if (!exec_client.ReportExecutionState(ara::exec::ExecutionState::kTerminating)) {
        logger.LogError() << "reporting execution state \"kTerminating\" to the EM has failed.";
    }

    if (!ara::core::Deinitialize()) {
        // No interaction with ARA is possible here since some ARA resources can be destroyed already
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
