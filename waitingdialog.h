#ifndef WAITINGDIALOG_H
#define WAITINGDIALOG_H

#include <QDialog>
#include "network/connection_aco.h"
#include "network/connection_sma.h"
#include "simulacion.h"

namespace Ui {
class waitingDialog;
}

class waitingDialog : public QDialog
{
    Q_OBJECT

    ::network::connections::ACO *C_ACO;
    ::network::connections::SMA *C_SMA;

    int requirementsReady;

    Ui::waitingDialog *ui;

public slots:
    void newRequirementReady();

public:
    explicit waitingDialog(QWidget *parent = 0,
                           ::network::connections::ACO *C_ACO = NULL,
                           ::network::connections::SMA *C_SMA = NULL,
                           simulacion *sm = NULL);
    ~waitingDialog();

public slots:
    void on_pushButton_clicked();
    void connectionWithSMA();

};

#endif // WAITINGDIALOG_H
