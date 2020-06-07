#ifndef SENSORDETAIL_H
#define SENSORDETAIL_H

#include <QObject>

class SensorDetail
{
public:
    QString sensorName;
    QString sensorBlurb;
    SensorDetail();
    SensorDetail(QString, QString);
    /*
    inline QString name()
    {
        return this->sensorName;
    }
    inline QString blurb()
    {
        return this->sensorBlurb;
    }
    */
};

#endif // SENSORDETAIL_H
