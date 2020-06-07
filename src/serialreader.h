#ifndef SERIALREADER_H
#define SERIALREADER_H

#include <QString>
#include <QThread>
#include <QSerialPort>
#include <QDebug>

class SerialReader : public QThread
{
    Q_OBJECT
    QSerialPort* input;
public:
    SerialReader(QSerialPort*);
    void run() override{
        }
    public slots:
        void sread()
        {

        }
    signals:
        void resultReady(const QString &s);
};

#endif // SERIALREADER_H
