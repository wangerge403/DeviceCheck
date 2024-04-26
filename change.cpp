#pragma execution_character_set("utf-8")
#include "change.h"
#include <QApplication>
#include <QMutex>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <qdebug.h>

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

ComChange::ComChange(QObject *parent) : QObject(parent)
{
    qApp->installNativeEventFilter(this);  // 安装事件过滤器

}

ComChange* ComChange::m_comChange = nullptr;

ComChange *ComChange::getInstance() {
    if(m_comChange == nullptr)
    {
        static QMutex mutex;         //实例互斥锁。
        QMutexLocker locker(&mutex); //加互斥锁。
        if(m_comChange == nullptr)
        {
            m_comChange = new ComChange();
        }
    }
    return m_comChange;

}

// 获取系统中所有可用的串口名
QStringList ComChange::getAvailablePort()
{
    QStringList strName;
    foreach(const QSerialPortInfo& info, QSerialPortInfo::availablePorts())
    {
        QSerialPort port(info);
        if(port.open(QIODevice::ReadWrite))
        {
            strName << info.portName();
            port.close();
        }
    }
    return strName;
}

// 设置窗口句柄用于过滤事件
void ComChange::setHWND(HWND hwnd)
{
    this->m_hwnd = hwnd;
}

bool ComChange::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) {
    if(eventType == "windows_generic_MSG")  // Windows消息
    {
        qDebug() << "Windows消息";
        MSG* msg = reinterpret_cast<MSG*>(message);

        if(msg->message == WM_DEVICECHANGE  // 通知应用程序设备或计算机的硬件配置发生更改。
          && msg->hwnd == this->m_hwnd)  // 过滤事件
        {
            PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)msg->lParam;
            PDEV_BROADCAST_DEVICEINTERFACE lpdbv = (PDEV_BROADCAST_DEVICEINTERFACE)lpdb;
            switch (msg->wParam)
            {
                case DBT_DEVICEARRIVAL:// 设备接入
                {
                    if (lpdb->dbch_devicetype == DBT_DEVTYP_PORT)  // 设备类型为串口
                    {
                        PDEV_BROADCAST_PORT lpdbv = (PDEV_BROADCAST_PORT)lpdb;
                        QString strName = QString::fromWCharArray(lpdbv->dbcp_name);  //插入的串口名
                        emit comStatus(strName, true);
                    }
                    // 鼠标、键盘
                    if(lpdb->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
                    {
                        if(lpdbv->dbcc_classguid == GUID_DEVINTERFACE_KEYBOARD)
                        {
                            emit usbStatus("键盘接入：" + QString::fromWCharArray(lpdbv->dbcc_name));
                        }
                        else if(lpdbv->dbcc_classguid == GUID_DEVINTERFACE_MOUSE)
                        {
                            emit usbStatus("鼠标接入：" + QString::fromWCharArray(lpdbv->dbcc_name));
                        }
                    }
                    break;
                }
                case DBT_DEVICEREMOVECOMPLETE:  // 移除情况
                {
                    if (lpdb->dbch_devicetype == DBT_DEVTYP_PORT)  // 设备类型为串口
                    {
                        PDEV_BROADCAST_PORT lpdbv = (PDEV_BROADCAST_PORT)lpdb;
                        QString strName = QString::fromWCharArray(lpdbv->dbcp_name);  //拔出的串口名
                        emit comStatus(strName, false);
                    }
                    // 鼠标、键盘
                    if(lpdb->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
                    {
                        if(lpdbv->dbcc_classguid == GUID_DEVINTERFACE_KEYBOARD)
                        {
                            emit usbStatus("键盘拔出：" + QString::fromWCharArray(lpdbv->dbcc_name));
                        }
                        else if(lpdbv->dbcc_classguid == GUID_DEVINTERFACE_MOUSE)
                        {
                            emit usbStatus("鼠标拔出：" + QString::fromWCharArray(lpdbv->dbcc_name));
                        }
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }
    return false;
}
