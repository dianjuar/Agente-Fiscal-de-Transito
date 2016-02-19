#ifndef MYCANVAS_H
#define MYCANVAS_H

#include "qsfmlcanvas.h"
#include "Tools/sfml.h"
#include "entornografico.h"
#include "waitingdialog.h"
#include "RVO/rvo_manager.h"

#include "Agentes/agentmanager.h"
#include "network/connection_sma.h"


//--------protyping---------
class simulacion;
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

class simulacion : public QSFMLCanvas
{
    Q_OBJECT

public:
    simulacion(QWidget* Parent, const QPoint& Position, const QSize& Size,
               QString map, float dist,
               network::connections::SMA *sma, network::connections::ACO *aco);

    void OnInit();
    void draw();
    void update(float deltaTime, float currentTime);

    int numeroMaximoPath;

    void setInformacionGrafica(QString map, float dist);

signals:
    void IHaveWhatINeed(int nReq);

public slots:
    void inicioDeLaSimulacion();

private :
    sf::Texture libreTex, obstTex, inicioTex, llegadaTex;

    network::connections::SMA *sma;
    network::connections::ACO *aco;

    entornoGrafico::mapa *mapa;

    agents::agentManager *aManager;

    QString map;
    float dist;

    bool fullyLoaded;

    float map_longitudPorCuadro_REAL;
    float radioReal, radioScaled;
    float zonaSeguraReal, zonaSeguraScaled;

    float tiempoTranscurrido;

    int numeroPath;
    bool printed_V_MIN_MAX;

    RVO_Manager *rvo;
};

#endif // MYCANVAS_H
