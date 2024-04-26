/*
* 通过继承于QAbstractNativeEventFilter来监测串口热插拔由于windows消息会发给每一个窗口，
* 所以如果打开多个窗或者使用到如QComboBox之类的控件，当串口插入或者拔出时会触发多次事件，
* 在这里我使用的解决方法时传入一个窗口句柄，通过窗口句柄过滤事件，但不知道有没有其它更好的办法。
*/
#ifndef CHANGE_H
#define CHANGE_H

#include <QObject>
#include <qabstractnativeeventfilter.h>
#include <qt_windows.h>

class ComChange : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT
public:
    static ComChange* getInstance();
    static QStringList getAvailablePort();       // 获取系统中所有可用的串口名
    void setHWND(HWND hwnd);                     // 设置窗口句柄

private:
    explicit ComChange(QObject *parent = nullptr);

protected:
    bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override;

signals:

    // flag  true：插入 false：拔出
    void comStatus(QString name, bool flag);
    void usbStatus(QString name);

private:
    static ComChange* m_comChange;
    HWND m_hwnd;

};

#endif // CHANGE_H
