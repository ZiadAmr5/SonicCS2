#include "mainwindow.h"

#include <QApplication>
#include <player.h>
//#include <GameEngine.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Player *p = new Player();
    MainWindow w(p);
    w.show();
    return a.exec();
}
