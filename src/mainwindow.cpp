#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include<level.h>

MainWindow::MainWindow(Player*p,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,view(nullptr),loop(nullptr),wp(p)
{
    ui->setupUi(this);
     level* l = new level();
    view= new gameView(wp,l,this);
    this->setCentralWidget(view);
    loop=new gameLoop(view,wp);

    view->show();




}

MainWindow::~MainWindow()
{
    delete ui;
    
}
