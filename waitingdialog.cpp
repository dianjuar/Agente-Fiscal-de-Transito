#include "waitingdialog.h"
#include "ui_waitingdialog.h"

waitingDialog::waitingDialog(QWidget *parent,
                             network::connections::ACO *C_ACO,
                             network::connections::SMA *C_SMA,
                             simulacion* sm) :
    QDialog(parent),
    ui(new Ui::waitingDialog),
    C_ACO(C_ACO),
    C_SMA(C_SMA),
    requirementsReady(0)
{
    ui->setupUi(this);


    connect(C_SMA, SIGNAL(newIncomingConnection()),
            this, SLOT(newRequirementReady()) );

    connect(sm, SIGNAL(IHaveWhatINeed()),
            this, SLOT(newRequirementReady()) );

    connect(C_SMA, SIGNAL(newIncomingConnection()),
            this, SLOT(connectionWithSMA()) );
}

void waitingDialog::newRequirementReady()
{
    requirementsReady++;

    if(requirementsReady == 2)
    {

    }
}

waitingDialog::~waitingDialog()
{
    delete ui;
}

void waitingDialog::on_pushButton_clicked()
{
    C_ACO->set_host( ui->ACO_addres->text() );
    C_ACO->connectToHost();

    if(C_ACO->isConnected())
    {
        C_ACO->sendConnected();
        ui->label_estadoConexionACO->setText("Conectado!");
    }
    else
    {
        ui->label_estadoConexionACO->setText("Error!");
    }
}

void waitingDialog::connectionWithSMA()
{
    ui->label_estadoConexionSMA->setText("Conectado!");
}
