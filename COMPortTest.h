// 串口检测功能
// 1. 获取所有可用串口 、鼠标、键盘
// 2. 自动检测热插拔
#ifndef COMPORTTEST_H
#define COMPORTTEST_H

#include <QWidget>

namespace Ui {
class COMPortTest;
}

class COMPortTest : public QWidget
{
    Q_OBJECT

public:
    explicit COMPortTest(QWidget *parent = nullptr);
    ~COMPortTest();

protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
    void registerGUID(); // 鼠标、键盘
public slots:
    void on_comStatus(QString name, bool flag); // 端口号
    void on_USBStatus(QString name);  // 鼠标、键盘

private:
    Ui::COMPortTest *ui;
};

#endif // COMPORTTEST_H
