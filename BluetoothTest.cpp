#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
#include <QtBluetooth/QBluetoothLocalDevice>
#include <QtBluetooth/QBluetoothAddress>
#include <QtBluetooth/QBluetoothDeviceInfo>
#include <QOVerload>
#include <BluetoothTest.h>
#include <QList>
#include <QObject>
#include <QDebug>
#include <ui_bluetoothTest.h>

BluetoothTest::BluetoothTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BluetoothTest)
{
    ui->setupUi(this);

    this->setWindowTitle("显示蓝牙设备");
    this->resize(600, 480);


    QBluetoothDeviceDiscoveryAgent *agent = new QBluetoothDeviceDiscoveryAgent(this);
    qDebug() << "蓝牙实例：" << agent ;
    connect(agent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &BluetoothTest::deviceDiscoveredHandler_slot);
    // connect(agent, &QBluetoothDeviceDiscoveryAgent::finished,
    //         this, &YourClass::scanFinished);
    // connect(agent, QOverload<QBluetoothDeviceDiscoveryAgent::Error>::of(&QBluetoothDeviceDiscoveryAgent::error),
    //         this, &BluetoothTest::scanError_slot);


}

BluetoothTest::~BluetoothTest()
{
    delete ui;
}


void BluetoothTest::deviceDiscoveredHandler_slot(const QBluetoothDeviceInfo &info) {
    qDebug() << "Discovered Bluetooth device:" << info.name() << info.address().toString();
    // 处理扫描到的设备信息
}

// 错误处理槽函数
// void BluetoothTest::scanError_slot(QBluetoothDeviceDiscoveryAgent::Error error) {
//     qDebug() << "Bluetooth scan error:" << error;
//     // 处理可能出现的错误
// }
void BluetoothTest::scanForBluetoothDevices() {
}
