#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Create a SFML view inside the main frame
    MyCanvas* SFMLView = new MyCanvas(get_container(), QPoint(0, 0), get_container()->size() );
    SFMLView->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QWidget *MainWindow::get_container()
{
    return ui->container;
}
