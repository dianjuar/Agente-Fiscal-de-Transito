#include "mycanvas.h"

MyCanvas::MyCanvas(QWidget* Parent, const QPoint& Position, const QSize& Size):
    QSFMLCanvas(Parent, Position, Size),
    numeroMaximoPath(0),
    printed_V_MIN_MAX(false),
    tiempoTranscurrido(0)
{
    map_longitudPorCuadro_REAL = 21.023796f;
    radioReal = 13.5f;
    zonaSeguraReal = 4.f;

    float globalTime = 5;
    float maxVelocity = 0.1;
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

    sf::Texture libreTex, obstTex, inicioTex, llegadaTex;

    tools::sfml::loadTexture(libreTex, "src/Img/Mapa/libre.png");
    tools::sfml::loadTexture(obstTex, "src/Img/Mapa/obstaculo.png");
    tools::sfml::loadTexture(inicioTex, "src/Img/Mapa/inicio.png");
    tools::sfml::loadTexture(llegadaTex, "src/Img/Mapa/llegada.png");


    setBackgroudColor(sf::Color::Black);

    QString map_Str = "111111111\n"
                      "100000000\n"
                      "100000000\n"
                      "100000000\n"
                      "100011000\n"
                      "100011000\n"
                      "100000000\n"
                      "100000000\n"
                      "000000000";



    mapa = new entornoGrafico::mapa( map_Str,
                                     map_longitudPorCuadro_REAL,
                                     QSFMLCanvas::size().width(),
                                     libreTex, obstTex, inicioTex, llegadaTex  );

    radioScaled = mapa->medidaReal2Pixel(radioReal);
    zonaSeguraScaled = mapa->medidaReal2Pixel(zonaSeguraReal);

    setup_agentes();
    rvo->setupScenario( mapa->medidaReal2Pixel(radioReal+zonaSeguraReal),
                        &aManager->agentes,
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
            qDebug()<<"Time RVO:"<<rvo->sim->getGlobalTime()/100<<"       Time real"<<currentTime+delay;
        }
    }
}

void MyCanvas::inicioDeLaSimulacion()
{
    reset_currentTime();
}

void MyCanvas::setup_agentes()
{

}
