#include "agentmanager.h"

using namespace agents;

agentManager::agentManager(network::connections::SMA *sma, network::connections::ACO *aco):
                             sma(sma),
                             aco(aco)
{
    connect( sma, SIGNAL(newAgente(int,int)),
             this, SLOT(newAgente(int,int)) );

    connect( aco, SIGNAL(newStep(int,int,float,int,int)),
             this, SLOT(newStep(int,int,float,int,int)) );
}

void agentManager::newAgente(int ID, int direccion)
{
    addAgent( new agent(ID, aco, sma, direccion) );

    emit newAgenteAdded();
}

void agentManager::newStep(int ID, int direccion, float distancia, int newPos_X, int newPos_Y)
{
    ((agent*)agentes[ID-1])->newStep(direccion,
                                     distancia,
                                     sf::Vector2f(newPos_X,newPos_Y));
}

void agentManager::addAgent(agent *a)
{
    QObject::connect( a, SIGNAL(velocidadesCalculadas(int,float,float)),
                      sma, SLOT(enviarVelocidades(int,float,float)));

    agentes.push_back( a );
}

agent *agentManager::ultimoAgene()
{
    return agentes.back();
}

std::vector<agents::agent *> agentManager::getAgentesDisponibles()
{
    std::vector<agents::agent *> r;

    for(agents::agent *a : agentes)
    {
        if(a->isAvaliable())
            r.push_back(a);
    }

    return r;
}
