#include <iostream>
#include <memory>

#include <gmock/gmock.h>

#include "ITemperatureService.h"
#include "TemperatureProviderMock.h"
#include "WeatherClientMock.h"

class TemperatureServiceMock: public ara::demo::ITemperatureService
{
 public:
    TemperatureServiceMock()           = default;
    ~TemperatureServiceMock() override = default;
    MOCK_METHOD0(getCurrentTemperature, uint8_t());
};


class FunctionalTestsEnvironment : public ::testing::Environment
{
public:
void SetUp() override;

virtual void TearDown() override;

static std::unique_ptr<ara::demo::TemperatureProviderMock> temperatureProviderMock;
static std::unique_ptr<ara::demo::WeatherClientMock> weatherClientMock;
static std::unique_ptr<::TemperatureServiceMock> temperatureServiceMock;

};