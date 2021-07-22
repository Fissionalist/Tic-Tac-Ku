#include "playtogether.h"
#include "ui_playtogether.h"
#include <QtDebug>
#include <QPushButton>
#include <QMessageBox>
#include <QString>
playTogether::playTogether(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::playTogether)
{
    ui->setupUi(this);
    nexTurn = 1;
    order = -1;

    connect(this, SIGNAL(gameOver()), this, SLOT(showWinner()));
}

playTogether::~playTogether()
{
    delete ui;
}

void playTogether::setBackGround()
{
    // 设置窗口大小
    const int w = 800,  h = 500;
    const int x = (QApplication::desktop()->width() - w)/2;
    const int y = (QApplication::desktop()->height() - h)/2;
    this->setGeometry(x, y, w, h);

    this->setMaximumSize(QSize(w, h));
    this->setMinimumSize(QSize(w, h));
    //this->hide();
}

void playTogether::checkGameOver()
{
    bool allFull = true;
    for(int i = 0; i<9; i++)
        if(!fullHouse[i])
            allFull = false;
    if(allFull)
        emit gameOver();
}

int playTogether::getScore(int turn)
{
    int score = 0;
    for(int i = 0; i<9; i++)
        if(fullHouse[i] == turn)
            score++;
    return score;
}

void playTogether::showWinner()
{
    int X_score = getScore(1);
    int O_score = getScore(2);
    int result = 0;

    if(X_score > O_score)
        result = QMessageBox::information(this,"结果", "X 获胜！\n再来一局？",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
    else
        result = QMessageBox::information(this,"结果", "O 获胜！\n再来一局？",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);

    if(result == QMessageBox::Yes)
        on_restartGame_clicked();
    else if(result == QMessageBox::No)
        close();
}

void playTogether::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    QRect plot;
    //高亮处
    if(order != -1){//条件约束
        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                plot.setRect(50 + 150 * i, 20 + 150 * j, 150, 150);
                painter.fillRect(plot, Qt::gray);
            }
        }
        plot.setRect(50 + 150 * (order%3), 20 + 150 * (order/3), 150, 150);
        painter.fillRect(plot, QColor("cornsilk"));
    }
    else{//空约束
        for(int i=0; i<9;i++){
            plot.setRect(50+(i%3)*150, 20+(i/3)*150, 150, 150);
            if(!fullHouse[i])
                painter.fillRect(plot, QColor("cornsilk"));
            else
                painter.fillRect(plot, Qt::gray);
        }
    }

    QPen thinPen, thickPen;
    thickPen.setWidth(5);
    thickPen.setCapStyle(Qt::RoundCap);

    painter.setPen(thinPen);//画细线
    for(int i = 1; i<9; i++){
        painter.drawLine(50, 20+50*i, 500, 20+50*i);
        painter.drawLine(50+50*i, 20, 50+50*i, 470);
    }
    painter.setPen(thickPen);//画粗线
    for(int i =0; i<2; i++){
        painter.drawLine(52, 170+i*150, 498, 170+i*150);
        painter.drawLine(200+i*150, 22, 200+i*150, 468);
    }

    //画棋子
    QPixmap p_x, p_o;
    p_x.load(":/chess/imgs/x.png");
    p_o.load(":/chess/imgs/o.png");
    for(int i = 0; i<9; i++){
        if(fullHouse[i] == 1 || fullHouse[i] == 2){//
            plot.setRect(52+(i%3)*150, 22+(i/3)*150, 146, 146);
            painter.fillRect(plot, Qt::gray);
            if(fullHouse[i] == 1)
                painter.drawPixmap(54+(i%3)*150, 24+(i/3)*150, 142, 142, p_x);
            else if(fullHouse[i] == 2)
                painter.drawPixmap(54+(i%3)*150, 24+(i/3)*150, 142, 142, p_o);
        }
        else{//未满座
            for(int j = 0; j<9; j++){
                if(chessBoard[i][j] == 1)
                    painter.drawPixmap(52+(i%3)*150+(j%3)*50, 22+(i/3)*150+(j/3)*50, 46, 46, p_x);
                else if(chessBoard[i][j] == 2)
                    painter.drawPixmap(52+(i%3)*150+(j%3)*50, 22+(i/3)*150+(j/3)*50, 46, 46, p_o);
            }
        }
    }
    if(nexTurn == 2)//提示当前玩家是谁
        ui->nextTurn->setPixmap(p_o.scaled(ui->nextTurn->width(), ui->nextTurn->height(), Qt::KeepAspectRatio));
    else
        ui->nextTurn->setPixmap(p_x.scaled(ui->nextTurn->width(), ui->nextTurn->height(), Qt::KeepAspectRatio));

    ui->xscorelabel->setText(QString::number(getScore(1)));
    ui->oscorelabel->setText(QString::number(getScore(2)));
}

void playTogether::mouseReleaseEvent(QMouseEvent *event){
     qDebug()<<event->x()<<event->y();
     if(event->x()<50||event->x()>500||event->y()<20||event->y()>470)//点击坐标不在棋盘内，直接无视
         return;
     int i = (event->x()-50)/150 + 3 * ((event->y()-20)/150);
     int j = ((event->x()-(i%3)*150-50)/50) + 3 * ((event->y()-(i/3)*150-20)/50);
     qDebug()<<i<<j;

     if(order == -1){//标记值，空约束
         if(fullHouse[i] != 0 || chessBoard[i][j] != 0)
             return;
         else
             chessBoard[i][j] = nexTurn;
     }
     else{//非标记值，条件约束
         if(fullHouse[i] != 0 || chessBoard[i][j] != 0 || i != order)
             return;
         else
             chessBoard[i][j] = nexTurn;
     }
     updateGameState(i, j);
     checkGameOver();
     qDebug()<<order;
}


void playTogether::on_restartGame_clicked()
{
    nexTurn = 1;
    order = -1;
    for(int i=0; i<9; i++)
        fullHouse[i] = 0;
    for(int i = 0; i<9; i++)
        for(int j = 0; j<9; j++)
            chessBoard[i][j] = 0;
    update();
}

void playTogether::updateGameState(int i, int j)
{

    bool nextFull = true, nowFull = true;

    //判断下一区域和当前区域是否满座
    for(int k = 0; k < 9; k++){
        if(nextFull && !chessBoard[j][k])
            nextFull = false;
        if(nowFull && !chessBoard[i][k])
            nowFull = false;
    }

    bool occupied = false;
    //判断当前落子区域是否被占领
    switch (j) {
    case 0:{
        if((chessBoard[i][1] == nexTurn && chessBoard[i][2] == nexTurn)||(chessBoard[i][3] == nexTurn && chessBoard[i][6] == nexTurn)
                ||(chessBoard[i][4] == nexTurn && chessBoard[i][8] == nexTurn))
            occupied = true;
        break;
    }
    case 1:{
        if((chessBoard[i][0] == nexTurn && chessBoard[i][2] == nexTurn)||(chessBoard[i][4] == nexTurn && chessBoard[i][7] == nexTurn))
            occupied = true;
        break;
    }
    case 2:{
        if((chessBoard[i][0] == nexTurn && chessBoard[i][1] == nexTurn)||(chessBoard[i][5] == nexTurn && chessBoard[i][8] == nexTurn)
                ||(chessBoard[i][4] == nexTurn && chessBoard[i][6] == nexTurn))
            occupied = true;
        break;
    }
    case 3:{
        if((chessBoard[i][4] == nexTurn && chessBoard[i][5] == nexTurn)||(chessBoard[i][0] == nexTurn && chessBoard[i][6] == nexTurn))
            occupied = true;
        break;
    }
    case 4:{
        for(int p = 1; p<5; p++)
            if(chessBoard[i][4-p] == nexTurn && chessBoard[i][4+p] == nexTurn)
                occupied = true;
        break;
    }
    case 5:{
        if((chessBoard[i][3] == nexTurn && chessBoard[i][4] == nexTurn)||(chessBoard[i][2] == nexTurn && chessBoard[i][8] == nexTurn))
            occupied = true;
        break;
    }
    case 6:{
        if((chessBoard[i][0] == nexTurn && chessBoard[i][3] == nexTurn)||(chessBoard[i][7] == nexTurn && chessBoard[i][8] == nexTurn)
                ||(chessBoard[i][2] == nexTurn && chessBoard[i][4] == nexTurn))
            occupied = true;
        break;
    }
    case 7:{
        if((chessBoard[i][1] == nexTurn && chessBoard[i][4] == nexTurn)||(chessBoard[i][6] == nexTurn && chessBoard[i][8] == nexTurn))
            occupied = true;
        break;
    }
    case 8:{
        if((chessBoard[i][2] == nexTurn && chessBoard[i][5] == nexTurn)||(chessBoard[i][6] == nexTurn && chessBoard[i][7] == nexTurn)
                ||(chessBoard[i][0] == nexTurn && chessBoard[i][4] == nexTurn))
            occupied = true;
        break;
    }
    }

//    //更新fullHouse数组
//    if(occupied)
//        fullHouse[i] = nexTurn;
//    else if(nowFull){//此区域满座但未被占领，比较双方在此区域的棋子数量，棋子多者占领此区域
//        int now_num = 0, next_num = 0;
//        for(int k = 0; k<9; k++){
//            if(chessBoard[i][k] == nexTurn)
//                now_num++;
//            else if(chessBoard[i][k] == 3-nexTurn)
//                next_num++;
//        }
//        fullHouse[i] = now_num > next_num ? nexTurn : 3-nexTurn;
//    }
//    else;//未满座也未被占领，什么也不做

    //v2.0平衡性调整
    if(occupied)
        fullHouse[i] = nexTurn;
    else if(nowFull)
        fullHouse[i] = 3;
    else;

    //更新order，限定下一次的区域
    if(fullHouse[j])
        order = -1;
    else
        order = j;
    nexTurn = 3 - nexTurn;
    update();
}
