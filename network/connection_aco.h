#ifndef ACO_H
#define ACO_H

#include "network/MultiPropuseSockets.h"
#include "network/puertos.h"
#include "network/mensajes.h"
#include "headers/sfml_stuff.h"

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

    public slots:
        void solicitarSiguientePaso(int ID);

    signals:
        void llego_datosEntornoGrafico(QString map, float dist);
        void newStep(int ID, int direccion, float distancia,
                     int newPos_X, int newPos_Y);

    };

    }
}

#endif // ACO_H
