#include "serialparser.h"

#define PRINTSERIAL

SerialParser::SerialParser(QObject *parent) : QObject(parent)
{
    worker = nullptr;
}

void SerialParser::parse(QString message)
{
    /*Here we test parsing*/
#ifdef PRINTSERIAL
    qDebug() << message;
#endif
    QString retData;

    auto datums = message.split(':');
    if(-1 == message.indexOf("_data:") && 3 != datums.length())
    {
        return;//not found
    }
    QString sensor = datums.at(0);
    QString res = datums.at(2).mid(0, datums.at(2).indexOf('\r'));
    QString status = datums.at(1).mid(1,datums.at(1).indexOf('\r')-1);
    emit calibrationResult(sensor+'\r'+status+'\r'+res);

}

