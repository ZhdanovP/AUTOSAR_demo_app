#ifndef ITEMPERATURE_SERVICE_H_
#define ITEMPERATURE_SERVICE_H_

#include <iostream>

namespace ara
{
namespace demo
{

class ITemperatureService
{
  public:
    virtual uint8_t getCurrentTemperature() = 0;
    virtual ~ITemperatureService() = default;
};

}
}

#endif //ITEMPERATURE_SERVICE_H_