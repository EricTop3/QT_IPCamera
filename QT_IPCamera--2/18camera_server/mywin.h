/* author: blog.csdn.net/jklinux */

#ifndef MYWIN_H
#define MYWIN_H

#include <QWidget>
#include <QCamera>
#include <QCameraViewfinder>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QTimer>

namespace Ui {
class MyWin;
}

class MyWin : public QWidget
{
    Q_OBJECT

public:
    explicit MyWin(QWidget *parent = 0);
    ~MyWin();

private slots:
    void new_client();
    void distconnect_client();
    void read_data();
    void timer_slot();

private:
    Ui::MyWin *ui;

    QCamera *cam;
    QCameraViewfinder *view;

    QTcpServer *server;
    QList<QTcpSocket *> clients;
    QTimer *timer;
};

#endif // MYWIN_H
