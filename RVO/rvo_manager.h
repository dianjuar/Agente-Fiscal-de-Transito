#ifndef RVO_H
#define RVO_H

#include "entornografico.h"
#include "Agentes/agentmanager.h"
#include "Agentes/agent.h"

//------------prototyping------------
namespace agents
{
    class agent;
    class agentManager;
}

namespace entornoGrafico
{
    class mapa;
    class obstaculo;
}
//------------prototyping------------

class RVO_Manager: public QObject
{
    Q_OBJECT

    float maxAgents;
    float timeHorizon;
    float timeHorizonObst;
    float timeStep;
    float maxSpeed;

    agents::agentManager *aManager;
    void reachedGoal(std::vector<agents::agent *> agentesDisponibles);

signals:
    void agentsHaveReachedTheirGoal();

public slots:
    void add_UltimoAgente();
    void setVelocidad(float v);

public:

    ///
    /// \brief Time step de la simulación basado en segundos.
    ///
    static float TimeStep_real;

    RVO::RVOSimulator *sim;

    ///
    /// \brief RVO_Manager constructor de la case
    /// \param agentes. Lista donde se almacenan las agentes.
    /// \param timeStep. Tiempo de la simulacion. En centecimas de segundo
    /// \param velocity. Velocidad en cm/s
    ///
    RVO_Manager(agents::agentManager *aManager,
                float timeStep = 5.f , float velocity=15.0f);

    void add_obstacles( std::vector<entornoGrafico::obstaculo> listaObst );

    void setupScenario(float radius, std::vector<entornoGrafico::obstaculo> listaObst);

    void updateVisualization();
    void setPreferredVelocities(std::vector<agents::agent *> agentesDisponibles);

    //std::vector<agents::agent *> getAgentesDisponibles();
};

#endif // RVO_H
