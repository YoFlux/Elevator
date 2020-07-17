#ifndef FLOOR_H
#define FLOOR_H
#include "building.h"

class Floor : public Building
{
public:
    Floor(int x,int y,int width,int height);
    int floorNum;
    //一层楼最多16个人在等待
    QLabel *space[16];
    //判断楼层是否为空
    bool isNull();
    //获取第一个空闲的编号
    int getFreeSpace();
    //获取在某个编号的rect
    QRect getRectAtSpace(int sp);
};

#endif // FLOOR_H
