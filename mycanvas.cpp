#include "mycanvas.h"

MyCanvas::MyCanvas(QWidget* Parent, const QPoint& Position, const QSize& Size):
    QSFMLCanvas(Parent, Position, Size),
    numeroMaximoPath(0),
    printed_V_MIN_MAX(false),
    tiempoTranscurrido(0)
{
    map_longitudPorCuadro_REAL = 10.f;
    radioReal = 2.5f;
    zonaSeguraReal = 0.5f;

    float globalTime = 0.1;
    float maxVelocity = 10.f;
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

    QString map_Str = "00000\n"
                      "00000\n"
                      "00000\n"
                      "00000\n"
                      "00000";

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
    //this->reset_currentTime();

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
    agents::agent *a1 = new agents::agent( sf::Vector2f( 0,
                                                         1),
                                           sf::Vector2f( 2,
                                                         1),
                                           mapa->spriteSize,
                                           radioScaled, zonaSeguraScaled,
                                           0.f,
                                           sf::Color::Red);
    aManager->addAgent(a1);


    //agente 2
    agents::agent *a2 = new agents::agent( sf::Vector2f( 1,
                                                         0),
                                           sf::Vector2f( 1,
                                                         2),
                                           mapa->spriteSize,
                                           radioScaled, zonaSeguraScaled,
                                           180,
                                           sf::Color::Magenta);
    aManager->addAgent(a2);

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
