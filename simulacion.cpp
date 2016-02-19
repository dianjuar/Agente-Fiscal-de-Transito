#include "simulacion.h"


simulacion::simulacion(QWidget* Parent, const QPoint& Position, const QSize& Size,
                       QString map, float dist,
                       network::connections::SMA *sma,
                       network::connections::ACO *aco):
    QSFMLCanvas(Parent, Position, Size),
    numeroMaximoPath(0),
    printed_V_MIN_MAX(false),
    tiempoTranscurrido(0),
    fullyLoaded(false),
    sma(sma),
    aco(aco),
    map(map),
    dist(dist)
{
    map_longitudPorCuadro_REAL = 21.023796f;
    radioReal = 13.5f;
    zonaSeguraReal = 4.f;

    float globalTime = 5;
    float maxVelocity = 0.1;
    aManager = new agents::agentManager(sma,aco);

    rvo  = new RVO_Manager(&aManager->agentes,globalTime, maxVelocity);

    connect( aManager, SIGNAL(newAgenteAdded()),
             rvo, SLOT(add_UltimoAgente()) );
}

void simulacion::OnInit()
{
    // Load the image
    qDebug()<<"onInit";
    QString dir = QDir::currentPath();
    qDebug()<< "HELLO: " << dir;

    tools::sfml::loadTexture(libreTex, "src/Img/Mapa/libre.png");
    tools::sfml::loadTexture(obstTex, "src/Img/Mapa/obstaculo.png");
    tools::sfml::loadTexture(inicioTex, "src/Img/Mapa/inicio.png");
    tools::sfml::loadTexture(llegadaTex, "src/Img/Mapa/llegada.png");

    setBackgroudColor(sf::Color::Black);

    radioScaled = mapa->medidaReal2Pixel(radioReal);
    zonaSeguraScaled = mapa->medidaReal2Pixel(zonaSeguraReal);

    setInformacionGrafica(map,dist);
}

void simulacion::setInformacionGrafica(QString map, float dist)
{
    map_longitudPorCuadro_REAL = dist;

    mapa = new entornoGrafico::mapa( map,
                                     map_longitudPorCuadro_REAL,
                                     QSFMLCanvas::size().width(),
                                     libreTex, obstTex, inicioTex, llegadaTex  );

    rvo->setupScenario( mapa->medidaReal2Pixel(radioReal+zonaSeguraReal),
                        mapa->obstaculos);

    emit IHaveWhatINeed( waitingDialog::Req_InfEnv );

    fullyLoaded = true;
}

void simulacion::draw()
{
    if(fullyLoaded)
    {
        mapa->drawMapa( this );

        for(agents::agent *agente : aManager->agentes)
            agente->draw(this);
    }
}

void simulacion::update(float deltaTime, float currentTime)
{   
    if(aManager->agentes.size()!=0)
    {
        rvo->updateVisualization();

        float delay = (rvo->sim->getGlobalTime()/100.f)-currentTime;
        //QThread::sleep( delay < 0 ? 0:delay );
    }
}

void simulacion::inicioDeLaSimulacion()
{
    reset_currentTime();
}
