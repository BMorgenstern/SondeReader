#ifndef SENSORSPECIFIC
#define SENSORSPECIFIC

#include <QObject>

class SensorSpecific{
    private:
        SensorSpecific();
    public:
        enum SensorType {EC = 1 << 0, ORP = 1 << 1, pH = 1 << 2, DO = 1 << 3, RTD = 1 << 4};
        QWidget* widget;
        SensorType sensor;
        SensorSpecific(QWidget*, SensorType);
        SensorSpecific(QWidget*, int);
};

#endif // SENSORSPECIFIC

