#ifndef MYWIN_H
#define MYWIN_H


/* author: blog.csdn.net/jklinux */

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class MyWin;
}

class MyWin : public QWidget
{
    Q_OBJECT

public:
    explicit MyWin(QWidget *parent = 0);
    ~MyWin();
    void paintEvent(QPaintEvent *e);

private slots:
    void on_bnt_connect_clicked();
    void disconnect_slot();
    void read_data();

private:
    Ui::MyWin *ui;
    QTcpSocket *socket;

    char *data;
    int  len; //图像大小
    int  len_rcv; //接收大小
};

#endif // MYWIN_H
