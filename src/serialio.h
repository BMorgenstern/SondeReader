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
    QQueue<const char*> messages;
    std::atomic_bool reading;
public:
    QThread* worker;
    QByteArray* buffer;
    inline void setReading(bool isReading)
    {
        this->reading = isReading;
    }
    SerialIO();
    ~SerialIO();
    SerialIO(const QSerialPortInfo&);
private slots:
        void sDoneReading();
        void serialRead();
        void serialWrite(const char*);
signals:
        void doneReading(QString);
        void doneWriting();
};

#endif // SERIALIO_H
