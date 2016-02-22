#include "mycanvas.h"

MyCanvas::MyCanvas(QWidget* Parent, const QPoint& Position, const QSize& Size):
    QSFMLCanvas(Parent, Position, Size),
    numeroMaximoPath(0),
    printed_V_MIN_MAX(false),
    tiempoTranscurrido(0)
{
    map_longitudPorCuadro_REAL = 22.f;
    radioReal = 13.5f;
    zonaSeguraReal = 4.f;

    float globalTime = 5;
    float maxVelocity = 66.f/100.f;
    rvo  = new RVO_Manager(globalTime, maxVelocity);

    connection_SMA = new network::connections::SMA();

    aManager = new agents::agentManager(connection_SMA);
    //SIGNAL(connected())
    connect( connection_SMA , SIGNAL(newIncomingConnection()),
             this, SLOT(inicioDeLaSimulacion()));
}

void MyCanvas::OnInit()
{
    // Load the image
    qDebug()<<"onInit";
    QString dir = QDir::currentPath();
    qDebug()<< "HELLO: " << dir;

    sf::Texture libreTex,obstTex;

    tools::sfml::loadTexture(libreTex,"src/Img/Mapa/libre.png");
    tools::sfml::loadTexture(obstTex,"src/Img/Mapa/obstaculo.png");

    setBackgroudColor(sf::Color::Black);

    QString map_Str = "11111111\n"
                      "10000000\n"
                      "10000000\n"
                      "00000000\n"
                      "10000000\n"
                      "10000001\n"
                      "10000001\n"
                      "11111111";



    mapa = new entornoGrafico::mapa( map_Str,
                                     map_longitudPorCuadro_REAL,
                                     QSFMLCanvas::size().width(),
                                     libreTex, obstTex );

    radioScaled = mapa->medidaReal2Pixel(radioReal);
    zonaSeguraScaled = mapa->medidaReal2Pixel(zonaSeguraReal);

    setup_agentes();
    rvo->setupScenario( mapa->medidaReal2Pixel(radioReal+zonaSeguraReal),
                        aManager->agentes,
                        mapa->obstaculos);
}

void MyCanvas::draw()
{
    mapa->drawMapa( this );

    for(auto *agente : aManager->agentes)
        agente->draw(this);
}

void MyCanvas::update(float deltaTime, float currentTime)
{   
    if(connection_SMA->isConnected())
    {
        rvo->updateVisualization(aManager->agentes);

        float delay = (rvo->sim->getGlobalTime()/100.f)-currentTime;
        QThread::sleep( delay < 0 ? 0:delay );

        if( rvo->haveReachedTheirGoal )
        {

            qDebug()<<"***********";
            qDebug()<<"Max speed"<<rvo->get_maxSpeed()*100;
            qDebug()<<"Velocidad Linear"<<((agents::agent*) aManager->agentes[0])->vL_linear;
            qDebug()<<"Tiempo teorico ="<<map_longitudPorCuadro_REAL/aManager->agentes[0]->vL_linear<<
                      "      ------ Tiempo Real"<<currentTime+delay;
            qDebug()<<"Tiempo teorico ="<<map_longitudPorCuadro_REAL/aManager->agentes[0]->vL_linear<<
                      "      ------ Tiempo Real"<<currentTime+delay;
            qDebug()<<"Realcion:"<<aManager->agentes[0]->vL_linear/rvo->get_maxSpeed();
            //qDebug()<<"Time RVO:"<<rvo->sim->getGlobalTime()/100<<"       Time real"<<currentTime+delay;

            exit(EXIT_SUCCESS);
        }
    }
}

void MyCanvas::inicioDeLaSimulacion()
{
    reset_currentTime();
}

void MyCanvas::setup_agentes()
{
    agents::agent *a1 = new agents::agent( 1,
                                           sf::Vector2f( 2,
                                                         3),
                                           sf::Vector2f( 3,
                                                         3),
                                           mapa->spriteSize,
                                           0.f,
                                           sf::Color::Magenta);
    aManager->addAgent(a1);


   /* //agente 2
    agents::agent *a2 = new agents::agent( 2,
                                           sf::Vector2f( 5,
                                                         3),
                                           sf::Vector2f( 2,
                                                         3),
                                           mapa->spriteSize,
                                           180,
                                           sf::Color::Yellow);
    aManager->addAgent(a2);*/

    //agente 3
   /* agents::agent *a3 = new agents::agent( sf::Vector2f( 4,
                                                         0),
                                           sf::Vector2f( 3,
                                                         2),
                                           mapa->spriteSize,
                                           radioScaled, zonaSeguraScaled,
                                           sf::Color::Blue);
    aManager->addAgent(a3);*/
}
