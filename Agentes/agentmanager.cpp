#include "agentmanager.h"

using namespace agents;

agentManager::agentManager(network::connections::SMA *sma, network::connections::ACO *aco):
                             sma(sma),
                             aco(aco)
{
    connect( sma, SIGNAL(newAgente(int,int)),
             this, SLOT(newAgente(int,int)) );

    connect( sma, SIGNAL(correccionFinalizada(int)),
             this, SLOT(correccionFinalizada(int)) );

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

void agentManager::correccionFinalizada(int ID)
{
    ((agent*)agentes[ID-1])->correccionFinalizada();
}

void agentManager::addAgent(agent *a)
{
    QObject::connect( a, SIGNAL(velocidadesCalculadas(int,float,float)),
                      sma, SLOT(enviarVelocidades(int,float,float)));

    agentes.push_back( a );
}

int agentManager::size()
{
    return agentes.size();
}

std::vector<agent *> agentManager::getAgentesDisponibles()
{
    std::vector< agent * > aDis;

    for(agent*  a: agentes )
        if(a->isAvaliable())
            aDis.push_back(a);

    return aDis;
}

agent *agentManager::ultimoAgene()
{
    return agentes.back();
}

std::vector<agent *> agentManager::getAgentes()
{
    return agentes;
}
