#include <QApplication>
#include "COMPortTest.h"
// #include <BluetoothTest.h> // 扫描蓝牙设备

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    COMPortTest w;
    w.show();

    return a.exec();
}
