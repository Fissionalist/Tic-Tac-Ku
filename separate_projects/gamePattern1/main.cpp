#include "playtogether.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    playTogether w;
    w.show();
    return a.exec();
}
