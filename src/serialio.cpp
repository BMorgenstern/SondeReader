#include "serialio.h"

SerialIO::SerialIO()
{
    this->messages = QQueue<const char*>();
    this->setReading(false);
    buffer = new QByteArray();
}

SerialIO::SerialIO(const QSerialPortInfo& info) : QSerialPort(info)
{
    this->messages = QQueue<const char*>();
    this->setReading(false);
    buffer = new QByteArray();
}

SerialIO::~SerialIO()
{
   if(nullptr != buffer)
   {
       delete buffer;
   }
}

void SerialIO::serialRead()
{
        if(nullptr == buffer)
        {
            buffer = new QByteArray();
        }
        this->setReading(true);
        buffer->append(this->readAll());
        if(buffer->endsWith('\n'))
        {
            //newline character specifies end of message
            //qDebug() << *buffer;
            emit doneReading(QString(*buffer));
            buffer->clear();
            this->setReading(false);
        }
}
void SerialIO::serialWrite(const char* message)
{
#ifndef TESTING_SERIALIO
    while(this->reading)
    {
        qDebug() << "Waiting....";
    } // wait for reading to finish..
    qDebug() << "Writing : " << message;
    this->write(message);
    emit doneWriting();
#else
    qDebug() << ((reading) ? "Reading..." : "Not reading!");
    qDebug() << message;
    emit doneWriting();
#endif
}

void SerialIO::sDoneReading()
{
    qDebug() << "Finished Reading!";
}
