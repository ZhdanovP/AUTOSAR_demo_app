#include "ITemperatureService.h"
#include "TemperatureProviderMock.h"
#include "WeatherClientMock.h"

#include "FunctionalTestEnvironment.h"

#include <iostream>
#include <gtest/gtest.h>

using ::testing::Return;

class WeatherProviderFunctionalTests : public ::testing::Test
{
  public:
    WeatherProviderFunctionalTests() = default;
    ~WeatherProviderFunctionalTests() = default;
 protected:
  ::TemperatureServiceMock* temperatureServiceMock_raw = FunctionalTestsEnvironment::temperatureServiceMock.get();
  ara::demo::WeatherClientMock* weatherClientMock_raw = FunctionalTestsEnvironment::weatherClientMock.get();
  ara::demo::TemperatureProviderMock* temperatureprovierMock_raw = FunctionalTestsEnvironment::temperatureProviderMock.get();
};

TEST_F(WeatherProviderFunctionalTests, getWeatherForecastSuccess)
{
    //arrange
    uint8_t mockTemperature{35};
    EXPECT_CALL(*temperatureServiceMock_raw, getCurrentTemperature()).WillRepeatedly(Return(mockTemperature));
    
    //action
    auto weatherForecast = weatherClientMock_raw->getWeatherForcast("28.04.2022");

    //expectation
    //the response must contain the date provided as input
    EXPECT_TRUE(weatherForecast.find("28.04.2022") != std::string::npos);
    
    //let's wait to receive the current temperature field update callback in the weather client from weather provider 
    //upon getting the new temperature(35) from the temperature provider service 
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    auto temperature = weatherClientMock_raw->getCurrentTemperature();
    EXPECT_EQ(temperature, mockTemperature);

}