#include "rvo_manager.h"

float RVO_Manager::TimeStep_real = 0;

float RVO_Manager::maxAgents        = 3;
float RVO_Manager::timeHorizon      = 3*500.f;
float RVO_Manager::timeHorizonObst  = 0.5*500.f;

float RVO_Manager::timeStep         = 5.f;
float RVO_Manager::maxSpeed         = 0.0f;

//este es el que estoy usando
RVO_Manager::RVO_Manager(agents::agentManager *aManager, simulacion *REALsim,
                         float timeStep, float velocity):
    aManager(aManager),
    REALsim(REALsim)
{
    this->timeStep = timeStep;
    maxSpeed = (velocity*2.44)/100.f;
    RVOsim = new RVO::RVOSimulator();

    TimeStep_real = timeStep*100;
}

void RVO_Manager::add_obstacles(std::vector<entornoGrafico::c::obstaculo*> listaObst)
{
    for(entornoGrafico::c::obstaculo *ob: listaObst)
        RVOsim->addObstacle( ob->getPoint_RVO() );

    RVOsim->processObstacles();
}

void RVO_Manager::add_UltimoAgente()
{
    agents::agent *ultimoA =  aManager->ultimoAgene();
    RVOsim->addAgent(  RVO::Vector2(ultimoA->posIni.x, ultimoA->posIni.y) );


    if (RVOsim->getNumAgents() == 1)
        REALsim->reset_currentTime();
}

void RVO_Manager::setVelocidad(float v)
{
    for(int i=0; i < aManager->size(); i++ )
        RVOsim->setAgentMaxSpeed(i, (v*2.44)/100.f);
}

void RVO_Manager::setupScenario(float radius,
                                std::vector<entornoGrafico::c::obstaculo *> listaObst)
{
    this->aManager = aManager;

    /* Specify the global time step of the simulation. */
    RVOsim->setTimeStep(timeStep);

    /* Specify the default parameters for agents that are subsequently added. */
    RVOsim->setAgentDefaults( radius*(750/100),
                           maxAgents,
                           timeHorizon, timeHorizonObst,
                           radius,
                           maxSpeed);

    add_obstacles(listaObst);
}

void RVO_Manager::updateVisualization()
{
    std::vector<agents::agent *> agentesDisponibles = aManager->getAgentesDisponibles();

    reachedGoal(agentesDisponibles);
    agentesDisponibles = aManager->getAgentesDisponibles();

    for (size_t i = 0; i < agentesDisponibles.size(); ++i)
    {
        agents::agent *a = agentesDisponibles.at(i);

        a->calculateVelocities( RVOsim->getAgentPosition( a->ID-1 ),
                                RVOsim->getAgentVelocity( a->ID-1 ),
                                RVOsim->getTimeStep());
    }

    setPreferredVelocities(agentesDisponibles);

    RVOsim->doStep();
}

void RVO_Manager::reachedGoal(std::vector<agents::agent *> agentesDisponibles)
{
    /* Check if all agents have reached their goals. */

    for (size_t i = 0; i < agentesDisponibles.size(); ++i)
    {
        agents::agent *a = agentesDisponibles.at(i);

        if (RVO::absSq(RVOsim->getAgentPosition( a->ID-1 ) - a->get_goal_RVO()) < 4.0f )
        {
            RVOsim->setAgentVelocity( a->ID-1, RVO::Vector2(0,0) );
            RVOsim->setAgentPrefVelocity(a->ID-1, RVO::Vector2(0,0));
            a->solicitar_NewStep();
        }
    }
}

void RVO_Manager::setPreferredVelocities(std::vector<agents::agent *> agentesDisponibles)
{
    for (int i = 0; i < static_cast<int>(agentesDisponibles.size()); ++i)
    {
        agents::agent *a = agentesDisponibles.at(i);

        RVO::Vector2 goal = a->get_goal_RVO();

        if(goal == RVO::Vector2(-1,-1) )
            continue;

        RVO::Vector2 goalVector = goal - RVOsim->getAgentPosition( a->ID-1 );

        if (RVO::absSq(goalVector) > 1.0f)
            goalVector = RVO::normalize(goalVector);


        RVOsim->setAgentPrefVelocity( a->ID-1 , goalVector);

        //Perturb a little to avoid deadlocks due to perfect symmetry.
        float angle = std::rand() * 2.0f * M_PI / RAND_MAX;
        float dist = std::rand() * 0.0001f / RAND_MAX;

        RVOsim->setAgentPrefVelocity( a->ID-1 , RVOsim->getAgentPrefVelocity(a->ID-1) +
                                  dist * RVO::Vector2(std::cos(angle), std::sin(angle)));
    }
}

int RVO_Manager::getTiempoSimulacion()
{
    return RVOsim->getGlobalTime()/100.f;
}
