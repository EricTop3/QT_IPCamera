/* author: blog.csdn.net/jklinux */

#include "mywin.h"
#include <QApplication>
#include <QWidget>

#define W  360
#define H  380

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget wid;

    MyWin w(&wid), w2(&wid), w3(&wid), w4(&wid);

    w.setGeometry(0, 0, W, H);
    w2.setGeometry(W, 0, W, H);
    w3.setGeometry(0, H, W, H);
    w4.setGeometry(W, H, W, H);

    wid.show();
    return a.exec();
}
