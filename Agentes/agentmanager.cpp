    #include "agentmanager.h"

using namespace agents;

void agentManager::newAgente(int ID, int direccion)
{
    addAgent( new agent(ID, aco, direccion) );
}

void agentManager::newStep(int ID, int direccion, float distancia, int newPos_X, int newPos_Y)
{
    ((agent*)agentes[ID-1])->newStep(direccion,
                                     distancia,
                                     sf::Vector2f(newPos_X,newPos_Y));
}

agentManager::agentManager(network::connections::SMA *sma, network::connections::ACO *aco):
                             sma(sma),
                             aco(aco)
{
    connect( sma, SIGNAL(newAgente(int,int)),
             this, SLOT(newAgente(int,int)) );
}

void agentManager::addAgent(agent *a)
{
    QObject::connect( a, SIGNAL(velocidadesCalculadas(int,float,float)),
                      sma, SLOT(enviarVelocidades(int,float,float)));

    agentes.push_back( a );
}
