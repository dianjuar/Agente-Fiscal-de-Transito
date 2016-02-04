#ifndef AGENTMANAGER_H
#define AGENTMANAGER_H

#include "headers/qt_stuff.h"
#include "headers/rvo_stuff.h"
#include "headers/sfml_stuff.h"
#include "Agentes/agent.h"
#include "network/connection_sma.h"

namespace agents
{

class agentManager
{
    network::connections::SMA *connection_sma;

public:
    explicit agentManager(network::connections::SMA *connection_sma);

    std::vector<agent*> agentes;
    void addAgent( agent *a );
};

}

#endif // AGENTMANAGER_H
