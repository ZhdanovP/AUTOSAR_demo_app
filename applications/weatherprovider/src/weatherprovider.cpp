#include "weatherprovider.h"

#include "ara/core/instance_specifier.h"
#include "ara/com/com_error_domain.h"


#include <cstdint>
#include<thread>
#include<chrono>

#pragma GCC optimize("O0")

namespace ara
{
namespace demo
{
WeatherProvider::WeatherProvider() :
    Skeleton(ara::core::InstanceSpecifier("weatherprovider_exe/root_swc/WeatherServicePPort"), ara::com::MethodCallProcessingMode::kEventSingleThread)
{
}

void WeatherProvider::Init() {

    // Register Field Getters.
    logger_.LogInfo() << "WeatherProvider::init ++";
    CurrentTemperature.RegisterGetHandler(std::bind(&WeatherProvider::getCurrentTemperature, this));

    // Initialize Fields Values before Offering Service.
    CurrentTemperature.Update(currentTemperature_);

    OfferService();

    Proxy::StartFindService(
        [this](ara::com::ServiceHandleContainer<Proxy::HandleType> handles, ara::com::FindServiceHandle handler) {
            serviceAvailabilityCallback(std::move(handles), handler);
        },
        ara::core::InstanceSpecifier("weatherprovider_exe/root_swc/TemperatureServiceRPort")
    );

    logger_.LogInfo() << "WeatherProvider::init --";
}

void WeatherProvider::serviceAvailabilityCallback(ara::com::ServiceHandleContainer<Proxy::HandleType> handles,
        ara::com::FindServiceHandle handler)
{
    logger_.LogInfo() << " WeatherProvider::serviceAvailabilityCallback() ++";
    (void)handler;
    if (handles.size() <= 0) {
        logger_.LogError() << " WeatherProvider::serviceAvailabilityCallback() - No service found, handles = 0";
        return;
    }

    std::lock_guard<std::mutex> lock(proxy_mutex_);
    if (nullptr == proxy_) {
        proxy_ = std::make_shared<Proxy>(handles[0]);
        logger_.LogInfo() << " WeatherProvider::serviceAvailabilityCallback() - found service proxy";
    }

    logger_.LogInfo() << " WeatherProvider::serviceAvailabilityCallback() --";
}


ara::core::Future<WeatherService::GetWeatherForecastOutput> WeatherProvider::GetWeatherForecast(
        const ::ara::demo::DateType& date)
{
    logger_.LogInfo() << "WeatherProvider::GetWeatherForecast ++";
    logger_.LogInfo() << "received GetWeatherForecast request";
    ara::core::Promise<ara::demo::WeatherService::GetWeatherForecastOutput> promise;

    WeatherService::GetWeatherForecastOutput output;
    if(date == "28.04.2022")
    {
        output.Forecast = "Date: " + date + "Sunny weather";
    }
    else
    {
        output.Forecast = "Date: " + date + "Rainy weather";
    }

    promise.set_value(output);
    logger_.LogInfo() << "WeatherProvider::GetWeatherForecast --";
    return promise.get_future();
}


ara::core::Future<ara::demo::skeleton::fields::CurrentTemperature::value_type> WeatherProvider::getCurrentTemperature() {
    logger_.LogInfo() << "WeatherProvider::getCurrentTemperature ++";
    ara::core::Promise<ara::demo::skeleton::fields::CurrentTemperature::value_type> promise;
    promise.set_value(currentTemperature_);
    logger_.LogInfo() << "WeatherProvider::getCurrentTemperature --";
    return promise.get_future();
}

void WeatherProvider::monitorTemperature() {
    logger_.LogInfo() << "WeatherProvider::monitorTemperature ++";
    while(true)
    {   
        ::ara::demo::TemperatureType temperature{0};
        {
            std::lock_guard<std::mutex> lock(proxy_mutex_);
            if (nullptr != proxy_) {
                auto temperatureFuture = proxy_->GetCurrentTemperature();
                temperature = temperatureFuture.get().Temperature;
                logger_.LogInfo() << "WeatherProvider::monitorTemperature - received current temperature: "<< temperature;
            }
        }
        
        if(temperature != currentTemperature_)
        {   
            currentTemperature_ = temperature;
            CurrentTemperature.Update(currentTemperature_);
            logger_.LogInfo() << "WeatherProvider::monitorTemperature - Updated the current temperature field ";
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    logger_.LogInfo() << "WeatherProvider::monitorTemperature --";
}

}
}