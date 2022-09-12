#include "temperatureprovider.h"

#include "ara/core/instance_specifier.h"
#include "ara/com/com_error_domain.h"


#include <cstdint>
#include<thread>
#include<chrono>

namespace ara
{
namespace demo
{
TemperatureProvider::TemperatureProvider() :
    Skeleton(ara::core::InstanceSpecifier("temperatureprovider_exe/root_swc/TemperatureServicePPort"), ara::com::MethodCallProcessingMode::kEventSingleThread)
{
}

void TemperatureProvider::Init() {

    // Register Field Getters.
    logger_.LogInfo() << "TemperatureProvider::init ++";
    
    OfferService();

    logger_.LogInfo() << "TemperatureProvider::init --";
}

ara::core::Future<TemperatureProvider::GetCurrentTemperatureOutput> TemperatureProvider::GetCurrentTemperature() {
    logger_.LogInfo() << "TemperatureProvider::getCurrentTemperature ++";
    ara::core::Promise<ara::demo::TemperatureService::GetCurrentTemperatureOutput> promise;
    ara::demo::TemperatureService::GetCurrentTemperatureOutput output;
    output.Temperature = currentTemperature_;
    promise.set_value(output);
    logger_.LogInfo() << "TemperatureProvider::getCurrentTemperature --";
    return promise.get_future();
}

void TemperatureProvider::monitorTemperature() {
    logger_.LogInfo() << "TemperatureProvider::monitorTemperature ++";
    uint8_t temp{25};
    constexpr uint8_t maxTemp{35};
    while(true)
    {
        currentTemperature_ = temp;
        temp ++;
        if(maxTemp == temp)
        {
            temp = 25;
        }

         std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    logger_.LogInfo() << "TemperatureProvider::monitorTemperature --";
}

}
}