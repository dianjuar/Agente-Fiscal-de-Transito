#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "simulacion.h"
#include "network/connection_aco.h"

namespace Ui {
class MainWindow;
}

//------prototiping------
class simulacion;

namespace network
{
    namespace connections
    {
        class ACO;
        class SMA;
    }
}
//------prototiping------

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(network::connections::SMA *sma,
               network::connections::ACO *aco,
               QString map,
               float dist,
               QWidget *parent = 0);

    ~MainWindow();

    QWidget* get_container();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    ::network::connections::SMA *sma;
    ::network::connections::ACO *aco;
};

#endif // MAINWINDOW_H
