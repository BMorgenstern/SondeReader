#include "serialio.h"

SerialIO::SerialIO()
{
    this->messages = QQueue<QString>();
    this->setReading(false);
    buffer = new QByteArray();
}

SerialIO::SerialIO(const QSerialPortInfo& info) : QSerialPort(info)
{
    this->messages = QQueue<QString>();
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
        auto lines = buffer->split('\n');
        for(int counter = 0; counter < lines.length() - 1; counter++)
        {
            emit doneReading(lines.at(counter));
        }
        *buffer = lines.last();
        this->setReading(false);
        /*
        if(buffer->endsWith('\n'))
        {
            //newline character specifies end of message
            //qDebug() << *buffer;
            this->setReading(false);
            emit doneReading(QString(*buffer));
            buffer->clear();

        }
        */
}
void SerialIO::serialWrite(QString message)
{
#ifndef TESTING_SERIALIO
    while(this->reading)
    {
        qDebug() << "Waiting....";
    } // wait for reading to finish..
    qDebug() << "Writing : " << message;
    this->write(message.toStdString().c_str());
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
