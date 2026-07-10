#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(Player*p,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,view(nullptr)
{
    ui->setupUi(this);
    view= new gameView(p,this);
    this->setCentralWidget(view);
    view->show();




}

MainWindow::~MainWindow()
{
    delete ui;
}
