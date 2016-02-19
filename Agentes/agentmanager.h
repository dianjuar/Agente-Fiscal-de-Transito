#ifndef AGENTMANAGER_H
#define AGENTMANAGER_H

#include "headers/qt_stuff.h"
#include "headers/rvo_stuff.h"
#include "headers/sfml_stuff.h"
#include "Agentes/agent.h"
#include "network/connection_sma.h"

//------prototyping-------
namespace network
{
    namespace connections
    {
        class SMA;
        class ACO;
    }
}
//------prototyping-------

namespace agents
{

class agentManager: public QObject
{
    Q_OBJECT

    network::connections::SMA *sma;
    network::connections::ACO *aco;

public slots:
    void newAgente(int ID, int direccion);
    void newStep(int ID, int direccion, float distancia,
                 int newPos_X, int newPos_Y);
signals:
    void newAgenteAdded();

public:
    explicit agentManager(network::connections::SMA *sma,
                          network::connections::ACO *aco);

    std::vector<agent*> agentes;
    void addAgent( agent *a );
};

}

#endif // AGENTMANAGER_H
