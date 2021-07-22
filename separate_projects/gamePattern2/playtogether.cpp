#include "playtogether.h"
#include "ui_playtogether.h"
#include <QtDebug>
#include <QPushButton>
#include <QMessageBox>
#include <QString>
PlayTogether::PlayTogether(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PlayTogether)
{
    ui->setupUi(this);
    nexTurn = 1;
    order = -1;
    setBackGround();

    connect(this, SIGNAL(gameOver()), this, SLOT(showWinner()));
}
PlayTogether::~PlayTogether()
{
    delete ui;
}

void PlayTogether::setBackGround()
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

void PlayTogether::checkFullHouse()
{
    bool fullHouse = true;
    for(int i =0;i<9; i++)
        for(int j =0; j<9; j++)
            if(chessBoard[i][j] == 0){
                fullHouse = false;
                break;
            }
    if(fullHouse)
        emit gameOver();
}

int PlayTogether::getScore(int sign)
{
    int score = 0;
    for(int i = 0; i<9; i++){
        if(chessBoard[i][1] == sign){
            if(chessBoard[i][0] == sign && chessBoard[i][2] == sign)
                score++;
        }
        if(chessBoard[i][3] == sign){
            if(chessBoard[i][0] == sign && chessBoard[i][6] == sign)
                score++;
        }
        if(chessBoard[i][5] == sign){
            if(chessBoard[i][2] == sign && chessBoard[i][8] == sign)
                score++;
        }
        if(chessBoard[i][7] == sign){
            if(chessBoard[i][6] == sign && chessBoard[i][8] == sign)
                score++;
        }
        if(chessBoard[i][4] == sign){
            for(int j = 1; j<5; j++)
                if(chessBoard[i][4-j] == sign && chessBoard[i][4+j] == sign)
                    score++;
        }
    }
    return score;
}

void PlayTogether::showWinner()
{
    int X_score = getScore(1);
    int O_score = getScore(2);
    int winner = X_score - O_score;
    int result = 0;
    if(winner == 0)
        result = QMessageBox::information(this,"结果","平分秋色！\n再来一局？",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
    else{
        QString str = winner > 0 ? "X" : "O";
        result = QMessageBox::information(this,"结果", str + "获胜！\n再来一局？",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
    }
    if(result == QMessageBox::Yes)
        on_restartGame_clicked();
    else if(result == QMessageBox::No)
        close();
}

void PlayTogether::paintEvent(QPaintEvent* event){
    Q_UNUSED(event);

    QPainter painter(this);

    QRect plot(50, 20, 450, 450);
    painter.fillRect(plot, QColor("cornsilk"));

    //高亮处
    if(order != -1){
        QRect rect;
        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                rect.setRect(50 + 150 * i, 20 + 150 * j, 150, 150);
                painter.fillRect(rect, Qt::gray);
            }
        }
        rect.setRect(50 + 150 * (order%3), 20 + 150 * (order/3), 150, 150);
        painter.fillRect(rect, QColor("cornsilk"));
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

    QPixmap p_x, p_o;
    p_x.load(":/img/imgs/x.png");
    p_o.load(":/img/imgs/o.png");
    for(int i = 0; i<9; i++){
        for(int j = 0; j<9; j++){
            if(chessBoard[i][j] == 1)
                painter.drawPixmap(52+(i%3)*150+(j%3)*50, 22+(i/3)*150+(j/3)*50, 46, 46, p_x);
            else if(chessBoard[i][j] == 2)
                painter.drawPixmap(52+(i%3)*150+(j%3)*50, 22+(i/3)*150+(j/3)*50, 46, 46, p_o);
        }
    }
    if(nexTurn == 2)//提示当前玩家是谁
        ui->nextTurn->setPixmap(p_o.scaled(ui->nextTurn->width(), ui->nextTurn->height(), Qt::KeepAspectRatio));
    else
        ui->nextTurn->setPixmap(p_x.scaled(ui->nextTurn->width(), ui->nextTurn->height(), Qt::KeepAspectRatio));
    ui->xscorelabel->setText(QString::number(getScore(1)));
    ui->oscorelabel->setText(QString::number(getScore(2)));
}

void PlayTogether::mouseReleaseEvent(QMouseEvent* event){
    qDebug()<<event->x()<<event->y();
    if(event->x()<50||event->x()>500||event->y()<20||event->y()>470)//点击坐标不在棋盘内，直接无视
        return;
    int i = (event->x()-50)/150 + 3 * ((event->y()-20)/150);
    int j = ((event->x()-(i%3)*150-50)/50) + 3 * ((event->y()-(i/3)*150-20)/50);
    qDebug()<<i<<j;
    if(order == -1){//标记值，解除约束
        //V2.0版本平衡性调整
        if(changed[i][j])//表示此位置改过，不可二次更改
            return;
        else if(chessBoard[i][j])//此位置有棋子但未被更改
            changed[i][j] = true;
        chessBoard[i][j] = nexTurn;
        nexTurn = 3 - nexTurn;
        changeOrder(j);
        update();
    }
    else{//非标记值，条件约束
        if(chessBoard[i][j] == 0 && i == order){
                chessBoard[i][j] = nexTurn;
                nexTurn = 3 - nexTurn;
                changeOrder(j);
                update();
        }
    }
    qDebug()<<order;
    checkFullHouse();
}

void PlayTogether::changeOrder(int i)
{
    bool haveFree = false;
    for(int j = 0; j < 9; j++)
        if(chessBoard[i][j] == 0){
            haveFree = true;
            break;
        }
    if(!haveFree)
        order = -1;
    else
        order = i;
}

void PlayTogether::on_restartGame_clicked()
{
    nexTurn = 1;
    order = -1;
    for(int i = 0; i<9; i++)
        for(int j = 0; j<9; j++){
            chessBoard[i][j] = 0;
            changed[i][j] = false;
        }

    update();
}
void PlayTogether::restartGame(){//未用上
    nexTurn = 1;
    order = -1;
    for(int i = 0; i<9; i++)
        for(int j = 0; j<9; j++)
            chessBoard[i][j] = 0;
    update();
}
