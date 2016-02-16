#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "simulacion.h"
#include "network/connection_aco.h"
#include "network/connection_sma.h"
#include "waitingdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QWidget* get_container();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    ::network::connections::ACO *C_ACO;
    ::network::connections::SMA *C_SMA;
};

#endif // MAINWINDOW_H
