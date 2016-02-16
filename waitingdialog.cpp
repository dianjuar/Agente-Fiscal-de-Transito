#include "waitingdialog.h"
#include "ui_waitingdialog.h"

waitingDialog::waitingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::waitingDialog),
    requirementsReady(0)
{
    ui->setupUi(this);

    C_ACO = new network::connections::ACO(ui->label_estadoConexionACO->text());
    C_SMA = new network::connections::SMA();

    connect(C_SMA, SIGNAL(newIncomingConnection()),
            this, SLOT(newRequirementReady()) );

    connect(C_ACO, SIGNAL(llego_datosEntornoGrafico(QString,float)),
            this, SLOT(get_datosEntornoGrafico(QString,float)) );
}

void waitingDialog::newRequirementReady(int nReq)
{
    requirementsReady++;

    switch (nReq)
    {
        case Req_SMA:
            ui->label_estadoConexionSMA->setText("Conectado!");
        break;

        case Req_InfEnv:
            ui->label_estadoInformacionGrafica->setText("Conectado!");
        break;
    }

    if(requirementsReady == 2)
    {
        MainWindow *w = new MainWindow(C_SMA,C_ACO,
                                       map,dist);
        w->show();

        this->setVisible(false);
    }
}

void waitingDialog::get_datosEntornoGrafico(QString map, float dist)
{
    this->map = map;
    this->dist = dist;

    newRequirementReady(Req_InfEnv);
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
