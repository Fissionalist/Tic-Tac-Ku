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
namespace Ui { class PlayTogether; }
QT_END_NAMESPACE

class PlayTogether : public QWidget
{
    Q_OBJECT

public:
    PlayTogether(QWidget *parent = nullptr);
    ~PlayTogether();
    void setBackGround();
    void checkFullHouse();//判断棋盘是否全部满座
    int getScore(int sign);
    void restartGame();

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
    Ui::PlayTogether *ui;
    int chessBoard[9][9] = {0};
    bool changed[9][9] = {0};
    int nexTurn;
    int order;
    void changeOrder(int i);

};
#endif // PLAYTOGETHER_H
