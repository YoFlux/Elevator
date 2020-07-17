#ifndef ELEVATOR_H
#define ELEVATOR_H
#include "building.h"
#include <QLabel>
#include <QRect>

class Elevator:public Building
{
public:
    Elevator(int x,int y,int width,int height);
    int direct;
    int floorNumber=1;
    //一个电梯间容纳10个人
    QLabel *space[10];
    //判断电梯是否为空
    bool isNull();
    //获取第一个空闲的位置的编号
    int getFreeSpace();
    //获取某一个编号的相应的qrect
    QRect getRectAtSpace(int sp);
};

#endif // ELEVATOR_H
