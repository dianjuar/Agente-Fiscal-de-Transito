    #include "agentmanager.h"

using namespace agents;

void agentManager::newAgente(int ID, int direccion)
{
    addAgent( new agent(ID, direccion) );
}

agentManager::agentManager(network::connections::SMA *connection_sma):
                             connection_sma(connection_sma)
{
    connect( connection_sma, SIGNAL(newAgente(int,int)),
             this, SLOT(newAgente(int,int)) );
}

void agentManager::addAgent(agent *a)
{
    QObject::connect( a, SIGNAL(velocidadesCalculadas(int,float,float)),
                      connection_sma, SLOT(enviarVelocidades(int,float,float)));

    agentes.push_back( a );
}
