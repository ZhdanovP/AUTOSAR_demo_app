#include "TemperatureProviderMock.h"

#include "ara/core/instance_specifier.h"
#include "ara/com/com_error_domain.h"


#include <cstdint>
#include<thread>
#include<chrono>

namespace ara
{
namespace demo
{
TemperatureProviderMock::TemperatureProviderMock(ara::demo::ITemperatureService* temperatureService) :
    Skeleton(ara::core::InstanceSpecifier("functionaltests_exe/root_swc/TemperatureServicePPort"), ara::com::MethodCallProcessingMode::kEventSingleThread),
    temperatureService_{temperatureService}

{
    OfferService();
}

ara::core::Future<TemperatureProviderMock::GetCurrentTemperatureOutput> TemperatureProviderMock::GetCurrentTemperature() {
    logger_.LogInfo() << "TemperatureProviderMock::getCurrentTemperature ++";
    ara::core::Promise<ara::demo::TemperatureService::GetCurrentTemperatureOutput> promise;
    ara::demo::TemperatureService::GetCurrentTemperatureOutput output;
    output.Temperature = temperatureService_->getCurrentTemperature();
    promise.set_value(output);
    logger_.LogInfo() << "TemperatureProviderMock::getCurrentTemperature --";
    return promise.get_future();
}

}
}