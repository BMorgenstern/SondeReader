#ifndef SERIALIO_H
#define SERIALIO_H


#include <QString>
#include <QQueue>
#include <QSerialPort>
#include <QDebug>
#include <atomic>


//#define TESTING_SERIALIO

class SerialIO : public QSerialPort
{
    Q_OBJECT
    QQueue<QString> messages;
    std::atomic_bool reading;
public:
    QThread* worker;
    inline void setReading(bool isReading)
    {
        this->reading = isReading;
    }
    SerialIO();
    SerialIO(const QSerialPortInfo&);
private slots:
        void sDoneReading();
        void serialRead();
        void serialWrite(QString);
signals:
        void doneReading(QString);
        void doneWriting();
};

#endif // SERIALIO_H
