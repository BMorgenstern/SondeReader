#include "mainwindow.h"
#include <QApplication>

#include "serialio.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    if(!w.connected())
    {
        //terminate the program if no SerialPort
        return 0;
    }
    else
    {
        w.setTab(0);
        w.setWindowTitle("Sonde Reader");
        w.show();
        //w.newSession();
        //Same as writing newsession via serialmonitor
    }

    return a.exec();

}
