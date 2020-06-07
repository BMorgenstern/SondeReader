#include "serialio.h"

SerialIO::SerialIO()
{
    this->messages = QQueue<QString>();
}

SerialIO::SerialIO(const QSerialPortInfo& info) : QSerialPort(info)
{
    this->messages = QQueue<QString>();
}

void SerialIO::serialRead()
{
#ifdef TESTING_SERIALIO
    reading = true;
    this->sleep(tts);
    qDebug() << tts << " seconds have passed";
    reading = false;
#elif 0
    QString result;
    if(nullptr != this->input)
    {
        QByteArray buffer;
        reading = true;
        int timeOut = 0;
        while(!buffer.endsWith('\r') && !buffer.endsWith('\n') && timeOut < 500)
        {
            buffer.append(this->input->readAll());
            qDebug() << buffer;
            timeOut++;
        }
        reading = false;
        qDebug() << buffer;//error check ~!!
        qDebug() << "-----END TRASMISSION-----";
        emit doneReading("Hello");
    }
    emit doneReading(result);
#else
    qDebug() << this->readAll();
#endif
}
void SerialIO::serialWrite(QString message)
{
#ifndef TESTING_SERIALIO
    if(this->open(QSerialPort::ReadWrite))
    {
        this->write(message.toUtf8());
    }
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
