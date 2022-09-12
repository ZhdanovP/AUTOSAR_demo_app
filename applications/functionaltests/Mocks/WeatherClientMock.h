#ifndef WEATHER_CLIENT_MOCK_H_
#define WEATHER_CLIENT_MOCK_H_

#include "ara/demo/weatherservice_proxy.h"
#include "ara/demo/weatherservice_common.h"
#include "ara/log/logging.h"

#include <mutex>
namespace ara
{
namespace demo
{
class WeatherClientMock {
public:
    using Proxy = proxy::WeatherServiceProxy;

    WeatherClientMock();
    ~WeatherClientMock() = default;

    std::string getWeatherForcast(std::string date);
    uint8_t getCurrentTemperature();

private:
    std::shared_ptr<Proxy> proxy_;
    std::mutex proxy_mutex_;
    ara::log::Logger& logger_{ara::log::CreateLogger("WCLM", "WeatherClientMock mock", ara::log::LogLevel::kDebug)};

    void serviceAvailabilityCallback(ara::com::ServiceHandleContainer<Proxy::HandleType> handles,
            ara::com::FindServiceHandle handler);

    void SubscribeData();

    void OnCurrentTemperatureChange();

    uint8_t currentTemperature_;

};
}//namspace demo end
}//namspace ara end

#endif // WEATHER_CLIENT_MOCK_H_
