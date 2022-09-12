#ifndef WEATHER_PROVIDER_H_
#define WEATHER_PROVIDER_H_

#include "ara/demo/temperatureservice_skeleton.h"
#include "ara/demo/temperatureservice_common.h"

#include "ara/core/string.h"
#include "ara/log/logging.h"
#include "ara/core/instance_specifier.h"

#include <memory>

namespace ara
{
namespace demo
{
class TemperatureProvider : public ara::demo::skeleton::TemperatureServiceSkeleton {
public:
    using Skeleton = ara::demo::skeleton::TemperatureServiceSkeleton;
    using ara::demo::TemperatureService::GetCurrentTemperatureOutput;
    using Temperature = ara::demo::TemperatureType;
    TemperatureProvider();

    void Init();


    ara::core::Future<GetCurrentTemperatureOutput> GetCurrentTemperature() override;

    void monitorTemperature();

private:

    Temperature currentTemperature_;
    ara::log::Logger& logger_{ara::log::CreateLogger("TRPR", "TemperatureProvider", ara::log::LogLevel::kDebug)};
};

}
}
#endif // RADIO_CONTROLLER_H_
