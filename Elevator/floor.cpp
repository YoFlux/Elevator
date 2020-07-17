#include "floor.h"

Floor::Floor(int x, int y, int width, int height):
    Building (x,y,width,height)
{
    for(int i=0;i<16;i++){
        this->space[i] = nullptr;
    }
}

bool Floor::isNull(){
    for(int i=0;i<16;i++){
        if(this->space[i]!=nullptr)
            return false;
    }
    return true;
}

int Floor::getFreeSpace(){
    for (int i=0;i<16;i++) {
        if(this->space[i]==nullptr){
            return i;
        }
    }
    return -1;
}

QRect Floor::getRectAtSpace(int sp){
    //person的label的大小是22,22
    return QRect(this->x+sp*22,this->y+this->height-22,22,22);
}
