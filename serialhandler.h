#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

#include <QSerialPort>
#include <QThread>

class Serialhandler : public QSerialPort
{
    Q_OBJECT
    QThread* workerThread;
    Serialhandler();
public:
    Serialhandler(const QSerialPortInfo&);

};

#endif // SERIALHANDLER_H
