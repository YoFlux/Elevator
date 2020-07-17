#include "mainshow.h"
#include <QApplication>

/*
 * 作者：莫一流
 * 学号：20182131047
 * 题目：41-随机电梯使用场景模拟
 */



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainShow w;
    w.show();

    return a.exec();
}
