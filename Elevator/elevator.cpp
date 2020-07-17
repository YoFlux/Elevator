#include "elevator.h"

Elevator::Elevator(int x,int y,int width,int height):
    Building (x,y,width,height)
{
    direct = -1;
    for(int i=0;i<10;i++){
        this->space[i] = nullptr;
    }
}

bool Elevator::isNull(){
    for(int i=0;i<10;i++){
        if(this->space[i]!=nullptr)return false;
    }
    return true;
}

int Elevator::getFreeSpace(){
    for (int i=0;i<10;i++) {
        if(this->space[i]==nullptr){
            return i;
        }
    }
    return -1;
}

QRect Elevator::getRectAtSpace(int sp){
    //person的label的大小是22,22
    return QRect(this->x+sp*22,this->y+this->height-22,22,22);
}
