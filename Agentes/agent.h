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
    int Npasos_solicitudCDT;
    bool sended_NextStep;
    bool sended_CRT;
    bool waitingForCorrection;

    ///
    /// \brief skipedStep. This flag will tell if the agent request another
    /// steps beacuse the first one was unreacheable
    ///
    bool skippedStep;

    float oring;

    void calculateVL(RVO::Vector2 velocity, float timeStep);
    void calculateTeta(RVO::Vector2 velocity, float timeStep);
    void set_RealP_Based_LogicalP(sf::Vector2f &real_P, sf::Vector2f logial_P);

    void setDireccion(int newDireccion, bool enviarSMA = false);

signals:
    void velocidadesCalculadas(int ID, float VL, float VR);
    void solicitarNuevaVelocidad();

public:

    ///
    /// id's 'cuadro' map
    ///
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

    ///
    /// \brief ID id of the robot
    ///
    int ID;

    ///
    /// \brief direccion. Direction of the robot, exm: norte, sur, suroeste.
    ///
    int direccion;

    ///
    /// \brief D_real distance between the center of the robot to the front of the robot. Expresed in centimeters
    ///
    static float D_real;

    ///
    /// \brief L_real distance between the wheels. Expresed in centimeters
    ///
    static float L_real;

    ///
    /// \brief wheelRadius_real radius of the wheel. Expresed in centimeters
    ///
    static float wheelRadius_real;

    ///
    /// \brief radius_real radius of the robot. Expresed in centimeters
    /// to know the real readius of the robot is necesary add the sage area of the robot.
    ///
    static float radius_real;

    ///
    /// \brief zonaSegura_real safe area of the robot. Expresed in centimeters.
    /// to know the real readius of the robot is necesary add the radius of the robot.
    ///
    static float zonaSegura_real;

    ///
    /// \brief radius_pixel radius of the robot. Expresed in pixels
    ///
    static float radius_pixel;

    ///
    /// \brief zonaSegura_pixel safe area of the robot. Expresed in pixels.
    ///
    static float zonaSegura_pixel;

    ///
    /// \brief teta orientation of the robot.
    ///
    float teta;

    ///
    /// \brief vL_linear, vR_linear. Linear velocites of the robot.
    /// The left one and the rigith one, in that order.
    /// Expresed in cm/sec
    ///
    float vL_linear,vR_linear;

    ///
    /// \brief vL_angular, vR_angular. Angular velocites of the robot.
    /// The left one and the rigith one, in that order.
    /// Expresed in rad/sec
    ///
    float vL_angular, vR_angular;


    ///
    /// \brief P    position of the efective center of the robot.
    ///
    sf::Vector2f P;
    ///
    /// \brief p_grap.   graphical efective center of the robot.
    ///
    sf::CircleShape p_grap;

    sf::Vector2f posGoal_real;
    sf::Vector2f posIni;
    sf::Vector2f posGoal_digital;

    sf::CircleShape destinoShape;
    sf::Vertex *lineaDestino;

    ///
    /// \brief agent            constructor of the agent
    /// \param ID.              Id of the agent. Must be unique.
    /// \param aco.             connection manager with ACO.
    /// \param sma              connection manager with SMA.
    /// \param direccionInicial initial direcction of the robot.
    ///
    agent(int ID,
          network::connections::ACO *aco, network::connections::SMA *sma,
          int direccionInicial = 0.0f);

    void updateLineTrayectoria();
    void set_goal(sf::Vector2f posGoal_real );
    sf::Vector2f get_goal();
    RVO::Vector2 get_goal_RVO();

    void calculateVelocities(RVO::Vector2 position, RVO::Vector2 velocity , float timeStep);

    ///
    /// \brief getRadioCompleto_pixel
    /// \return sumatory of raduis and safe area. Expresed in pixels.
    ///
    static float getRadioCompleto_pixel();

    ///
    /// \brief getRadioCompleto_real
    /// \return sumatory of raduis and safe area. Expresed in centimeters.
    ///
    static float getRadioCompleto_real();

    ///
    /// \brief calculateP. Calculates the new center of the robot.
    ///
    void calculateP();

    ///
    /// \brief draw.    draw the agent.
    /// \param sim.     the simulation who manager the visual area.
    ///
    void draw(::simulacion *sim);

    ///
    /// \brief solicitar_NewStep. Request a new step to ACO.
    ///
    void solicitar_NewStep();

    ///
    /// \brief newStep.     proccess the new step arrives from ACO-
    /// \param direccion.   the new direcction of the agent.
    /// \param distancia.   the distance to reach it.
    /// \param newPos.      the new position of the step. This position is logical.
    ///
    void newStep(int direccion, float distancia, sf::Vector2f newPos);

    void correccionFinalizada();

    ///
    /// \brief isAvaliable return if the agent is busy or avaliable
    /// \return true.   if the agent is avaliable
    ///         false.  if the agent is bussy
    ///
    bool isAvaliable();
};

}//namespace

#endif // BALL_H
