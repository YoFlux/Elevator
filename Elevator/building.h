#ifndef BUILDING_H
#define BUILDING_H
#include <QRect>
#include <QLabel>

//基本建筑类，是电梯和楼层类的基类
class Building
{
public:
    Building(int x,int y,int width,int height);
    ~Building();
    int height;
    int width;
    int x;
    int y;
    //label指针将用来指向建筑的label
    QLabel *label;
    QRect getRect();
};

#endif // BUILDING_H
