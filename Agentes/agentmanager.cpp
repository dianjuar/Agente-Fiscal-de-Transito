    #include "agentmanager.h"

using namespace agents;

agentManager::agentManager(network::connections::SMA *connection_sma):
                             connection_sma(connection_sma)
{

}

void agentManager::addAgent(agent *a)
{
    QObject::connect( a, SIGNAL(velocidadesCalculadas(int,float,float)),
                      connection_sma, SLOT(enviarVelocidades(int,float,float)));

    agentes.push_back( a );
    }

/*void agentManager::buildDiferencialAgentsPath()
{
    for(agent *a : agentes)
        a->buildAgentPath();

    int mayorNumeroDePaths = -1;

    for(int i=0; i<agentes.size(); i++)
    {
        agent *a = agentes[i];

        for(int j=i+1; j<agentes.size(); j++)
        {
            agent *b = agentes[j];

            bool hasCollision = false;

            do
            {
                for(agentPath *p:a->getPaths())
                {
                    std::vector< agentPath* > pathInTheSameTimeWith_P = b->getPathInTheSameTime( *p );

                        for(agentPath *pathInTheSameTime:pathInTheSameTimeWith_P)
                        {
                            hasCollision = p->collideWith( *pathInTheSameTime );

                            if( hasCollision )
                            {
                                a->dividePath( p );
                                b->dividePath( pathInTheSameTime );
                            }
                        }
                }
            }
            while(hasCollision);
        }

        qDebug()<<"*********************************";
        int nCleanSteps = a->getPaths().size();
        qDebug()<<"Robot"<<i<<" Clean Steps: "<<nCleanSteps;

        if( nCleanSteps > mayorNumeroDePaths)
            mayorNumeroDePaths = nCleanSteps;
    }

    ((MyCanvas*)this->parent())->reset_currentTime();
    ((MyCanvas*)this->parent())->numeroMaximoPath = mayorNumeroDePaths;

}*/
