#ifndef RVO_H
#define RVO_H

#include "entornografico.h"
#include "Agentes/agentmanager.h"
#include "simulacion.h"

//------------prototyping------------
class simulacion;

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
    simulacion *REALsim;

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

    RVO::RVOSimulator *RVOsim;

    /// \brief RVO_Manager - constructor de la case
    /// \param aManager. Puntero del Manager de Agentes
    /// \param REALsim. Punto de la simulacion real
    /// \param timeStep. Tiempo de la simulacion. En centecimas de segundo
    /// \param velocity. Velocidad en cm/s
    //
    RVO_Manager(agents::agentManager  *aManager,
                simulacion *REALsim,
                float timeStep = 5.f , float velocity=10.f);

    void add_obstacles( std::vector<entornoGrafico::obstaculo> listaObst );

    void setupScenario(float radius, std::vector<entornoGrafico::obstaculo> listaObst);

    void updateVisualization();
    void setPreferredVelocities(std::vector<agents::agent *> agentesDisponibles);

    ///
    /// \brief getTiempoSimulacion - tiempo de la simulacion RVO
    /// \return tiempo de ejecución de la sumilacion en segundos
    ///
    int getTiempoSimulacion();
};

#endif // RVO_H
