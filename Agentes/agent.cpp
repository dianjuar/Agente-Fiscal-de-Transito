#include "agent.h"

using namespace agents;

float agent::radius = 0;
float agent::zonaSegura = 0;

float agent::D = 0;
float agent::L = 0;
float agent::wheelRadius = 0;

agent::agent(int ID, network::connections::ACO *aco, network::connections::SMA *sma,
             int direccionInicial):
                QObject(),
                ID(ID),
                direccion(direccionInicial),
                vL_linear(0), vR_linear(0),
                aco(aco),
                sma(sma),
                pasos(0),
                sended(false)
{
    setDireccion(direccionInicial);

    radius = entornoGrafico::mapa::medidaReal2Pixel(13.5f);
    zonaSegura = entornoGrafico::mapa::medidaReal2Pixel(4.f);

    D = 13.5f;
    L = 11.35;
    wheelRadius = 2.75f;

    switch (ID)
    {
        case 1:
            ballColor = sf::Color::Magenta;
        break;

        case 2:
            ballColor = sf::Color::Yellow;
        break;

        case 3:
            ballColor = sf::Color::Green;
        break;
    }

    // Create the ball
    setFillColor( ballColor );

    oring = entornoGrafico::mapa::spriteSize/2;

    set_RealP_Based_LogicalP(posIni, entornoGrafico::mapa::inicio_Point);


    setOrigin(radius,radius);
    setPosition(this->posIni);

    setRadius(radius);
    setOutlineThickness(zonaSegura);
    setOutlineColor( sf::Color(0,0,255,63) );

    destinoShape.setRadius( radius );
    destinoShape.setFillColor( sf::Color( 0, 255, 0, 20 ) );

    posGoal = sf::Vector2f(-1,-1);
    set_goal( posGoal );

    calculateP();
    p_grap.setRadius(4);
    p_grap.setFillColor(sf::Color::Black);
    p_grap.setOrigin( p_grap.getRadius()*1.5, p_grap.getRadius()*1.5 );

    //solicitad por primera vez un movimiento.
    aco->solicitarSiguientePaso(ID);
}

void agent::updateLineTrayectoria()
{
    lineaDestino[0] = sf::Vertex( this->getPosition() );
}

void agent::set_goal(sf::Vector2f posGoal)
{
    this->posGoal = sf::Vector2f(posGoal.x*entornoGrafico::mapa::spriteSize + oring,
                                 posGoal.y*entornoGrafico::mapa::spriteSize + oring);

    destinoShape.setPosition( this->posGoal );

    lineaDestino = new sf::Vertex[2];
    lineaDestino[0] = sf::Vertex( sf::Vector2f( posIni.x+radius,
                                                posIni.y+radius ));
    lineaDestino[1] = sf::Vertex( sf::Vector2f( this->posGoal.x+radius,
                                                this->posGoal.y+radius));
}

sf::Vector2f agent::get_goal()
{
    return posGoal;
}

RVO::Vector2 agent::get_goal_RVO()
{
    return RVO::Vector2( posGoal.x, posGoal.y );
}

float agent::getRadioCompleto()
{
    return radius+zonaSegura;
}

void agent::calculateVelocities(RVO::Vector2 position,RVO::Vector2 velocity,
                                float timeStep)
{ 
    if( posGoal == sf::Vector2f(-1,-1) )
        return;

    calculateVL(velocity, timeStep);
    calculateTeta(velocity,timeStep);
    calculateP();
    //qDebug()<<"***********************";

    setPosition( position.x(), position.y() );
}

void agent::calculateP()
{
    float Dtransformed = entornoGrafico::mapa::medidaReal2Pixel(D);

    P = sf::Vector2f( getPosition().x + Dtransformed*(cos(teta)),
                      getPosition().y + Dtransformed*(sin(teta)));

    p_grap.setPosition( P );
}

void agent::calculateVL(RVO::Vector2 velocity, float timeStep)
{
    //float denominador = qPow( cos(teta*M_PI/180)  ,2) - qPow( sin(teta*M_PI/180)  ,2);

    cv::Mat m1(2,2,CV_32F),
            m2(2,2,CV_32F),
            m,
            v(2,1,CV_32F),
            u;

    m1.at<float>(0,0) = cos(teta);
    m1.at<float>(0,1) = cos(teta);
    m1.at<float>(1,0) = sin(teta);
    m1.at<float>(1,1) = sin(teta);
    //tools::math::printMat(m1);

    m2.at<float>(0,0) = sin(teta);
    m2.at<float>(0,1) = -sin(teta);
    m2.at<float>(1,0) = -cos(teta);
    m2.at<float>(1,1) = cos(teta);
    //tools::math::printMat(m2);

    m = 0.5f*m1 + (D/L)*m2;

    v.at<float>(0,0) = entornoGrafico::mapa::pixel2MedidaReal( velocity.x() )*100;
    v.at<float>(1,0) = entornoGrafico::mapa::pixel2MedidaReal( velocity.y() )*100;
    //tools::math::printMat(v);

    u = m.inv()*v;
    //tools::math::printMat(u);

    vL_linear = u.at<float>(0,0);
    vR_linear = u.at<float>(1,0);

    vL_angular = (vL_linear/wheelRadius)*(180/M_PI);
    vR_angular = (vR_linear/wheelRadius)*(180/M_PI);

    if( ID == 1 || ID == 2 )
        emit velocidadesCalculadas(ID,
                                   vR_angular,
                                   vL_angular);
}

void agent::calculateTeta(RVO::Vector2 velocity,float timeStep)
{
    float deltaT = timeStep/100;
    float w = (vR_linear - vL_linear)/L;

    teta = teta + w*deltaT;
}

void agent::set_RealP_Based_LogicalP(sf::Vector2f &real_P, sf::Vector2f logial_P)
{
    real_P = sf::Vector2f(logial_P.x * entornoGrafico::mapa::spriteSize + oring,
                          logial_P.y * entornoGrafico::mapa::spriteSize + oring);
}

void agent::draw(::simulacion *m)
{
    m->RenderWindow::draw(*this);
    m->RenderWindow::draw( p_grap );
}

void agent::solicitar_NewStep()
{
    sended = true;
    aco->solicitarSiguientePaso(ID);
}

void agent::setDireccion(int newDireccion, bool enviarSMA)
{
    direccion = newDireccion;
    teta = direccion*(M_PI/4)-(M_PI/2);

    int grados = ::tools::math::cuantosGradosGiraryHaciaDonde(this->direccion, newDireccion);

    if(enviarSMA)
        sma->sendRotation( ID, grados );
}

void agent::newStep(int direccion, float distancia, sf::Vector2f newPos)
{
    sended = false;
    setDireccion(direccion);
    set_RealP_Based_LogicalP( posGoal, newPos );
}

bool agent::isSended()
{
    return sended;
}
