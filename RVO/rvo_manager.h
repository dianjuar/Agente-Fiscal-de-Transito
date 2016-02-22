#ifndef RVO_H
#define RVO_H

#include "entornografico.h"
#include "Agentes/agent.h"

//------------prototyping------------
namespace agents
{
    class agent;
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

    std::vector<agents::agent *> *agentes;
    void reachedGoal();

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
    RVO_Manager(std::vector<agents::agent *> *agentes,
                float timeStep = 5.f , float velocity=15.f);

    void add_obstacles( std::vector<entornoGrafico::obstaculo> listaObst );

    void setupScenario(float radius, std::vector<entornoGrafico::obstaculo> listaObst);

    void updateVisualization();
    void setPreferredVelocities();
};

#endif // RVO_H
