#include "version.h"
#include "weatherclient.h"

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
    using namespace ara::log;

    if (!ara::core::Initialize()) {
        // No interaction with ARA is possible here since initialization failed
        std::cerr << "ara runtime initialization failed";
        return EXIT_FAILURE;
    }

    Logger& logger = CreateLogger("MAIN", "WeatherClient main", LogLevel::kVerbose);

    ara::exec::ExecutionClient exec_client;

    if (!exec_client.ReportExecutionState(ara::exec::ExecutionState::kRunning)) {
        logger.LogError() << "reporting execution state \"kRunning\" to the EM has failed.";
    }

    if (!RegisterSigTermHandler()) {
        logger.LogError() << "Unable to register signal handler";
    }

    logger.LogInfo() << "WeatherClient version" << g_appVersion;

    ara::demo::WeatherClient weatherClient;

    try
    {
        weatherClient.Init();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
         if (!exec_client.ReportExecutionState(ara::exec::ExecutionState::kTerminating)) {
            logger.LogError() << "reporting execution state \"kTerminating\" to the EM has failed.";
        }
        return EXIT_FAILURE;
    }
    
    std::thread forecastMonitor(&ara::demo::WeatherClient::getPeriodicWeatherForcast, &weatherClient);
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
