#ifndef PLAYTOGETHER_H
#define PLAYTOGETHER_H

#include <QWidget>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QFont>
#include <QBrush>
#include <QColor>
#include <QPaintDevice>
#include <QPaintEngine>
#include <QPixmap>
#include <QPainterPath>
#include <QDebug>
#include <QLCDNumber>
#include <QRect>
#include <QRectF>
#include <QPoint>
#include <QPointF>
#include <QLabel>
#include <QLine>

QT_BEGIN_NAMESPACE
namespace Ui { class playTogether; }
QT_END_NAMESPACE

class playTogether : public QWidget
{
    Q_OBJECT

public:
    playTogether(QWidget *parent = nullptr);
    ~playTogether();
    void setBackGround();
    void checkGameOver();//判断游戏是否结束
    int getScore(int turn);

public slots:
    void showWinner();

signals:
    void gameOver();

protected:
    //绘制画布和鼠标按下操作
    void paintEvent(QPaintEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

private slots:
    void on_restartGame_clicked();

private:
    Ui::playTogether *ui;
    int chessBoard[9][9] = {0};
    int fullHouse[9] = {0};
    int nexTurn;
    int order;
    void updateGameState(int i, int j);
};
#endif // PLAYTOGETHER_H
