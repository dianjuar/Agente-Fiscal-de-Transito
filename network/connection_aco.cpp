#include "connection_aco.h"

using namespace network::connections;

ACO::ACO(QString add):
    network::Client(add, network::connections::puertos::ACO)
{

}

void ACO::sendConnected()
{
    write("iniSMA2ACO->HowMany3_VelMax1000_VelIni999");
}

void ACO::dataAnalizer(QString msj)
{
    if( msj.contains(network::mensajes::divisor) )
    {

        QStringList msjDividido =  msj.split(network::mensajes::divisor, QString::SkipEmptyParts );

        QString encabezado = msjDividido.at(0);
        QString cuerpo = msjDividido.at(1);

        if( QString::compare(encabezado, mensajes::ACOtoMe_EnviromentInfo, Qt::CaseInsensitive) == 0 )
        {
            QString map = cuerpo.split( mensajes::divisor_2, QString::SkipEmptyParts )[0];
            float distanciaEntreCuadros = cuerpo.split( mensajes::divisor_2, QString::SkipEmptyParts )[1].toFloat();

            emit llego_datosEntornoGrafico(map,distanciaEntreCuadros);
        }
        else if( QString::compare(encabezado, mensajes::ACOtoMe_NextStep, Qt::CaseInsensitive) == 0 )
        {
            QStringList msjs = cuerpo.split(mensajes::divisor_2, QString::SkipEmptyParts);

            int ID          = msjs[0].toInt();
            int direccion   = msjs[1].toInt();
            float distancia = msjs[2].toFloat()*100;
            int newPos_X    = msjs[3].toInt();
            int newPos_Y    = msjs[4].toInt();

            emit newStep(ID,direccion,distancia,newPos_X,newPos_Y);
        }
        else if( QString::compare(encabezado, mensajes::ACOtoMe_setVelocidad, Qt::CaseInsensitive) == 0 )
        {
            emit newVelocity( cuerpo.toFloat() );
        }
    }
}

void ACO::solicitarSiguientePaso(int ID)
{
    write(network::mensajes::meToACO_SiguientePaso +
          network::mensajes::divisor + QString::number(ID));
}

