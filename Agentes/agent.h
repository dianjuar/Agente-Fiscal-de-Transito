#ifndef BALL_H
#define BALL_H


#include "entornografico.h"
#include "headers/qt_stuff.h"
#include "headers/rvo_stuff.h"
#include "headers/sfml_stuff.h"
#include "headers/opencv_stuff.h"
#include "Tools/math.h"
#include "simulacion.h"
#include "network/connection_sma.h"

//----------prototyping----------------
class simulacion;

namespace network
{
    namespace connections
    {
        class SMA;
        class ACO;
    }
}
//----------prototyping----------------

namespace agents
{

class agent: public QObject,
             public sf::CircleShape
{
    Q_OBJECT

    network::connections::ACO *aco;
    network::connections::SMA *sma;

    sf::Color ballColor;

    int pasos;
    bool sended;

    float oring;

    void calculateVL(RVO::Vector2 velocity, float timeStep);
    void calculateTeta(RVO::Vector2 velocity, float timeStep);
    void set_RealP_Based_LogicalP(sf::Vector2f &real_P, sf::Vector2f logial_P);

    void setDireccion(int newDireccion, bool enviarSMA = false);

signals:
    void velocidadesCalculadas(int ID, float VL, float VR);
    void solicitarNuevaVelocidad();

public:
    enum
    {
        norte=0,
        noreste=1,
        este=2,
        sureste=3,
        sur=4,
        suroeste=5,
        oeste=6,
        noroeste=7,
    };

    int ID;
    int direccion;

    static float radius;
    static float D;
    static float L;
    static float wheelRadius;
    static float zonaSegura;

    float teta;

    float vL_linear,vR_linear;
    float vL_angular, vR_angular;

    sf::Vector2f P;

    sf::Vector2f posGoal;
    sf::Vector2f posIni;

    sf::CircleShape p_grap;
    sf::CircleShape destinoShape;
    sf::Vertex *lineaDestino;

    //constructor
    agent(int ID,
          network::connections::ACO *aco, network::connections::SMA *sma,
          int direccionInicial = 0.0f);

    void updateLineTrayectoria();
    void set_goal(sf::Vector2f posGoal );
    sf::Vector2f get_goal();
    RVO::Vector2 get_goal_RVO();

    void calculateVelocities(RVO::Vector2 position, RVO::Vector2 velocity , float timeStep);

    float getRadioCompleto();

    void calculateP();

    void draw(::simulacion *m);
    void update();

    void solicitar_NewStep();

    void newStep(int direccion, float distancia, sf::Vector2f newPos);
    bool isSended();
};

}//namespace

#endif // BALL_H
