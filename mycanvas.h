#ifndef MYCANVAS_H
#define MYCANVAS_H

#include "qsfmlcanvas.h"
#include "Tools/sfml.h"
#include "entornografico.h"
#include "RVO/rvo_manager.h"

#include "Agentes/agentmanager.h"
#include "network/connection_sma.h"



//--------protyping---------
class MyCanvas;
class RVO_Manager;


namespace agents
{
    class agentManager;
}

namespace entornoGrafico
{
    class mapa;
    class obstaculo;
}
//--------protyping---------

class MyCanvas : public QSFMLCanvas
{
    Q_OBJECT

    network::connections::SMA *connection_SMA;

public :
    MyCanvas(QWidget* Parent, const QPoint& Position, const QSize& Size);

    void OnInit();
    void draw();
    void update(float deltaTime, float currentTime);

    int numeroMaximoPath;

public slots:
    void inicioDeLaSimulacion();

private :

    entornoGrafico::mapa *mapa;

    agents::agentManager *aManager;

    float map_longitudPorCuadro_REAL;
    float radioReal, radioScaled;
    float zonaSeguraReal, zonaSeguraScaled;

    float tiempoTranscurrido;

    int numeroPath;
    bool printed_V_MIN_MAX;

    RVO_Manager *rvo;

    void setup_agentes();
   // void drawCleanSteps();
};

#endif // MYCANVAS_H
