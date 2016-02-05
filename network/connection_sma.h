#ifndef CONEXION_SMA_H
#define CONEXION_SMA_H

#include "headers/qt_stuff.h"
#include "network/MultiPropuseSockets.h"
#include "network/puertos.h"
#include "network/mensajes.h"

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

        public slots:
            void enviarVelocidades(int ID, float Vl, float Vr);
        };
    }
}

#endif // CONEXION_SMA_H