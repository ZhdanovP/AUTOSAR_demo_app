#include "WeatherClientMock.h"
#include "ara/core/instance_specifier.h"
#include <threads.h>
#include<string>
#include<chrono>

namespace ara
{
namespace demo
{
WeatherClientMock::WeatherClientMock():
                currentTemperature_{0} 
{
    Proxy::StartFindService(
        [this](ara::com::ServiceHandleContainer<Proxy::HandleType> handles, ara::com::FindServiceHandle handler) {
            serviceAvailabilityCallback(std::move(handles), handler);
        },
        ara::core::InstanceSpecifier("functionaltests_exe/root_swc/WeatherServiceRPort")
    );
}

void WeatherClientMock::serviceAvailabilityCallback(ara::com::ServiceHandleContainer<Proxy::HandleType> handles,
        ara::com::FindServiceHandle handler)
{
    logger_.LogInfo() << " WeatherClientMock::serviceAvailabilityCallback() ++";
    (void)handler;
    if (handles.size() <= 0) {
        return;
    }

    std::lock_guard<std::mutex> lock(proxy_mutex_);
    if (nullptr == proxy_) {
        proxy_ = std::make_shared<Proxy>(handles[0]);

        SubscribeData();
    }

    logger_.LogInfo() << " WeatherClientMock::serviceAvailabilityCallback() --";
}

void WeatherClientMock::SubscribeData() {
    logger_.LogInfo() << " WeatherClientMock::SubscribeData() ++";
    std::size_t subscriptionChacheSize = 1; // max sample count
    if (nullptr != proxy_)
    {
        proxy_->CurrentTemperature.SetReceiveHandler(std::bind(&WeatherClientMock::OnCurrentTemperatureChange, this));
        proxy_->CurrentTemperature.Subscribe(subscriptionChacheSize);
    }
    logger_.LogInfo() << " WeatherClientMock::SubscribeData() --";
}

void WeatherClientMock::OnCurrentTemperatureChange() {
    logger_.LogInfo() << " WeatherClientMock::OnCurrentTemperatureChange() ++";
    std::lock_guard<std::mutex> lock(proxy_mutex_);

    proxy_->CurrentTemperature.GetNewSamples(
        [this](const auto& sample){
            uint8_t temperature = *sample;
            currentTemperature_ = temperature;
            logger_.LogInfo() << "Current temperature: " << temperature;
        }
    );

    logger_.LogInfo() << " WeatherClientMock::OnCurrentTemperatureChange() --";
}

uint8_t WeatherClientMock::getCurrentTemperature() {
    logger_.LogInfo() << " getCurrentTemperature ++";
    return currentTemperature_;
    logger_.LogInfo() << " WeatherClientMock::OnCurrentTemperatureChange() --";
}

std::string WeatherClientMock::getWeatherForcast(std::string date) {
    logger_.LogInfo() << " WeatherClientMock::getWeatherForcast() ++";
    std::string forecast;
    if(nullptr != proxy_)
    {
        std::lock_guard<std::mutex> lock(proxy_mutex_);
        auto forecastFuture = proxy_->GetWeatherForecast(date);
        forecast = static_cast<std::string>(forecastFuture.get().Forecast.c_str());
        logger_.LogInfo() << "Weather Forecast: " << forecast;
    }
    
    logger_.LogInfo() << " WeatherClientMock::getWeatherForcast() --";
    return forecast;
}

}//ends demo
}// ends ara