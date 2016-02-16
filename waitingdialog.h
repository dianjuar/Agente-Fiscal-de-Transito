#ifndef WAITINGDIALOG_H
#define WAITINGDIALOG_H

#include <QDialog>
#include "network/connection_aco.h"
#include "network/connection_sma.h"
#include "simulacion.h"
#include "mainwindow.h"



namespace Ui {
class waitingDialog;
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

class waitingDialog : public QDialog
{
    Q_OBJECT

    ::network::connections::ACO *C_ACO;
    ::network::connections::SMA *C_SMA;

    int requirementsReady;

    QString map;
    float dist;

    Ui::waitingDialog *ui;

public:
    explicit waitingDialog(QWidget *parent = 0);
    ~waitingDialog();

    enum
    {
        Req_SMA     = (int) 0,
        Req_InfEnv  = (int) 1
    };

public slots:
    void on_pushButton_clicked();
    void newRequirementReady(int nReq=Req_SMA);
    void get_datosEntornoGrafico(QString map, float dist);

};

#endif // WAITINGDIALOG_H
