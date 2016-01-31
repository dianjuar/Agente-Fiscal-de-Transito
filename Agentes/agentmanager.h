#ifndef AGENTMANAGER_H
#define AGENTMANAGER_H

#include "headers/qt_stuff.h"
#include "headers/rvo_stuff.h"
#include "headers/sfml_stuff.h"
#include "Agentes/agent.h"
#include "mycanvas.h"


namespace agents
{

class agentManager : public QObject
{
    Q_OBJECT

public:
    explicit agentManager(QObject *parent = 0);

    std::vector<agent*> agentes;
    void addAgent( agent *a );

signals:

public slots:
    //void buildDiferencialAgentsPath();
};

}

#endif // AGENTMANAGER_H
