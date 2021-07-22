#include "playtogether.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PlayTogether w;
    w.show();
    return a.exec();
}
