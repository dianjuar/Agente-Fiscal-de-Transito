#include "agent.h"

using namespace agents;

float agent::D_real = 13.5;
float agent::L_real = 11.35;
float agent::wheelRadius_real = 2.75;

float agent::radius_real        = 13.5f;
float agent::zonaSegura_real    = 4.0f;
float agent::radius_pixel       = 0;
float agent::zonaSegura_pixel   = 0;

agent::agent(int ID, network::connections::ACO *aco, network::connections::SMA *sma,
             int direccionInicial):
                QObject(),
                ID(ID),
                direccion(direccionInicial),
                vL_linear(0), vR_linear(0),
                aco(aco),
                sma(sma),
                pasos(0), Npasos_solicitudCDT(3),
                sended_NextStep(true), sended_CRT(false), waitingForCorrection(false)
{
    radius_pixel       = entornoGrafico::mapa::medidaReal2Pixel( agent::radius_real );
    zonaSegura_pixel   = entornoGrafico::mapa::medidaReal2Pixel( agent::zonaSegura_real  );


    setDireccion(direccionInicial);

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

    set_RealP_Based_LogicalP(posIni, entornoGrafico::mapa::C_inicio->logicalPoint);


    setOrigin(radius_pixel,radius_pixel);
    setPosition(this->posIni);

    setRadius(radius_pixel);
    setOutlineThickness(zonaSegura_pixel);
    setOutlineColor( sf::Color(0,0,255,63) );

    destinoShape.setRadius( radius_pixel );
    destinoShape.setFillColor( sf::Color( 0, 255, 0, 20 ) );

    posGoal_real = sf::Vector2f(-1,-1);
    set_goal( posGoal_real );

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
    this->posGoal_digital = posGoal;
    this->posGoal_real = sf::Vector2f(posGoal.x*entornoGrafico::mapa::spriteSize + oring,
                                 posGoal.y*entornoGrafico::mapa::spriteSize + oring);

    destinoShape.setPosition( this->posGoal_real );

    lineaDestino = new sf::Vertex[2];
    lineaDestino[0] = sf::Vertex( sf::Vector2f( posIni.x+radius_pixel,
                                                posIni.y+radius_pixel ));
    lineaDestino[1] = sf::Vertex( sf::Vector2f( this->posGoal_real.x+radius_pixel,
                                                this->posGoal_real.y+radius_pixel));
}

sf::Vector2f agent::get_goal()
{
    return posGoal_real;
}

RVO::Vector2 agent::get_goal_RVO()
{
    return RVO::Vector2( posGoal_real.x, posGoal_real.y );
}

float agent::getRadioCompleto_pixel()
{
    return radius_pixel+zonaSegura_pixel;
}

float agent::getRadioCompleto_real()
{
    return radius_real+radius_real;
}

void agent::calculateVelocities(RVO::Vector2 position,RVO::Vector2 velocity,
                                float timeStep)
{ 
    if( posGoal_real == sf::Vector2f(-1,-1) )
        return;

    calculateVL(velocity, timeStep);
    calculateTeta(velocity,timeStep);
    calculateP();
    //qDebug()<<"***********************";

    setPosition( position.x(), position.y() );
}

void agent::calculateP()
{
    float Dtransformed = entornoGrafico::mapa::medidaReal2Pixel(D_real);

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

    m = 0.5f*m1 + (D_real/L_real)*m2;

    v.at<float>(0,0) = entornoGrafico::mapa::pixel2MedidaReal( velocity.x() )*100;
    v.at<float>(1,0) = entornoGrafico::mapa::pixel2MedidaReal( velocity.y() )*100;
    //tools::math::printMat(v);

    u = m.inv()*v;
    //tools::math::printMat(u);

    vL_linear = u.at<float>(0,0);
    vR_linear = u.at<float>(1,0);

    vL_angular = (vL_linear/wheelRadius_real)*(180/M_PI);
    vR_angular = (vR_linear/wheelRadius_real)*(180/M_PI);

    if( ID == 1 || ID == 2 || ID == 3)
        emit velocidadesCalculadas(ID,
                                   vR_angular,
                                   vL_angular);
}

void agent::calculateTeta(RVO::Vector2 velocity,float timeStep)
{
    float deltaT = timeStep/100;
    float w = (vR_linear - vL_linear)/L_real;

    teta = teta + w*deltaT;
}

void agent::set_RealP_Based_LogicalP(sf::Vector2f &real_P, sf::Vector2f logial_P)
{
    real_P = sf::Vector2f(logial_P.x * entornoGrafico::mapa::spriteSize + oring,
                          logial_P.y * entornoGrafico::mapa::spriteSize + oring);
}

void agent::draw(::simulacion *sim)
{
    sim->RenderWindow::draw(*this);
    sim->RenderWindow::draw( p_grap );
}

void agent::solicitar_NewStep()
{

    if(pasos < Npasos_solicitudCDT && !sended_NextStep)
    {
        sended_NextStep = true;
        pasos++;
        aco->solicitarSiguientePaso(ID);
    }
    else
    {
        waitingForCorrection = true;
        pasos = 0;
        sma->solicitarCDT(ID, direccion, posGoal_digital);
    }
}

void agent::setDireccion(int newDireccion, bool enviarSMA)
{
    teta = newDireccion*(M_PI/4)-(M_PI/2);

    if(enviarSMA && direccion != newDireccion)
    {
        int grados = ::tools::math::cuantosGradosGiraryHaciaDonde(direccion, newDireccion);
        sma->sendRotation( ID, grados );
    }

    direccion = newDireccion;
}

void agent::newStep(int direccion, float distancia, sf::Vector2f newPos)
{
    setDireccion(direccion,true);
    set_goal(newPos);

    //es necesario que esté acá de último para que le de chance de enviar la rotacion.
    sended_NextStep = false;
}

void agent::correccionFinalizada()
{
    sended_NextStep = false;
    waitingForCorrection = false;
}

bool agent::isAvaliable()
{
    if( sended_NextStep || waitingForCorrection)
        return false;

    return true;
}
