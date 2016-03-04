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

    if( QString::compare(encabezado, network::mensajes::SMAtoMe_ExistenseNotification, Qt::CaseInsensitive)==0 )
    {
        int ID = cuerpo.split( network::mensajes::divisor_2, QString::SkipEmptyParts ).at(0).toInt();
        int direccion = cuerpo.split( network::mensajes::divisor_2, QString::SkipEmptyParts ).at(1).toInt();

        emit newAgente(ID, direccion);
    }
    else if(QString::compare(encabezado, network::mensajes::SMAtoMe_CDT_Terminada, Qt::CaseInsensitive)==0 )
    {
        int ID = cuerpo.split( network::mensajes::divisor_2, QString::SkipEmptyParts ).at(0).toInt();

        emit correccionFinalizada( ID );
    }
}

void SMA::enviarVelocidades(int ID, float Vl, float Vr)
{
    write( mensajes::MeToSMA_Velocidades + mensajes::divisor +
           QString::number(ID) + mensajes::divisor_2 +
           QString::number(Vl) + mensajes::divisor_2 + QString::number(Vr)+ "\n" );
}

void SMA::sendRotation(int ID, int grados)
{
    write( mensajes::MeToSMA_Rotation + mensajes::divisor +
           QString::number(ID) + mensajes::divisor_2 + QString::number(grados) +"\n");
}

void SMA::solicitarCDT(int ID, int direccion, sf::Vector2f pos)
{
    int x = (int)pos.x;
    int y = (int)pos.y;

    write( mensajes::MeToSMA_CDT + mensajes::divisor +
           QString::number( ID ) + mensajes::divisor_2 +
           QString::number( direccion ) + mensajes::divisor_2 +
           QString::number( x ) + mensajes::divisor_2 +
           QString::number( y )+"\n" );
}
