#include "mainwindow.h"
#include "src/ui_mainwindow.h"
#include<level.h>
#include <QMenu>
#include <QMenuBar>
#include <QAction>

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

    // --- Levels menu: jump between / restart levels ---
    QMenu* levelMenu = menuBar()->addMenu(tr("Levels"));
    for (int i = 0; i < view->levels.size(); ++i)
    {
        QAction* act = levelMenu->addAction(view->levels[i].name);
        connect(act, &QAction::triggered, this,
                [this, i]{ loop->goToLevel(i); view->setFocus(); });
    }
    levelMenu->addSeparator();
    QAction* restart = levelMenu->addAction(tr("Restart Level"));
    connect(restart, &QAction::triggered, this,
            [this]{ loop->goToLevel(view->currentLevel); view->setFocus(); });
}

MainWindow::~MainWindow()
{
    delete ui;
    
}
