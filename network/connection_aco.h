#ifndef ACO_H
#define ACO_H

#include "network/MultiPropuseSockets.h"
#include "network/puertos.h"
#include "network/mensajes.h"

namespace network
{
    namespace connections
    {

    class ACO: public ::network::Client
    {
        Q_OBJECT

    public:
        ACO(QString add);
        void sendConnected();

        void dataAnalizer(QString msj);

    signals:
        void llego_datosEntornoGrafico(QString map, float dist);

    };

    }
}

#endif // ACO_H
