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
    float globalTime;
    float maxSpeed;

signals:
    //void agentsHaveReachedTheirGoal();

public:
    RVO::RVOSimulator *sim;

    bool haveReachedTheirGoal;

    RVO_Manager(RVO::RVOSimulator *sim);
    RVO_Manager(float radius, std::vector<agents::agent*> agentes,
                std::vector<entornoGrafico::obstaculo> listaObst);
    RVO_Manager(float globalTime, float maxVelocity);

    void add_obstacles( std::vector<entornoGrafico::obstaculo> listaObst );
    void add_agentes(std::vector< agents::agent* > agentes );

    void setupScenario(float radius, std::vector<agents::agent*> agentes, std::vector<entornoGrafico::obstaculo> listaObst);

    void updateVisualization(std::vector<agents::agent *> &agentes);
    void setPreferredVelocities(std::vector<agents::agent*> agentes);
private:
    bool reachedGoal(std::vector<agents::agent*> &agentes);
};

#endif // RVO_H
