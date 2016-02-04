#include "connection_sma.h"

using namespace network::connections;

SMA::SMA():
    network::ServerSimple(QHostAddress::Any, puertos::Recibe_SMA )
{

}

void SMA::dataAnalizer(QString)
{

}

void SMA::enviarVelocidades(int ID, float Vl, float Vr)
{
    write( mensajes::Velocidades + mensajes::divisor +
           QString::number(ID) + mensajes::divisor_2 +
           QString::number(Vl) + mensajes::divisor_2 + QString::number(Vr)+ "\n" );
}
