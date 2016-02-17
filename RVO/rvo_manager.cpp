#include "rvo_manager.h"

float RVO_Manager::TimeStep_real = 0;

/*RVO_Manager::RVO_Manager(RVO::RVOSimulator *sim):
    maxAgents(3),
    timeHorizon(5),
    timeHorizonObst(5),
    timeStep(0.2),
    maxSpeed(2.f)
{
    this->sim = sim;
}*/

/*RVO_Manager::RVO_Manager(float radius,
                         std::vector<agents::agent *> agentes,
                         std::vector<entornoGrafico::obstaculo> listaObst):
                            maxAgents(3),
                            timeHorizon(5),
                            timeHorizonObst(5),
                            timeStep(0.1),
                            maxSpeed(0.01f),
                            haveReachedTheirGoal(false)
{
    sim = new RVO::RVOSimulator();

    // Specify the global time step of the simulation.
    sim->setTimeStep(timeStep);

    // Specify the default parameters for agents that are subsequently added.
    sim->setAgentDefaults( radius*2,
                           maxAgents,
                           timeHorizon, timeHorizonObst,
                           radius,
                           maxSpeed);

    add_agentes(agentes);
    add_obstacles(listaObst);
}*/

//este es el que estoy usando
RVO_Manager::RVO_Manager(float timeStep, float maxVelocity):
    maxAgents(1),
    timeHorizon(5),
    timeHorizonObst(5),
    haveReachedTheirGoal(false)
{
    this->timeStep = timeStep;
    maxSpeed = maxVelocity;
    sim = new RVO::RVOSimulator();

    TimeStep_real = timeStep*100;
}

void RVO_Manager::add_obstacles(std::vector<entornoGrafico::obstaculo> listaObst)
{
    for(entornoGrafico::obstaculo ob: listaObst)
        sim->addObstacle( ob.getPoint_RVO() );

    sim->processObstacles();
}

void RVO_Manager::add_agentes()
{
   // int c=0;
    for(auto *a: *agentes)
  //  {
        sim->addAgent(  RVO::Vector2(a->posIni.x, a->posIni.y) );
    //    sim->setAgentPrefVelocity(c,RVO::Vector2(maxSpeed,maxAgents));

    /*    c++;
    }*/
}

void RVO_Manager::setupScenario(float radius, std::vector<agents::agent *> *agentes,
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
    add_agentes();
}

void RVO_Manager::updateVisualization(std::vector<agents::agent *> &agentes)
{
    if(!reachedGoal(agentes))
    {
        for (size_t i = 0; i < sim->getNumAgents(); ++i)
            ((agents::agent *) agentes.at(i))->calculateVelocities( sim->getAgentPosition(i),
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
        else
            agentes[i]->reachedGoal();
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

