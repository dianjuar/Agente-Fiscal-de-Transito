#ifndef CONEXION_SMA_H
#define CONEXION_SMA_H

#include "headers/qt_stuff.h"
#include "network/MultiPropuseSockets.h"
#include "network/puertos.h"
#include "network/mensajes.h"
#include "Agentes/agent.h"


class MainWindow;

namespace network
{
    namespace connections
    {
        class SMA: public ::network::ServerSimple
        {
            Q_OBJECT
        public:
            SMA();
            void dataAnalizer(QString);

        signals:
            void newAgente(int ID, int direccion);
            void correccionFinalizada(int ID);

        public slots:
            void enviarVelocidades(int ID, float Vl, float Vr);
            void sendRotation(int ID, int grados);
            void solicitarCDT(int ID, int direccion, sf::Vector2f pos);
        };
    }
}

#endif // CONEXION_SMA_H
