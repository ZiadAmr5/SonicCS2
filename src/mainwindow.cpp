#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(Player*p,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,view(nullptr),loop(nullptr),wp(p)
{
    ui->setupUi(this);
    view= new gameView(wp,this);
    this->setCentralWidget(view);
    loop=new gameLoop(view,wp);
    view->show();




}

MainWindow::~MainWindow()
{
    delete ui;
    
}
