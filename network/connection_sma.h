#ifndef CONEXION_SMA_H
#define CONEXION_SMA_H

#include "network/MultiPropuseSockets.h"
#include "network/puertos.h"
#include "network/mensajes.h"

namespace network
{
    namespace connections
    {
        class SMA:public ::network::ServerSimple
        {

        public:
            SMA();

            void dataAnalizer(QString);
            void enviarVelocidades(float Vl, float Vr);

        };
    }
}

#endif // CONEXION_SMA_H
