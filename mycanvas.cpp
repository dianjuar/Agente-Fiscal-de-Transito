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

    /*connect( rvo, SIGNAL(agentsHaveReachedTheirGoal()),
             aManager, SLOT(buildDiferencialAgentsPath()));*/
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



    /*if(rvo->haveReachedTheirGoal)
            drawCleanSteps();*/

    /*for(entornoGrafico::obstaculo obst:mapa->obstaculos)
        for (int i = 0; i <= numeroDeArista; ++i)
        {
            sf::CircleShape c;
            c.setRadius(4);
            c.setFillColor( sf::Color::White );
            c.setPosition( obst.getPoint_RVO().at(i).x(), obst.getPoint_RVO().at(i).y() );
            c.setOrigin(4,4);
            this->RenderWindow::draw(c);
        }*/
}

void MyCanvas::update(float deltaTime, float currentTime)
{   
    if( !rvo->haveReachedTheirGoal /*&& currentTime >= rvo->sim->getTimeStep()/100.f*/)
    {
        tiempoTranscurrido += currentTime;

        if(!printed_V_MIN_MAX)
        {
            printed_V_MIN_MAX = !printed_V_MIN_MAX;
            this->reset_currentTime();
        }

        rvo->updateVisualization(aManager->agentes,false);
        qDebug()<<"Time RVO:"<<rvo->sim->getGlobalTime()/100<<"       Time real"<<tiempoTranscurrido;

        this->reset_currentTime();
    }
    /*else
    {
        if(!printed_V_MIN_MAX)
        {
            int nAgente=0;
            foreach (agents::agent *a, aManager->agentes)
            {
               // qDebug()<<"**************************";
                qDebug()<<"Agente Nº"<<nAgente++;
                qDebug()<<"VL_MIN"<<a->vL_MIN<<"VR_MIN"<<a->vR_MIN;
                qDebug()<<"VL_MAX"<<a->vL_MAX<<"VR_MAX"<<a->vL_MAX;
            }
            printed_V_MIN_MAX = !printed_V_MIN_MAX;
        }
    }*/
}

void MyCanvas::setup_agentes()
{
    agents::agent *a1 = new agents::agent( sf::Vector2f( 0,
                                                         2),
                                           sf::Vector2f( 1,
                                                         2),
                                           mapa->spriteSize,
                                           radioScaled, zonaSeguraScaled,
                                           0.f,
                                           sf::Color::Red);
    aManager->addAgent(a1);


    //agente 2
    agents::agent *a2 = new agents::agent( sf::Vector2f( 1,
                                                         2),
                                           sf::Vector2f( 0,
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
