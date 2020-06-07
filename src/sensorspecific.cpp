#include "sensorspecific.h"

SensorSpecific::SensorSpecific()
{

}

SensorSpecific::SensorSpecific(QWidget* obj, SensorType sensor)
{
    this->widget = obj;
    this->sensor = sensor;
}

SensorSpecific::SensorSpecific(QWidget* obj, int sensor)
{
    this->widget = obj;
    this->sensor = (SensorType)sensor;
}


