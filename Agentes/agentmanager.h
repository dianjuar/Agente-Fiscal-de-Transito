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
    }
}
//------prototyping-------

namespace agents
{

class agentManager: public QObject
{
    Q_OBJECT

    network::connections::SMA *connection_sma;

public slots:
    void newAgente(int ID, int direccion);

public:
    explicit agentManager(network::connections::SMA *connection_sma);

    std::vector<agent*> agentes;
    void addAgent( agent *a );
};

}

#endif // AGENTMANAGER_H
