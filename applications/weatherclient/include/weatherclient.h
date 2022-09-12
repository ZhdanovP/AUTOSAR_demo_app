#ifndef WEATHER_CLIENT_H_
#define WEATHER_CLIENT_H_

#include "ara/demo/weatherservice_proxy.h"
#include "ara/demo/weatherservice_common.h"
#include "ara/log/logging.h"
#include "ara/per/key_value_storage.h"
#include "ara/per/kvstype/kvs_type.h"

#include <mutex>
namespace ara
{
namespace demo
{
class WeatherClient {
public:
    using Proxy = proxy::WeatherServiceProxy;

    WeatherClient();
    ~WeatherClient();

    void Init();
    void getPeriodicWeatherForcast();

private:
    ara::core::InstanceSpecifier kvsInstanceSpecifier_{"weatherclient_exe/root_swc/WeatherClientKvsPRPort"};
    ara::per::SharedHandle<ara::per::KeyValueStorage> storage_;
    std::shared_ptr<Proxy> proxy_;
    std::mutex proxy_mutex_;
    ara::log::Logger& logger_{ara::log::CreateLogger("WRCL", "WeatherClient", ara::log::LogLevel::kDebug)};

    void serviceAvailabilityCallback(ara::com::ServiceHandleContainer<Proxy::HandleType> handles,
            ara::com::FindServiceHandle handler);

    void SubscribeData();

    void OnCurrentTemperatureChange();

    bool persistTemperature(uint8_t temperature);
    std::pair<bool, uint8_t> getPersistedTemperature();
    uint8_t currentTemperature_;

};
}//namspace demo end
}//namspace ara end

#endif // DASHBOARD_CLIENT_H_
