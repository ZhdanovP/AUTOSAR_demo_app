#include "FunctionalTestEnvironment.h"

std::unique_ptr<ara::demo::TemperatureProviderMock> FunctionalTestsEnvironment::temperatureProviderMock{nullptr};
std::unique_ptr<ara::demo::WeatherClientMock> FunctionalTestsEnvironment::weatherClientMock{nullptr};
std::unique_ptr<::TemperatureServiceMock> FunctionalTestsEnvironment::temperatureServiceMock{nullptr};

void FunctionalTestsEnvironment:: SetUp()
{
    temperatureServiceMock = std::make_unique<::TemperatureServiceMock>();
    temperatureProviderMock = std::make_unique<ara::demo::TemperatureProviderMock>(temperatureServiceMock.get());
    weatherClientMock = std::make_unique<ara::demo::WeatherClientMock>();

    //sleep for 5 seconds so that proxy is able to find the service
    std::this_thread::sleep_for(std::chrono::milliseconds(2000)); 
}

void FunctionalTestsEnvironment::TearDown()
{
    temperatureServiceMock.reset();
    temperatureProviderMock.reset();
    weatherClientMock.reset();
}