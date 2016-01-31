#include "rvo_manager.h"
RVO_Manager::RVO_Manager(RVO::RVOSimulator *sim):
    maxAgents(3),
    timeHorizon(5),
    timeHorizonObst(5),
    globalTime(0.2),
    maxSpeed(2.f)
{
    this->sim = sim;
}

RVO_Manager::RVO_Manager(float radius,
                         std::vector<agents::agent *> agentes,
                         std::vector<entornoGrafico::obstaculo> listaObst):
                            maxAgents(3),
                            timeHorizon(5),
                            timeHorizonObst(5),
                            globalTime(0.1),
                            maxSpeed(0.01f),
                            haveReachedTheirGoal(false)
{
    sim = new RVO::RVOSimulator();

    /* Specify the global time step of the simulation. */
    sim->setTimeStep(globalTime);

    /* Specify the default parameters for agents that are subsequently added. */
    sim->setAgentDefaults( radius*2,
                           maxAgents,
                           timeHorizon, timeHorizonObst,
                           radius,
                           maxSpeed);

    add_agentes(agentes);
    add_obstacles(listaObst);
}


//este es el que estoy usando
RVO_Manager::RVO_Manager(float globalTime, float maxVelocity):
    maxAgents(1),
    timeHorizon(5),
    timeHorizonObst(5),
    haveReachedTheirGoal(false)
{
    this->globalTime = globalTime;
    maxSpeed = maxVelocity;
    sim = new RVO::RVOSimulator();
}

void RVO_Manager::add_obstacles(std::vector<entornoGrafico::obstaculo> listaObst)
{
    for(entornoGrafico::obstaculo ob: listaObst)
        sim->addObstacle( ob.getPoint_RVO() );

    sim->processObstacles();
}

void RVO_Manager::add_agentes(std::vector<agents::agent *> agentes)
{
    for(auto *a: agentes)
        sim->addAgent(  RVO::Vector2(a->posIni.x, a->posIni.y) );
}

void RVO_Manager::setupScenario(float radius, std::vector<agents::agent *> agentes,
                                std::vector<entornoGrafico::obstaculo> listaObst)
{
    /* Specify the global time step of the simulation. */
    sim->setTimeStep(globalTime);

    /* Specify the default parameters for agents that are subsequently added. */
    sim->setAgentDefaults( radius*(750/100),
                           maxAgents,
                           timeHorizon, timeHorizonObst,
                           radius,
                           maxSpeed);

    add_obstacles(listaObst);
    add_agentes(agentes);
}

void RVO_Manager::updateVisualization(std::vector<agents::agent *> &agentes)
{
    if(!reachedGoal(agentes))
    {
        //if(printInformation) qDebug()<< "Agentes **Inicio**-------------";


        //if(printInformation) qDebug()<< "Agentes **Fin**-------------" ;

        for (size_t i = 0; i < sim->getNumAgents(); ++i)
            agentes.at(i)->calculateVelocities( sim->getAgentPosition(i),
                                                sim->getAgentVelocity(i),
                                                sim->getTimeStep());

        setPreferredVelocities(agentes);

        sim->doStep();
    }
}

bool RVO_Manager::reachedGoal(std::vector<agents::agent *> &agentes)
{
    /* Check if all agents have reached their goals. */

    for (size_t i = 0; i < sim->getNumAgents(); ++i)
    {
        //qDebug()<<RVO::absSq(sim->getAgentPosition(i) - agentes[i]->get_goal_RVO())<<"*-*-*-*-";

        if (RVO::absSq(sim->getAgentPosition(i) - agentes[i]->get_goal_RVO()) > 4.0f)
            return false;
    }

    haveReachedTheirGoal = true;
    //emit agentsHaveReachedTheirGoal();
    return true;
}

void RVO_Manager::setPreferredVelocities(std::vector<agents::agent *> agentes)
{
    for (int i = 0; i < static_cast<int>(sim->getNumAgents()); ++i)
    {
        RVO::Vector2 goal = agentes.at(i)->get_goal_RVO();

        RVO::Vector2 goalVector = goal - sim->getAgentPosition(i);

        if (RVO::absSq(goalVector) > 1.0f) {
            goalVector = RVO::normalize(goalVector);
        }

        sim->setAgentPrefVelocity(i, goalVector);

        //Perturb a little to avoid deadlocks due to perfect symmetry.
        float angle = std::rand() * 2.0f * M_PI / RAND_MAX;
        float dist = std::rand() * 0.0001f / RAND_MAX;

        sim->setAgentPrefVelocity(i, sim->getAgentPrefVelocity(i) +
                                  dist * RVO::Vector2(std::cos(angle), std::sin(angle)));
    }

}

