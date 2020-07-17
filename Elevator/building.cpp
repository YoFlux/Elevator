#include "building.h"

Building::Building(int x, int y, int width, int height)
{
    this->height=height;
    this->width=width;
    this->x=x;
    this->y=y;
}

QRect Building::getRect(){
    return QRect(x,y,width,height);
}

Building::~Building(){
    if(label!=nullptr){
        label->hide();
        delete label;
    }
}
