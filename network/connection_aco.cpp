#include "connection_aco.h"

using namespace network::connections;

ACO::ACO(QString add):
    network::Client(add, network::connections::puertos::ACO)
{

}

void ACO::sendConnected()
{
    write("iniSMA2ACO->HowMany3_VelMax33.333_VelIni15.0");
}

void ACO::dataAnalizer(QString msj)
{
    QStringList msjDividido =  msj.split(network::mensajes::divisor, QString::SkipEmptyParts );

    QString encabezado = msjDividido.at(0);
    QString cuerpo = msjDividido.at(1);

    if( QString::compare(encabezado, mensajes::ACOtoMe_EnviromentInfo, Qt::CaseInsensitive) )
    {
        QString map = cuerpo.split( mensajes::divisor_2, QString::SkipEmptyParts )[0];
        float distanciaEntreCuadros = cuerpo.split( mensajes::divisor_2, QString::SkipEmptyParts )[1].toFloat();

        emit llego_datosEntornoGrafico(map,distanciaEntreCuadros);
    }
}

