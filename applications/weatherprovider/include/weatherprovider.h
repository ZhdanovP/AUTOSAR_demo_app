#ifndef WEATHER_PROVIDER_H_
#define WEATHER_PROVIDER_H_

#include "ara/demo/weatherservice_skeleton.h"
#include "ara/demo/weatherservice_common.h"

#include "ara/demo/temperatureservice_proxy.h"
#include "ara/demo/temperatureservice_common.h"

#include "ara/core/string.h"
#include "ara/log/logging.h"
#include "ara/core/instance_specifier.h"

#include <memory>

namespace ara
{
namespace demo
{
class WeatherProvider : public ara::demo::skeleton::WeatherServiceSkeleton {
public:
    using Skeleton = ara::demo::skeleton::WeatherServiceSkeleton;
    using ara::demo::WeatherService::GetWeatherForecastOutput;
    using Proxy = proxy::TemperatureServiceProxy;
    
    WeatherProvider();

    void Init();


    ara::core::Future<GetWeatherForecastOutput> GetWeatherForecast(
        const ::ara::demo::DateType& Date) override;

    ara::core::Future<ara::demo::skeleton::fields::CurrentTemperature::value_type> getCurrentTemperature();

    void monitorTemperature();

private:

    void serviceAvailabilityCallback(ara::com::ServiceHandleContainer<Proxy::HandleType> handles,
            ara::com::FindServiceHandle handler);
            
    std::shared_ptr<Proxy> proxy_;
    std::mutex proxy_mutex_;

    uint8_t currentTemperature_;
    ara::log::Logger& logger_{ara::log::CreateLogger("WRPR", "WeatherProvider", ara::log::LogLevel::kDebug)};
};

}
}
#endif // RADIO_CONTROLLER_H_
