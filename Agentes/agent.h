#ifndef BALL_H
#define BALL_H


#include "entornografico.h"
#include "headers/qt_stuff.h"
#include "headers/rvo_stuff.h"
#include "headers/sfml_stuff.h"
#include "headers/opencv_stuff.h"
#include "Tools/math.h"
#include "mycanvas.h"

class MyCanvas;

namespace agents
{
class agentPath;

class agent: public QObject,
             public sf::CircleShape
{
    Q_OBJECT

    int ID;
    float whatIsDiferentVelocities;

    sf::Color ballColor;
    float oring;

    float tetaOriginal;
    float diferenciaTeta;


    void calculateVL(RVO::Vector2 velocity);
    void calculateTeta(float timeStep);

signals:
    void velocidadesCalculadas(int ID, float VL, float VR);


public:
    static float radius;
    static float zonaSegura;
    float teta;
    float D;
    float L;

    float vL,vR;
    float vL_real, vR_real;

    sf::Vector2f P;

    sf::Vector2f posGoal;
    sf::Vector2f posIni;

    sf::CircleShape p_grap;
    sf::CircleShape destinoShape;
    sf::Vertex *lineaDestino;

    agent(int ID, sf::Vector2f const posIni, sf::Vector2f posGoal,
           float spriteSize,
           float radius = 10.f, float zonaSegura = 0.f, float tetaInicial = 0.0f, float D=10.f, float L=20.f,
           sf::Color color = sf::Color::Red);

    void updateLineTrayectoria();
    void set_goal(sf::Vector2f posGoal );
    sf::Vector2f get_goal();
    RVO::Vector2 get_goal_RVO();

    void calculateVelocities(RVO::Vector2 position, RVO::Vector2 velocity , float timeStep);

    float getRadioCompleto();

    void calculateP();

    void draw(::MyCanvas *m);
    void update();
};



}//namespace

#endif // BALL_H
