// 扫描蓝牙设备

#ifndef BLUETOOTHTEST_H
#define BLUETOOTHTEST_H

#include <QWidget>
#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
#include <QtBluetooth/QBluetoothDeviceInfo>


namespace Ui {
class BluetoothTest;
}

class BluetoothTest : public QWidget
{
    Q_OBJECT

public:
    explicit BluetoothTest(QWidget *parent = nullptr);
    ~BluetoothTest();
    void scanForBluetoothDevices(); //扫描蓝牙设备

private:

public slots:
    void deviceDiscoveredHandler_slot(const QBluetoothDeviceInfo &info);
    // void scanError_slot(QBluetoothDeviceDiscoveryAgent::Error error);

private:
    Ui::BluetoothTest *ui;
};


#endif // BLUETOOTHTEST_H
