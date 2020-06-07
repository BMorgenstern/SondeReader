#include "serialhandler.h"

Serialhandler::Serialhandler()
{

}


Serialhandler::Serialhandler(const QSerialPortInfo& info) : QSerialPort(info)
{
    this->workerThread = new QThread();
    this->moveToThread(this->workerThread);
}
