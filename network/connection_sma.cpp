#include "connection_sma.h"

using namespace network::connections;

SMA::SMA():
    network::ServerSimple(QHostAddress::Any, puertos::Recibe_SMA )
{
}

void SMA::dataAnalizer(QString msj)
{
    QStringList msjDividido =  msj.split(network::mensajes::divisor, QString::SkipEmptyParts );

    QString encabezado = msjDividido.at(0);
    QString cuerpo = msjDividido.at(1);

    if( QString::compare(encabezado, network::mensajes::SMAtoMe_ExistenseNotification, Qt::CaseInsensitive) )
    {
        int ID = cuerpo.split( network::mensajes::divisor_2, QString::SkipEmptyParts ).at(0).toInt();
        int direccion = cuerpo.split( network::mensajes::divisor_2, QString::SkipEmptyParts ).at(1).toInt();

        emit newAgente(ID, direccion);
    }
}

void SMA::enviarVelocidades(int ID, float Vl, float Vr)
{
    write( mensajes::MeToSMA_Velocidades + mensajes::divisor +
           QString::number(ID) + mensajes::divisor_2 +
           QString::number(Vl) + mensajes::divisor_2 + QString::number(Vr)+ "\n" );
}

