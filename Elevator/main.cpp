#include "mainshow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainShow w;
    w.show();

    return a.exec();
}
