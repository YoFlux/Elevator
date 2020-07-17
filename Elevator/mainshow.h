#ifndef MAINSHOW_H
#define MAINSHOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include "building.h"
#include "elevator.h"
#include "floor.h"

namespace Ui {
class MainShow;
}

class MainShow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainShow(QWidget *parent = nullptr);
    ~MainShow();
    void init();

public slots:
    void waitTime();
    void moveTime();
    void createTime();
    void personLabelMove();
    void judge();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainShow *ui;

    QTimer *waitTimer;
    QTimer *moveTimer;
    QTimer *createTimer;
    QTimer *personMoveTimer;

    QTime *randTime;

    Building *building;
    Elevator *elevator;
    Floor *floor[10];

    QLabel *movingPerson;
    QRect movingFromRect;
    QRect movingToRect;

    //是否要等待开始
    bool elevator_wait;
    //倍速模拟
    int speed;
};

#endif // MAINSHOW_H
