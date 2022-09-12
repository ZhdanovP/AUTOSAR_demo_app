#ifndef TEMPERATURE_PROVIDER_MOCK_H_
#define TEMPERATURE_PROVIDER_MOCK_H_

#include "ara/demo/temperatureservice_skeleton.h"
#include "ara/demo/temperatureservice_common.h"

#include "ara/core/string.h"
#include "ara/log/logging.h"
#include "ara/core/instance_specifier.h"
#include "ITemperatureService.h"

#include <memory>

namespace ara
{
namespace demo
{
class TemperatureProviderMock : public ara::demo::skeleton::TemperatureServiceSkeleton {
public:
    using Skeleton = ara::demo::skeleton::TemperatureServiceSkeleton;
    using ara::demo::TemperatureService::GetCurrentTemperatureOutput;
    using Temperature = ara::demo::TemperatureType;
    TemperatureProviderMock(ara::demo::ITemperatureService* temperatureService);

    ara::core::Future<GetCurrentTemperatureOutput> GetCurrentTemperature() override;


private:

    Temperature currentTemperature_;
    ara::demo::ITemperatureService* temperatureService_;
    ara::log::Logger& logger_{ara::log::CreateLogger("TPRM", "TemperatureProviderMock", ara::log::LogLevel::kDebug)};
};

}
}
#endif // TEMPERATURE_PROVIDER_MOCK_H_
