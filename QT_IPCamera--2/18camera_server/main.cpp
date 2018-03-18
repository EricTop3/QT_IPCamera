/* author: blog.csdn.net/jklinux */

#include "mywin.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyWin w;
    w.show();

    return a.exec();
}
