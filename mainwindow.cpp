#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(network::connections::SMA *sma,
                       network::connections::ACO *aco, QString map, float dist,
                       QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    aco(aco),
    sma(sma)
{
    ui->setupUi(this);

    //Create a SFML view inside the main frame
    simulacion* sm = new simulacion(get_container(), QPoint(0, 0), get_container()->size(),
                                    map,dist,
                                    sma,aco);
    sm->show();
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
