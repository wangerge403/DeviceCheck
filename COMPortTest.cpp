#pragma execution_character_set("utf-8")
#include "COMPortTest.h"
#include "ui_COMPortTest.h"
#include <qdebug.h>
#include "change.h"

#ifdef Q_OS_WIN
#include <windows.h>
#include "dbt.h"
#include "initguid.h"
#include "usbiodef.h"
#include "hidclass.h"
#include "ntddkbd.h"
#include "ntddmou.h"
#pragma comment(lib, "user32.lib")
#endif

COMPortTest::COMPortTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::COMPortTest)
{
    ui->setupUi(this);

    this->setWindowTitle("热插拔检测");
    this->resize(680, 480);
    ComChange::getInstance()->setHWND((HWND)this->winId());
    // 2.鼠标、键盘注册通知
    registerGUID();

    // 1.端口号
    connect(ComChange::getInstance(), &ComChange::comStatus, this, &COMPortTest::on_comStatus);
    // 2. 鼠标、键盘信号槽
    connect(ComChange::getInstance(), &ComChange::usbStatus, this, &COMPortTest::on_USBStatus);
    // 获取所有可用串口
    QStringList strName = ComChange::getAvailablePort();
    ui->comboBox->addItems(strName);
}

// 端口号
COMPortTest::~COMPortTest()
{
    delete ui;
}

// 更新COM
void COMPortTest::on_comStatus(QString name, bool flag)
{
    if(flag) // 串口插入时自动添加串口名
    {
        if(ui->comboBox->findText(name) < 0) // 新增的串口名
        {
            ui->comboBox->addItem(name);
        }
    }
    else // 串口拔出时自动移除串口名
    {
        ui->comboBox->removeItem(ui->comboBox->findText(name));
    }
}

// 鼠标、键盘状态槽函数
void COMPortTest::on_USBStatus(QString name) {
    qDebug() << "鼠标、键盘变化：" << name;
    ui->textEdit->append(name);
}

// 鼠标、键盘
// 给设备注册通知
void COMPortTest::registerGUID()
{
#ifdef Q_OS_WIN
    qDebug() << "设备注册通知";
    DEV_BROADCAST_DEVICEINTERFACE mouseInterface;
    ZeroMemory(&mouseInterface, sizeof(DEV_BROADCAST_DEVICEINTERFACE));
    mouseInterface.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    mouseInterface.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    mouseInterface.dbcc_classguid = GUID_DEVINTERFACE_MOUSE ;
    if(!RegisterDeviceNotificationW((HANDLE)this->winId(), &mouseInterface, DEVICE_NOTIFY_WINDOW_HANDLE))
    {
        qDebug() << "鼠标GUID注册失败~";
    }

    DEV_BROADCAST_DEVICEINTERFACE keyboardInterface;
    ZeroMemory(&keyboardInterface, sizeof(DEV_BROADCAST_DEVICEINTERFACE));
    keyboardInterface.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    keyboardInterface.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    keyboardInterface.dbcc_classguid = GUID_DEVINTERFACE_KEYBOARD;
    if(!RegisterDeviceNotificationW((HANDLE)this->winId(), &keyboardInterface, DEVICE_NOTIFY_WINDOW_HANDLE))
    {
        qDebug() << "键盘GUID注册失败~";
    }
#endif
}

bool COMPortTest::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
qDebug() << "配置变化~";
#ifdef Q_OS_WIN
    if(eventType == "windows_generic_MSG")
    {
        MSG* msg = reinterpret_cast<MSG*>(message);
        qDebug() << "硬件配置变化<<<<<<：" << msg->message;
        // 通知应用程序设备或计算机的硬件配置发生更改。
        if(msg->message == WM_DEVICECHANGE)
        {
            PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)msg->lParam;
            PDEV_BROADCAST_DEVICEINTERFACE lpdbv = (PDEV_BROADCAST_DEVICEINTERFACE)lpdb;
            switch (msg->wParam)
            {
            case DBT_DEVICEARRIVAL:// 插入
            {
                if (lpdb->dbch_devicetype == DBT_DEVTYP_PORT) // 设备类型为串口
                {
                    PDEV_BROADCAST_PORT lpdbv = (PDEV_BROADCAST_PORT)lpdb;
                    QString strName = QString::fromWCharArray(lpdbv->dbcp_name); //插入的串口名
                    qDebug() << strName;
                }
                // 鼠标、键盘
                if(lpdb->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
                {
                    if(lpdbv->dbcc_classguid == GUID_DEVINTERFACE_KEYBOARD)
                    {
                        ui->textEdit->append("键盘接入：" + QString::fromWCharArray(lpdbv->dbcc_name));
                    }
                    else if(lpdbv->dbcc_classguid == GUID_DEVINTERFACE_MOUSE)
                    {
                        ui->textEdit->append("鼠标接入：" + QString::fromWCharArray(lpdbv->dbcc_name));
                    }
                }
                break;
            }
            case DBT_DEVICEREMOVECOMPLETE: // 拔出
            {
                if (lpdb->dbch_devicetype == DBT_DEVTYP_PORT) // 设备类型为串口
                {
                    PDEV_BROADCAST_PORT lpdbv = (PDEV_BROADCAST_PORT)lpdb;
                    QString strName = QString::fromWCharArray(lpdbv->dbcp_name); //拔出的串口名
                    qDebug() << strName;
                }
                // 鼠标、键盘
                if(lpdb->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
                {
                    if(lpdbv->dbcc_classguid == GUID_DEVINTERFACE_KEYBOARD)
                    {
                        ui->textEdit->append("键盘拔出：" + QString::fromWCharArray(lpdbv->dbcc_name));
                    }
                    else if(lpdbv->dbcc_classguid == GUID_DEVINTERFACE_MOUSE)
                    {
                        ui->textEdit->append("鼠标拔出：" + QString::fromWCharArray(lpdbv->dbcc_name));
                    }
                }
                break;
            }
            default:
                break;
            }
        }
    }
#endif

    return false;
}
