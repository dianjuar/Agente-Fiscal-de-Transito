#include "rvo_manager.h"

float RVO_Manager::TimeStep_real = 0;

//este es el que estoy usando
RVO_Manager::RVO_Manager(std::vector<agents::agent *> *agentes,
                         float timeStep, float velocity):
    agentes(agentes),
    maxAgents(1),
    timeHorizon(5),
    timeHorizonObst(5)
{
    this->timeStep = timeStep;
    maxSpeed = (velocity*2.44)/100.f;
    sim = new RVO::RVOSimulator();

    TimeStep_real = timeStep*100;
}

void RVO_Manager::add_obstacles(std::vector<entornoGrafico::obstaculo> listaObst)
{
    for(entornoGrafico::obstaculo ob: listaObst)
        sim->addObstacle( ob.getPoint_RVO() );

    sim->processObstacles();
}

void RVO_Manager::add_UltimoAgente()
{
    agents::agent *ultimoA =  agentes->back();
    sim->addAgent(  RVO::Vector2(ultimoA->posIni.x, ultimoA->posIni.y) );

}

void RVO_Manager::setVelocidad(float v)
{
    for(int i=0; i < agentes->size(); i++ )
        sim->setAgentMaxSpeed(i, (v*2.44)/100.f);
}

void RVO_Manager::setupScenario(float radius,
                                std::vector<entornoGrafico::obstaculo> listaObst)
{
    this->agentes = agentes;

    /* Specify the global time step of the simulation. */
    sim->setTimeStep(timeStep);

    /* Specify the default parameters for agents that are subsequently added. */
    sim->setAgentDefaults( radius*(750/100),
                           maxAgents,
                           timeHorizon*300, timeHorizonObst*300,
                           radius,
                           maxSpeed);

    add_obstacles(listaObst);
}

void RVO_Manager::updateVisualization()
{
    reachedGoal();

    for (size_t i = 0; i < sim->getNumAgents(); ++i)
        ((agents::agent *) agentes->at(i))->calculateVelocities( sim->getAgentPosition(i),
                                                                 sim->getAgentVelocity(i),
                                                                 sim->getTimeStep());
    setPreferredVelocities();

    sim->doStep();
}

void RVO_Manager::reachedGoal()
{
    /* Check if all agents have reached their goals. */

    for (size_t i = 0; i < sim->getNumAgents(); ++i)
    {
        agents::agent *a = agentes->at(i);

        if (RVO::absSq(sim->getAgentPosition(i) - a->get_goal_RVO()) < 4.0f &&
            !a->isSended() )
                a->solicitar_NewStep();
    }
}

void RVO_Manager::setPreferredVelocities()
{
    for (int i = 0; i < static_cast<int>(sim->getNumAgents()); ++i)
    {
        RVO::Vector2 goal = agentes->at(i)->get_goal_RVO();

        if(goal == RVO::Vector2(-1,-1) )
            continue;

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
