#include "serialparser.h"

#define PRINTSERIAL

SerialParser::SerialParser(QObject *parent) : QObject(parent)
{
    worker = nullptr;
    status = "";
}

void SerialParser::parse(QString message)
{
    /*Here we test parsing*/
#ifdef PRINTSERIAL
    qDebug() << message;
#endif
    QString retData;

    if(-1 != message.indexOf("Success"))
    {
        this->status = "Success";
    }
    if(-1 != message.indexOf("Failed"))
    {
        this->status = "Failed";
    }

    auto datums = message.split("_data:");

    if(2 == datums.length())
    {
        QString sensor = datums.at(0);
        QString res = datums.at(1);
        res.remove('\r');
            //QString status = datums.at(1).mid(1,datums.at(1).indexOf('\r')-1);
        if(!this->status.isEmpty())
        {
            emit calibrationResult(sensor+'\r'+this->status+'\r'+res);
            this->status = "";
        }
        else
        {
            emit reading(sensor+'\r'+res);
        }

    }
    //more options

    //QString sensor = datums.at(0);
    //QString res = datums.at(2).mid(0, datums.at(2).indexOf('\r'));
    //QString status = datums.at(1).mid(1,datums.at(1).indexOf('\r')-1);
    //emit calibrationResult(sensor+'\r'+status+'\r'+res);

}

