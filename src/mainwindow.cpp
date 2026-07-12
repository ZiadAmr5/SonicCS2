#include "mainwindow.h"
#include "src/ui_mainwindow.h"
#include<level.h>

MainWindow::MainWindow(Player*p,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,view(nullptr),loop(nullptr),wp(p)
{
    ui->setupUi(this);
     level* l = new level();
    view= new gameView(wp,l,this);
    loop=new gameLoop(view,wp,l);
    this->setCentralWidget(view);

    view->show();




}

MainWindow::~MainWindow()
{
    delete ui;
    
}
