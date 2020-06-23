#ifndef SERIALMONITOR_H
#define SERIALMONITOR_H

#include <QWidget>

#include "serialio.h"

namespace Ui {
class SerialMonitor;
}

class SerialMonitor : public QWidget
{
    Q_OBJECT

public:
    explicit SerialMonitor(QWidget *parent = nullptr);
    SerialMonitor(SerialIO*);
    void initPort(SerialIO* p);
    ~SerialMonitor();
private slots:
    void printToMonitor(QString); //connect to SerialIO's read signal
    void on_userSubmit_clicked();

    void on_userInput_returnPressed();

private:
    Ui::SerialMonitor *ui;
    SerialIO* port;
signals:
    void writeFromMonitor(QString); //connect to SerialIO's write slot
};

#endif // SERIALMONITOR_H
