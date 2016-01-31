#ifndef BALL_H
#define BALL_H


#include "entornografico.h"
#include "headers/qt_stuff.h"
#include "headers/rvo_stuff.h"
#include "headers/sfml_stuff.h"
#include "headers/opencv_stuff.h"
#include "Tools/math.h"
#include "mycanvas.h"
#include "Agentes/agentpath.h"
#include "Agentes/dirystep.h"

class MyCanvas;

namespace agents
{
class agentPath;

class agent: public sf::CircleShape
{
    sf::Color ballColor;

    std::vector< diryStep* > dirtySteps;

    float oring;

    float tetaOriginal;
    float diferenciaTeta;


    void calculateVL(RVO::Vector2 velocity);
    void calculateTeta(RVO::Vector2 v);

public:
    static float radius;
    static float zonaSegura;
    float teta;
    float D;
    float L;

    float vL,vR;
    float vL_MAX,vR_MAX;
    float vL_MIN,vR_MIN;

    sf::Vector2f P;

    sf::Vector2f posGoal;
    sf::Vector2f posIni;

    sf::CircleShape p_grap;
    sf::CircleShape destinoShape;
    sf::Vertex *lineaDestino;

    agent(sf::Vector2f const posIni, sf::Vector2f posGoal,
           float spriteSize,
           float radius = 10.f, float zonaSegura = 0.f, float tetaInicial = 0.0f,
           sf::Color color = sf::Color::Red);

    void updateLineTrayectoria();
    void set_goal(sf::Vector2f posGoal );
    sf::Vector2f get_goal();
    RVO::Vector2 get_goal_RVO();

    void addDirtyStep(diryStep *step , float timeStep);
    std::vector<diryStep *> getDirtySteps();

    float getRadioCompleto();

    void calculateP();

    void draw(::MyCanvas *m);
    void update();
};



}//namespace

#endif // BALL_H
