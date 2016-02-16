#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    C_ACO = new network::connections::ACO( "127.0.0.1" );
    C_SMA = new network::connections::SMA();



    //Create a SFML view inside the main frame
    simulacion* sm = new simulacion(get_container(), QPoint(0, 0), get_container()->size(),
                                      C_SMA);
    //sm->show();

    waitingDialog *w = new waitingDialog(parent,C_ACO,C_SMA,sm);
    w->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QWidget *MainWindow::get_container()
{
    return ui->container;
}

void MainWindow::on_pushButton_clicked()
{

}
