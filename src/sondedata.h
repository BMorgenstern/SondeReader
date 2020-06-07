#ifndef SONDEDATA_H
#define SONDEDATA_H
#include <QTime>

class SondeData
{
private:
    double ec;
    double tds;
    double sal;
    double sg;
    double Do;
    double sat;
    double ph;
    double orp;
    double rtd;
    QTime currTime;
public:
    SondeData();
    SondeData(QList<QByteArray>);
    QTime getTime();
    inline double getEC(){ return this->ec; }
    inline double getTDS() { return this->tds; }
    inline double getSAL() { return this->sal; }
    inline double getSG(){ return this->sg; }
    inline double getDO() { return this->Do; }
    inline double getSAT() { return this->sat; }
    inline double getPH(){ return this->ph; }
    inline double getORP() { return this->orp; }
    inline double getRTD() { return this->rtd; }

};

#endif // SONDEDATA_H
