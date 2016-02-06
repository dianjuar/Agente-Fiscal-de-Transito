#include "agent.h"

using namespace agents;

float agent::radius = 0;
float agent::zonaSegura = 0;

float agent::D = 0;
float agent::L = 0;
float agent::wheelRadius = 0;

agent::agent(int ID,
             const sf::Vector2f posIni, sf::Vector2f posGoal,
             float spriteSize, float tetaInicial,
             sf::Color color):
                QObject(),
                ID(ID),
                teta(tetaInicial),
                ballColor(color),
                vL(0), vR(0),
                whatIsDiferentVelocities(0.0f)
{
    radius = entornoGrafico::mapa::medidaReal2Pixel(13.5f);
    zonaSegura = entornoGrafico::mapa::medidaReal2Pixel(4.f);

    D = entornoGrafico::mapa::medidaReal2Pixel(4.f);
    L = entornoGrafico::mapa::medidaReal2Pixel(11.35);
    wheelRadius = entornoGrafico::mapa::medidaReal2Pixel(2.75f);




    // Create the ball
    setFillColor( color );
    float hipotenusaExterna = sqrt( 2*qPow(spriteSize/2,2) );
    float hipotenusaInterna = sqrt( 2*qPow(radius,2) );

    oring = hipotenusaExterna < hipotenusaInterna ?
                            hipotenusaInterna-hipotenusaExterna:
                            hipotenusaExterna-hipotenusaInterna;

    //oring = qPow( qPow(oring,2)/2 ,0.5f)*(hipotenusaExterna > hipotenusaInterna ? 1:-1);

    oring = spriteSize/2;
    /*this->posIni = sf::Vector2f(posIni.x * spriteSize + oring,
                                posIni.y * spriteSize + oring);*/

    this->posIni = sf::Vector2f(posIni.x * spriteSize + oring,
                                posIni.y * spriteSize + oring);

    setOrigin(radius,radius);

    setPosition(this->posIni);

    //setOrigin( oring,oring  );
    setRadius(radius);
    setOutlineThickness(zonaSegura);
    setOutlineColor( sf::Color(0,0,255,63) );

    destinoShape.setRadius( radius );
    destinoShape.setFillColor( sf::Color( 0, 255, 0, 20 ) );

    set_goal( posGoal );

    calculateP();
    p_grap.setRadius(4);
    p_grap.setFillColor(sf::Color::Black);
    p_grap.setOrigin( p_grap.getRadius()*1.5, p_grap.getRadius()*1.5 );

    tetaOriginal = teta;
    diferenciaTeta = 0;
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
    calculateVL(velocity, timeStep);
    calculateTeta(velocity,timeStep);
    calculateP();
    //qDebug()<<"***********************";

    setPosition( position.x(), position.y()  );
}

void agent::calculateP()
{
    P = sf::Vector2f( getPosition().x + D*(cos(teta*(M_PI/180))),
                      getPosition().y + D*(sin(teta*(M_PI/180))));

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

    float tetaRadian = teta*(M_PI/180);

    m1.at<float>(0,0) = cos(tetaRadian);
    m1.at<float>(0,1) = cos(tetaRadian);
    m1.at<float>(1,0) = sin(tetaRadian);
    m1.at<float>(1,1) = sin(tetaRadian);
    //tools::math::printMat(m1);

    m2.at<float>(0,0) = sin(tetaRadian);
    m2.at<float>(0,1) = -sin(tetaRadian);
    m2.at<float>(1,0) = -cos(tetaRadian);
    m2.at<float>(1,1) = cos(tetaRadian);
    //tools::math::printMat(m2);

    m = 0.5f*m1 + (D/L)*m2;

    v.at<float>(0,0) = velocity.x();
    v.at<float>(1,0) = velocity.y();
    //tools::math::printMat(v);

    u = m.inv()*v;
    //tools::math::printMat(u);

    vL = u.at<float>(0,0);
    vR = u.at<float>(1,0);

   // vL = vL*(100/timeStep);
   // vR = vR*(100/timeStep);

    vL_real = vL*(180/M_PI)*(100/timeStep);
    vR_real = vR*(180/M_PI)*(100/timeStep);
    bool changed = false;

    /*if( qAbs(vl_realNEW - vL_real ) >= whatIsDiferentVelocities )
    {
        vL_real = vl_realNEW;
        changed = true;
    }

    if( qAbs(vr_realNEW - vR_real ) >= whatIsDiferentVelocities )
    {
        vR_real = vr_realNEW;
        changed = true;
    }*/

    if( ID == 1 || ID == 1 )
        emit velocidadesCalculadas(ID, vL_real, vR_real);
}

void agent::calculateTeta(RVO::Vector2 velocity,float timeStep)
{
  /* float w;
   w = v.x() == 0 ? 0:atan(v.y()/v.x())*180/M_PI;
   //teta = w;*/

   teta = teta + ((vR_real - vL_real)*(wheelRadius/L));

}

void agent::draw(::MyCanvas *m)
{
    m->RenderWindow::draw(*this);
    m->RenderWindow::draw( p_grap );
}
