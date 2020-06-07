#include "sondedata.h"
#include <QTime>
SondeData::SondeData()
{
    this->ec = 0;
    this->tds = 0;
    this->sal = 0;
    this->sg = 0;
    this->Do = 0;
    this->sat = 0;
    this->ph = 0;
    this->orp = 0;
    this->rtd = 0;
    this->currTime = QTime::currentTime();
}

SondeData::SondeData(QList<QByteArray> args)
{
    if(10 != args.length() )
    {
        SondeData();
        return;
    }
    this->ec = args.at(0).toDouble();
    this->tds = args.at(1).toDouble();
    this->sal = args.at(2).toDouble();
    this->sg = args.at(3).toDouble();
    this->Do = args.at(4).toDouble();
    this->sat = args.at(5).toDouble();
    this->ph = args.at(6).toDouble();
    this->orp = args.at(7).toDouble();
    this->rtd = args.at(8).toDouble();
    this->currTime = QTime::fromString(args.at(9));
    //Convert UTC to EST
    //this->currTime.setHMS(this->currTime.hour()-5, this->currTime.minute(), this->currTime.second());
}

QTime SondeData::getTime()
{
    return this->currTime;
}
