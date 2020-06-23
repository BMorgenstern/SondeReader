#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sondedata.h"
#include "sondemode.h"
#include "sensordetail.h"
#include "sensorspecific.h"
#include "serialio.h"
#include "serialparser.h"
#include "commandresult.h"
#include "serialmonitor.h"

#include <QMainWindow>
#include <QMessageBox>
#include <QPointer>
#include <QQueue>
#include <QThread>

#include <QSerialPort>
#include <QSerialPortInfo>

#include <QDebug>

#define SONDE_BAUD QSerialPort::Baud9600
#define ARDUINO_BAUD QSerialPort::Baud115200




namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void ReadSerial();
    void getReading(QString);

    void calRes(QString);

    void on_serialSend_clicked();

    void on_readFrom_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_currSensor_currentIndexChanged(const QString &arg1);



    void on_sensorInfo_clicked();

    void on_sensorRead_clicked();

    void on_exportCal_clicked();

    void on_calInfo_clicked();

    void on_findSensor_clicked();


    void on_setProbe_clicked();

    void on_currProbe_clicked();

    void on_sensorStatus_clicked();

    void on_currSensor_currentIndexChanged(int index);

    void on_sensorSleep_clicked();

    void on_sensorFactory_clicked();

    void on_currTemp_clicked();

    void on_setTemp_clicked();

    void on_calDry_clicked();

    void on_calClear_clicked();

    void on_calN_clicked();


    void on_sensorRead_2_clicked();

    void on_changeSettings_clicked();

    void on_rtdTemp_clicked();

    void on_probeSlope_clicked();

    void on_calOptions_currentChanged(int index);

    void on_loCal_clicked();

    void on_hiCal_clicked();

    void on_midCalButton_clicked();

    void on_setPressure_clicked();

    void on_currPressure_clicked();

    void on_setDLI_clicked();

    void on_disableDL_clicked();

    void on_getDLI_clicked();

//    void on_var1_clicked();

    void on_memoryClear_clicked();

    void on_memoryLocation_clicked();

    void on_memoryRecall_clicked();

    void on_setProbe_2_clicked();

    void on_currSal_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_freq_submit_clicked();

    void on_coding_rate_submit_clicked();

    void on_spreading_factor_submit_clicked();

    void on_power_submit_clicked();

    void on_bandwidth_submit_clicked();

private:
    Ui::MainWindow *ui;
    SondeMode::ModeSelection currentMode;
    bool reading;
    QByteArray buffer;
    QQueue<QString> writeMessages;
    SerialIO* port;
    static const qint16 SONDEID = 1027;
    static const qint16 ARDUNIOID = 9025;
    void CalCommand(QString,QString,QString,bool);
    SerialParser* parser;
    SerialMonitor* monitor;
    QList<SensorDetail> sensorNames;
    QList<SensorSpecific> sensorSpecificWidgets;


public:
    void setClickedTitle();

    void setLCD(QString type, QString val);
    inline bool connected()
    {
        return nullptr != this->port;
    }
    inline SerialIO* getPort()
    {
        return this->port;
    }
    bool sendSerial(QString);
    void newSession();
    void hideAllSensorSpecificWidgets();
    void toggleSensorSpecificWidgets(SensorSpecific::SensorType);
    void changeMode(SondeMode::ModeSelection);
    void setTab(int);

    SondeMode::ModeSelection getMode()
    {
        return this->currentMode;
    }
    void setSerial(const QSerialPortInfo& info)
    {
        port = new SerialIO(info);

    }
    signals:
        void writeToSonde(QString);
        //void serialRead(unsigned long);
        //void doneWithSerial();
};

#endif // MAINWINDOW_H
