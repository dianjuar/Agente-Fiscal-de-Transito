#include "rvo_manager.h"

float RVO_Manager::TimeStep_real = 0;

//este es el que estoy usando
RVO_Manager::RVO_Manager(agents::agentManager *aManager,
                         float timeStep, float velocity):
    aManager(aManager),
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
    agents::agent *ultimoA =  aManager->ultimoAgene();
    sim->addAgent(  RVO::Vector2(ultimoA->posIni.x, ultimoA->posIni.y) );

    ultimoA->solicitar_NewStep();
}

void RVO_Manager::setVelocidad(float v)
{
    for(int i=0; i < aManager->agentes.size(); i++ )
        sim->setAgentMaxSpeed(i, (v*2.44)/100.f);
}

void RVO_Manager::setupScenario(float radius,
                                std::vector<entornoGrafico::obstaculo> listaObst)
{
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

//this is the first class. Here start all the calculations
void RVO_Manager::updateVisualization()
{
    //std::vector<agents::agent *> agentesDisponibles = aManager->getAgentesDisponibles();

    reachedGoal(aManager->getAgentesDisponibles());

    for(agents::agent *a : aManager->getAgentesDisponibles())
    {
        a->calculateVelocities( sim->getAgentPosition( a->ID-1 ),
                                sim->getAgentVelocity( a->ID-1 ),
                                sim->getTimeStep() );
    }

    setPreferredVelocities(aManager->getAgentesDisponibles());

    sim->doStep();
}

void RVO_Manager::reachedGoal(std::vector<agents::agent *> agentesDisponibles)
{
    /* Check if all agents have reached their goals. */

    for(agents::agent *a : agentesDisponibles)
    {
        if (RVO::absSq(sim->getAgentPosition( a->ID-1 ) - a->get_goal_RVO()) < 4.0f )
                a->solicitar_NewStep();
    }
}

void RVO_Manager::setPreferredVelocities(std::vector<agents::agent *> agentesDisponibles)
{
    for(agents::agent *a : agentesDisponibles)
    {
        RVO::Vector2 goal = a->get_goal_RVO();

        RVO::Vector2 goalVector = goal - sim->getAgentPosition(a->ID-1);

        if (RVO::absSq(goalVector) > 1.0f)
            goalVector = RVO::normalize(goalVector);

        sim->setAgentPrefVelocity( a->ID-1 , goalVector);

        //Perturb a little to avoid deadlocks due to perfect symmetry.
        float angle = std::rand() * 2.0f * M_PI / RAND_MAX;
        float dist = std::rand() * 0.0001f / RAND_MAX;

        sim->setAgentPrefVelocity( a->ID-1 , sim->getAgentPrefVelocity(a->ID-1) +
                                   dist * RVO::Vector2(std::cos(angle), std::sin(angle)));
    }

}
