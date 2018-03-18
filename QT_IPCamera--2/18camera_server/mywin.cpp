/* author: blog.csdn.net/jklinux */

#include "mywin.h"
#include "ui_mywin.h"
#include <QMessageBox>
#include <QScreen>
#include <QBuffer>

#define W  640
#define H  480

#define LEN  (W*H*2)

#define P_LEN  1380

class Data {
public:
    char data[LEN]; //图像数据
    int  len; //图像大小
    int  len_sent; //已发出的数据长度
    int  stats; //工作状态, 0表示空闲(图像数据可以更新), 1表示图像数据传输中(还不可以更新图像数据), 2表示需要发出图像数据的第一部分
    Data() {
        len_sent = 0;
        stats = 0;
    }
};

MyWin::MyWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWin)
{
    ui->setupUi(this);

    server = new QTcpServer(this);
    if (!server->listen(QHostAddress::Any, 12345))
    {
        QMessageBox::critical(this, "error", "listen port failed");
        exit(0);
    }

    connect(server, SIGNAL(newConnection()), this, SLOT(new_client()));

    cam = new QCamera;
    view = new QCameraViewfinder(this);
    view->setGeometry(0, 0, W, H);
    view->show();

    cam->setViewfinder(view);
    cam->start();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timer_slot()));
    timer->start(100);

    resize(640, 600);
}

MyWin::~MyWin()
{
    delete ui;
    cam->stop();
    delete cam;
    delete view;
    delete server;
}

void MyWin::timer_slot()
{
    QString s("newImage:%1");
    QScreen *screen = QApplication::primaryScreen();
    QPixmap map = screen->grabWindow(view->winId()); //只截view窗口对象上的图像，也就是摄像头的数据

   //再编码成jpg图像
    QByteArray ba;
    QBuffer    bf(&ba);
    map.save(&bf, "jpg", 50);



    //给每个QTcpSocket对象更新图像数据
    for (int i = 0; i < clients.size(); i++)
    {
        Data *d = (Data*)clients.at(i)->userData(0);

        if (d->stats != 1) // 1表示传输中
        {
            memcpy(d->data, ba.data(), ba.size());
            d->len = ba.size();

            if (d->stats == 2)
            {
                d->stats = 1; //改为传输中的状态
                d->len_sent = 0;
                clients.at(i)->write(s.arg(d->len).toUtf8());
            }
        }
    }
}

void MyWin::read_data()
{
    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());
    QString str = client->readAll();
    Data *d = (Data*)client->userData(0);
    QString s("newImage:%1");

    if (str == "new_request")
    {
        if ((d->len) && (d->stats==0)) //图像大小不为0，表示已更新图像数据了
        {
            d->stats = 1;
            client->write(s.arg(d->len).toUtf8());
            d->len_sent = 0;
        }
        else //图像数据还没有更新
            d->stats = 2; //在定时器的槽函数里发出"newImage..."
    }
    if (str == "ack")
    {
        int len_send = P_LEN;
        if (d->len_sent >= d->len) //如果图像已传输完毕
            return;

        if ((d->len_sent + P_LEN) > d->len)
            len_send = d->len - d->len_sent;

        d->len_sent += client->write(d->data+d->len_sent, len_send);
        if (d->len_sent >= d->len)
        {
            d->stats = 0; //传输完毕后，把状态改为可更新
            d->len = 0;
        }
    }

}

void MyWin::distconnect_client()
{
    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());
    Data *d = (Data*)client->userData(0);

    clients.removeOne(client);
}

void MyWin::new_client()
{
    QTcpSocket *client = server->nextPendingConnection();

    connect(client, SIGNAL(disconnected()), this, SLOT(distconnect_client()));
    connect(client, SIGNAL(readyRead()), this, SLOT(read_data()));
    client->setUserData(0, (QObjectUserData *)new Data);
    clients.append(client);
}
