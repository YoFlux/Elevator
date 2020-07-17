#include "mainshow.h"
#include "ui_mainshow.h"
#include <QDebug>

MainShow::MainShow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainShow)
{
    ui->setupUi(this);
    this->setWindowTitle("电梯模拟");
    this->setWindowIcon(QIcon(":/new/prefix1/imgs/icon.png"));
    //各控件初始化
    this->init();
}

MainShow::~MainShow()
{
    delete ui;
    //释放内存
    delete elevator;
    for(int i=0;i<10;i++)delete floor[i];
}
//初始化
void MainShow::init(){
    //初始化建筑类
    this->building = new Building(ui->building->x(),ui->building->y(),ui->building->width(),ui->building->height());
    this->building->label = ui->building;
    //初始化电梯
    this->elevator = new Elevator(ui->elevator->x(),ui->elevator->y(),ui->elevator->width(),ui->elevator->height());
    this->elevator->label = ui->elevator;
    ui->elevator->setNum(this->elevator->floorNumber);
    //初始化楼的每一层，一共10层
    for(int i=0;i<10;i++){
        this->floor[i] = new Floor(10+this->elevator->width,i*this->elevator->height+3,363,this->elevator->height-3);
        this->floor[i]->label = new QLabel(this);
        this->floor[i]->label->setGeometry(this->floor[i]->getRect());
        this->floor[i]->floorNum = 10-i;
        this->floor[i]->label->setNum(10-i);
        //设置样式
        this->floor[i]->label->setStyleSheet(QString("background:rgba(255, 255, 255, 200);"
                                                     "border-radius: 5px;"));
        this->floor[i]->label->show();
    }
    //按钮控件初始设置
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    //定时器:电梯等待，电梯移动，生成人，人移动
    this->waitTimer = new QTimer(this);
    this->moveTimer = new QTimer(this);
    this->createTimer = new QTimer(this);
    this->personMoveTimer = new QTimer(this);
    //连接槽
    connect(this->waitTimer,SIGNAL(timeout()),this,SLOT(waitTime()));
    connect(this->moveTimer,SIGNAL(timeout()),this,SLOT(moveTime()));
    connect(this->createTimer,SIGNAL(timeout()),this,SLOT(createTime()));
    connect(this->personMoveTimer,SIGNAL(timeout()),this,SLOT(personLabelMove()));
    //随机数
    this->randTime = new QTime(QTime::currentTime());
    qsrand(this->randTime->msec()+this->randTime->second()*1000);
    //设置暂停为false，即不暂停
    this->elevator_wait = false;
    this->speed = 1;
}

//随机生成人
void MainShow::createTime(){
    //随机生成人的函数
    if(this->elevator_wait)return;
    for(int i=0;i<10;i++){//遍历每一层
        if(qrand()%100+1>70){
            //找到这个楼层的空余位置，没有则不生成
            int sp = this->floor[i]->getFreeSpace();
            if(sp==-1)continue;
            //为这个人生成一个qlabel
            this->floor[i]->space[sp] = new QLabel(this);
            //随机生成这个人想去的楼层
            int toFloor = qrand()%10+1;
            while(toFloor==10-i)toFloor = qrand()%10+1;
            this->floor[i]->space[sp]->setNum(toFloor);
            //大小和样式
            this->floor[i]->space[sp]->setGeometry(this->floor[i]->getRectAtSpace(sp));
            QString str = QString("min-width:22px;min-height:22px;max-width:22px;max-height:22px;"
                                  "border-radius: 11px;"
                                  "border:1px solid black;background:");
            //颜色
            int color = qrand()%3;
            if(color==0)str = str+"red;";
            else if(color==1)str = str+"green;";
            else str = str+"blue;";
            //设置样式
            this->floor[i]->space[sp]->setStyleSheet(str);
            //qlabel显示
            this->floor[i]->space[sp]->show();
        }
    }
}

//等待，判断电梯是否需要移动
void MainShow::waitTime(){
    static int i=0;
    if(this->elevator_wait)return;
    i++;
    if(i==10){
        i=0;
        //判断是否电梯或楼层中有人
        bool exist_user = false;
        if(!this->elevator->isNull())exist_user = true;
        for(int i=0;i<10;i++)
            if(!this->floor[i]->isNull())exist_user = true;
        //没有人则继续等待
        if(!exist_user)return;
        //有人的话则停止等待计时器，调用判断函数（judge）
        this->waitTimer->stop();
        this->judge();
    }
}

//电梯的逻辑判断
void MainShow::judge(){
    //获取电梯的层数和本楼层的指针
    int floorNumber = this->elevator->floorNumber;
    Floor *nowFloor = this->floor[10-floorNumber];
    //从电梯出来，根据这个人的目的楼层和本楼层判断
    for (int i=9;i>=0;i--) {
        //如果符合出电梯的逻辑则出电梯
        if(this->elevator->space[i]!=nullptr&&this->elevator->space[i]->text().toInt()==floorNumber){
            //设置personMoveTimer的参数：movingPerson，movingFromRect和movingToRect
            this->movingPerson = this->elevator->space[i];
            this->elevator->space[i] = nullptr;
            this->movingFromRect = this->elevator->getRectAtSpace(i);
            this->movingToRect = nowFloor->getRectAtSpace(0);
            //开始移动
            this->personMoveTimer->start(5/this->speed);
            return;
        }
    }
    //进入电梯，判断逻辑是这个人要去的楼层是否是电梯的方向的楼层，如果方向不正确则不能进入电梯
    int FreeSpace = this->elevator->getFreeSpace();
    if(FreeSpace!=-1){
        //遍历该楼层所有的人，即所有qlabel
        for(int i=0;i<16;i++){
            //如果电梯没有满且符合进入的逻辑则进入电梯
            if(nowFloor->space[i]!=nullptr&&this->elevator->direct*(nowFloor->space[i]->text().toInt()-floorNumber)<0){
                //设置personMoveTimer的参数：movingPerson，movingFromRect和movingToRect
                this->movingPerson = nowFloor->space[i];
                this->elevator->space[FreeSpace] = nowFloor->space[i];
                nowFloor->space[i] = nullptr;
                this->movingFromRect = nowFloor->getRectAtSpace(i);
                this->movingToRect = this->elevator->getRectAtSpace(FreeSpace);
                //开始移动
                this->personMoveTimer->start(5/this->speed);
                return;
            }
        }
    }
    //如果没有人需要进入电梯且电梯里无人的话，则需判断电梯方向
    if(this->elevator->isNull()){
        //判断上面的楼层是否有人等待电梯
        bool up_note = false;
        for(int i=10-this->elevator->floorNumber-1;i>=0;i--){
            if(!this->floor[i]->isNull()){
                up_note = true;
                break;
            }
        }
        //判断下面的楼层是否有人等待电梯
        bool down_note = false;
        for(int i=10-this->elevator->floorNumber+1;i<=9;i++){
            if(!this->floor[i]->isNull()){
                down_note = true;
                break;
            }
        }
        //判断本层电梯是否有人在等待
        bool this_note = !nowFloor->isNull();

        //电梯无人则到等待计时器
        if(!up_note&&!down_note&&!this_note){
            this->waitTimer->start(10/this->speed);
            return;
        }

        if(this->elevator->direct==-1){//电梯上行
            //如果上面没有人等待电梯，则判断本层是否有人，有的话则电梯调换方向后再judge本层，因为这个人需要进入电梯，否则直接移动
            if(!up_note){
                this->elevator->direct = 1;
                if(this_note){
                    judge();
                    return;
                }
            }
        }else if(this->elevator->direct==1){//电梯下行
            //如果下面没有人等待电梯，则判断本层是否有人，有的话则电梯调换方向后再judge本层，否则直接移动
            if(!down_note){
                this->elevator->direct = -1;
                if(this_note){
                    judge();
                    return;
                }
            }
        }
    }
    //电梯继续移动
    /*
    int num = 0;
    for(int i=0;i<10;i++){
        if(this->elevator->space[i]!=nullptr)num++;
    }
    qDebug()<<num;
    */
    this->moveTimer->start(10/this->speed);
}

//人移动
void MainShow::personLabelMove(){
    static int i = 0;//总共移动的像素
    static int direct;//方向
    static QRect rect;//辅助qrect
    if(this->elevator_wait)return;
    //如果i==0，则说明是移动的开始，给各参数赋值
    if(i==0){
        int v = this->movingFromRect.x() - this->movingToRect.x();
        i = v>0?v:-v;
        direct = v>0?-1:1;
        rect = this->movingFromRect;
    }
    i--;
    //辅助qrect移动一个像素
    rect.setX(rect.x() + direct);
    //将辅助qrect的值传给movingPerson
    this->movingPerson->setGeometry(rect);
    //i<=0时，结束移动
    if(i<=0){
        i=0;
        if(direct==1){
            //隐藏该label
            this->movingPerson->hide();
            //释放内存
            delete this->movingPerson;
        }
        this->personMoveTimer->stop();
        this->judge();
    }
}

//电梯移动
void MainShow::moveTime(){
    static int i=0;
    if(this->elevator_wait)return;

    i++;//计数所移动的像素，用以判断电梯是否刚好移动了一层
    //电梯移动一个像素
    this->elevator->y+=this->elevator->direct;
    ui->elevator->setGeometry(this->elevator->getRect());
    //电梯里的人需要和电梯平行移动
    QRect temp;
    for(int j=0;j<10;j++){
        //遍历所有的人的label，移动一个像素
        if(this->elevator->space[j]!=nullptr){
            temp = this->elevator->space[j]->geometry();
            temp.setY(temp.y()+this->elevator->direct);
            this->elevator->space[j]->setGeometry(temp);
        }
    }
    //如果移动的像素等于电梯的高度，说明移动了一个楼层，则进行后续操作
    if(i==this->elevator->height){
        i=0;
        //电梯的记录的楼层相应的变化
        if(this->elevator->direct>0)this->elevator->floorNumber--;
        else this->elevator->floorNumber++;
        ui->elevator->setNum(this->elevator->floorNumber);
        //判断电梯是否到底层或顶层，如果是，则需要修改方向
        if(this->elevator->y<=0){
            this->elevator->direct = 1;
        }
        if(this->elevator->y>=this->building->height-this->elevator->height){
            this->elevator->direct = -1;
        }
        //this->waitTimer->start(10);
        //移动结束，开始电梯的逻辑判断
        this->moveTimer->stop();
        judge();
    }
}

void MainShow::on_pushButton_clicked()
{
    this->waitTimer->start(10/this->speed);
    this->createTimer->start(5000/this->speed);
    //开始先调用一次生成函数
    createTime();
    createTime();
    //禁用开始按钮
    ui->pushButton->setEnabled(false);
    //使用暂停和继续按钮
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
}

void MainShow::on_pushButton_2_clicked()
{
    //暂停和继续
    elevator_wait = !elevator_wait;
    if(elevator_wait)ui->pushButton_2->setText("继续模拟");
    else {
        ui->pushButton_2->setText("暂停模拟");
    }
}

void MainShow::on_pushButton_3_clicked()
{
    if(this->speed==4){
        this->speed = 1;
        ui->pushButton_3->setText("X1");
    }
    else {
        this->speed = this->speed*2;
        ui->pushButton_3->setText("X"+QString::number(this->speed));
    }
}
