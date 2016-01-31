#include "agent.h"

using namespace agents;

float agent::radius = 0;
float agent::zonaSegura = 0;

agent::agent(const sf::Vector2f posIni, sf::Vector2f posGoal,
               float spriteSize,
               float agentRadius, float zonaSeguraScaled, float tetaInicial,
               sf::Color color):
                teta(tetaInicial),
                ballColor(color),
                D(agentRadius), L(2*agentRadius),
                vL(0), vR(0),
                vR_MAX(-999),vL_MAX(-999), vL_MIN(999),vR_MIN(999)
{
    this->radius = agentRadius;
    this->zonaSegura = zonaSeguraScaled;

    // Create the ball
    setFillColor( color );
    float hipotenusaExterna = sqrt( 2*qPow(spriteSize/2,2) );
    float hipotenusaInterna = sqrt( 2*qPow(agentRadius,2) );

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
    setRadius(agentRadius);
    setOutlineThickness(zonaSeguraScaled);
    setOutlineColor( sf::Color(0,0,255,63) );

    destinoShape.setRadius( agentRadius );
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

std::vector<diryStep*> agent::getDirtySteps()
{
    return dirtySteps;
}

float agent::getRadioCompleto()
{
    return radius+zonaSegura;
}

void agent::addDirtyStep(diryStep *step, float timeStep)
{
    dirtySteps.push_back(step);

    calculateVL(step->velocity);
    calculateTeta(step->velocity);
    calculateP();
    qDebug()<<"***********************";

    setPosition( step->position.x(), step->position.y()  );
}

void agent::calculateP()
{
    P = sf::Vector2f( getPosition().x + D*(cos(teta*(M_PI/180))),
                      getPosition().y + D*(sin(teta*(M_PI/180))));

    p_grap.setPosition( P );

}

void agent::calculateVL(RVO::Vector2 velocity)
{
    //float denominador = qPow( cos(teta*M_PI/180)  ,2) - qPow( sin(teta*M_PI/180)  ,2);

    cv::Mat m1(2,2,CV_32F),
            m2(2,2,CV_32F),
            m,
            v(2,1,CV_32F),
            u;

    float tetaDegres = teta*M_PI/180;

    m1.at<float>(0,0) = cos(tetaDegres);
    m1.at<float>(0,1) = cos(tetaDegres);
    m1.at<float>(1,0) = sin(tetaDegres);
    m1.at<float>(1,1) = sin(tetaDegres);
    //tools::math::printMat(m1);

    m2.at<float>(0,0) = sin(tetaDegres);
    m2.at<float>(0,1) = -sin(tetaDegres);
    m2.at<float>(1,0) = -cos(tetaDegres);
    m2.at<float>(1,1) = cos(tetaDegres);
    //tools::math::printMat(m2);

    m = 0.5f*m1 + (D/L)*m2;

    v.at<float>(0,0) = velocity.x();
    v.at<float>(1,0) = velocity.y();
    //tools::math::printMat(v);

    u = m.inv()*v;
    //tools::math::printMat(u);

    vL = u.at<float>(0,0);
    vR = u.at<float>(1,0);

    if(vL > vL_MAX)
        vL_MAX = vL;
    if(vR > vR_MAX)
        vR_MAX = vR;

    if(vL < vL_MIN)
        vL_MIN = vL;
    if(vR < vR_MIN)
        vR_MIN = vR;

    //vL = ( velocity.x()*( cos(teta*M_PI/180) + sin(teta*M_PI/180) ) )/denominador;
    //vR = ( velocity.y()*( cos(teta*M_PI/180) + sin(teta*M_PI/180) ) )/denominador;

    qDebug()<<"VL:"<<vL<<"   VR:"<<vR;
}

void agent::calculateTeta(RVO::Vector2 v)
{
  /* float w;
   w = v.x() == 0 ? 0:atan(v.y()/v.x())*180/M_PI;
   //teta = w;*/

   teta = teta - ((vL-vR)/L)*100;

}

void agent::draw(::MyCanvas *m)
{
    m->RenderWindow::draw(*this);
    m->RenderWindow::draw( p_grap );
}
