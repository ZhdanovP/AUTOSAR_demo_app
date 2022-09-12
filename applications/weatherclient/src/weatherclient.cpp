#include "weatherclient.h"
#include "ara/core/instance_specifier.h"
#include <threads.h>
#include<string>
#include<chrono>

namespace ara
{
namespace demo
{
WeatherClient::WeatherClient():
                storage_{nullptr},
                currentTemperature_{0} 
{

}

void WeatherClient::Init() {
     logger_.LogInfo() << " WeatherClient::Init() ++";

    //Initialize persistency store

    auto openResult = ara::per::OpenKeyValueStorage(kvsInstanceSpecifier_);
    if (!openResult) {
        logger_.LogError() << "Failed to open key-value-storage due to: " << openResult.Error().Message();
        throw std::runtime_error("Could not create KeyValueStorage");
    }

    storage_ = std::move(openResult).Value();


    auto result = getPersistedTemperature();
    if(result.first)
    {
        currentTemperature_ = result.second;
        logger_.LogInfo() << "Last known temperature from persistency: " << currentTemperature_;
    }

    Proxy::StartFindService(
        [this](ara::com::ServiceHandleContainer<Proxy::HandleType> handles, ara::com::FindServiceHandle handler) {
            serviceAvailabilityCallback(std::move(handles), handler);
        },
        ara::core::InstanceSpecifier("weatherclient_exe/root_swc/WeatherServiceRPort")
    );

    logger_.LogInfo() << " WeatherClient::Init() --";
}

void WeatherClient::serviceAvailabilityCallback(ara::com::ServiceHandleContainer<Proxy::HandleType> handles,
        ara::com::FindServiceHandle handler)
{
    logger_.LogInfo() << " WeatherClient::serviceAvailabilityCallback() ++";
    (void)handler;
    if (handles.size() <= 0) {
        return;
    }

    std::lock_guard<std::mutex> lock(proxy_mutex_);
    if (nullptr == proxy_) {
        proxy_ = std::make_shared<Proxy>(handles[0]);

        SubscribeData();
    }

    logger_.LogInfo() << " WeatherClient::serviceAvailabilityCallback() --";
}

void WeatherClient::SubscribeData() {
    logger_.LogInfo() << " WeatherClient::SubscribeData() ++";
    std::size_t subscriptionChacheSize = 1; // max sample count
    if (nullptr != proxy_)
    {
        proxy_->CurrentTemperature.SetReceiveHandler(std::bind(&WeatherClient::OnCurrentTemperatureChange, this));
        proxy_->CurrentTemperature.Subscribe(subscriptionChacheSize);
    }
    logger_.LogInfo() << " WeatherClient::SubscribeData() --";
}

void WeatherClient::OnCurrentTemperatureChange() {
    logger_.LogInfo() << " WeatherClient::OnCurrentTemperatureChange() ++";
    std::lock_guard<std::mutex> lock(proxy_mutex_);

    proxy_->CurrentTemperature.GetNewSamples(
        [this](const auto& sample){
            uint8_t temperature = *sample;
            currentTemperature_ = temperature;
            logger_.LogInfo() << "Current temperature: " << temperature;
        }
    );

    logger_.LogInfo() << " WeatherClient::OnCurrentTemperatureChange() --";
}

void WeatherClient::getPeriodicWeatherForcast() {
    logger_.LogInfo() << " WeatherClient::getPeriodicWeatherForcast() ++";
    uint8_t count{0};
    DateType date{};
    constexpr uint8_t todayDate{28};
    const std::string thisMonthYear{".04.2022"};
    constexpr uint8_t MAX_COUNT{6};
    while(true)
    {

        auto day = todayDate + count;
        date = std::to_string(day) + thisMonthYear;
        if(nullptr != proxy_)
        {
            std::lock_guard<std::mutex> lock(proxy_mutex_);
            auto forecastFuture = proxy_->GetWeatherForecast(date);
            try
            {
                logger_.LogInfo() << "Weather Forecast: " << forecastFuture.get().Forecast;
            }
            catch(std::exception& e)
            {
                logger_.LogError() << "Got exception " << e.what();
            }
        }
        count++;
        if(MAX_COUNT == count )
        {
            count = 0;
        }
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
    logger_.LogInfo() << " WeatherClient::getPeriodicWeatherForcast() --";
}

bool WeatherClient::persistTemperature(uint8_t temperature)
{
    bool status{true};
    try {
        // Save data value
        ara::core::String key{"temperature"};
        storage_->RemoveKey(key);
        storage_->SetValue<uint8_t>(key, temperature);
        storage_->SyncToStorage();
    }
    // ara::per::ExceptionLogicError, ara::per::ExceptionPhysicalStorageError are handled here
    catch (const std::exception& e) {
        /* RULECHECKER_comment(2:0,2:0, check_macro_expansion_expression,
        "Won't fix", true_no_defect)*/
        logger_.LogError() << "[" << __FILE__ << ":" << __LINE__
                           << "]: Error writing data to persistency: " << e.what();
        status = false;
    }
    return status;
}


std::pair<bool, uint8_t> WeatherClient::getPersistedTemperature()
{
    uint8_t dataValue;
    ara::core::String key{"temperature"};
    bool result = false;

    auto entryvalue = storage_->GetValue<uint8_t>(key);

    if(entryvalue.HasValue())
    {
        dataValue = entryvalue.Value();
        result = true;
    }
    else
    {
        auto error = entryvalue.Error();
        if (error.Value() == static_cast<int>(ara::per::PerErrc::kKeyNotFound))
        {
            logger_.LogError() << "[" << __FILE__ << ":" << __LINE__ << "]: Key not found: " << key;
        }
        else
        {
            logger_.LogError() << "[" << __FILE__ << ":" << __LINE__ << "]: Failed to read, key: " << key;
        }
    }

    return {result, dataValue};
}

WeatherClient::~WeatherClient()
{
    if(false == persistTemperature(currentTemperature_))
    {
        logger_.LogError() << "Error storing temperature into persistency: ";
    }
}

}//ends demo
}// ends ara