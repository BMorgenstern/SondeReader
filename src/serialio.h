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
        void serialWrite(QString);
signals:
        void doneReading(QString);
        void doneWriting();
};

#endif // SERIALIO_H

#ifdef NOTDEFINED
QString data = "_data:";
data.indexOf("data");
int start = buffer.toStdString().find(data.toStdString().c_str()) + data.length();
int len = buffer.indexOf('\r', start)-start;

if(0 < len)
{
    data = QString::fromStdString(this->buffer.mid(start, len).toStdString());
}
else
{
    data = "";
}/*God help me*/

//qDebug() << "Data : " << data;

//qDebug() << this->buffer.toStdString().c_str();
//qDebug() << this->buffer.length() << this->buffer.toStdString().find("Success");

if((this->buffer.length() >= this->buffer.toStdString().find("Success")) && (this->buffer.length() < this->buffer.toStdString().find("Fail")))
{
    QString prefix = (sensor + " " + command + args);
    if(data.isEmpty())
    {
        if(expectResult)
        {
            CalCommand(sensor, command, args, expectResult);
            return;
        }
        ui->readOut->append(prefix + " : " + "Success");

    }
    else
    {
        ui->readOut->append(prefix + " : " + data);
    }

}
else
{
    //s.setText("Something went wrong");
    //s.exec();
    //Uncomment Later
}


#endif
