#include "mycanvas.h"

MyCanvas::MyCanvas(QWidget* Parent, const QPoint& Position, const QSize& Size):
    QSFMLCanvas(Parent, Position, Size),
    numeroMaximoPath(0),
    printed_V_MIN_MAX(false),
    tiempoTranscurrido(0)
{
    map_longitudPorCuadro_REAL = 22.02271f;
    radioReal = 13.5f;
    zonaSeguraReal = 0.5f;

    float globalTime = 3;
    float maxVelocity = 0.05f;
    rvo  = new RVO_Manager(globalTime, maxVelocity);

    aManager = new agents::agentManager(this);
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
                      "10000001\n"
                      "10000001\n"
                      "10000001\n"
                      "10000001\n"
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
    rvo->updateVisualization(aManager->agentes);

    float delay = (rvo->sim->getGlobalTime()/100.f)-currentTime;
    QThread::sleep( delay < 0 ? 0:delay );

    if( rvo->haveReachedTheirGoal )
    {
        qDebug()<<"Time RVO:"<<rvo->sim->getGlobalTime()/100<<"       Time real"<<currentTime+delay;
        exit(EXIT_SUCCESS);
    }
}

void MyCanvas::setup_agentes()
{
    float D,L;

    D = radioScaled;
    L = mapa->medidaReal2Pixel(11.35f);




    agents::agent *a1 = new agents::agent( sf::Vector2f( 2,
                                                         3),
                                           sf::Vector2f( 5,
                                                         3),
                                           mapa->spriteSize,
                                           radioScaled, zonaSeguraScaled,
                                           0.f,
                                           D, L,
                                           sf::Color::Red);
    aManager->addAgent(a1);


    //agente 2
   /* agents::agent *a2 = new agents::agent( sf::Vector2f( 3,
                                                         2),
                                           sf::Vector2f( 3,
                                                         5),
                                           mapa->spriteSize,
                                           radioScaled, zonaSeguraScaled,
                                           90,
                                           D, L,
                                           sf::Color::Magenta);
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
