#ifndef DIRYSTEP_H
#define DIRYSTEP_H

#include "headers/qt_stuff.h"
#include "headers/rvo_stuff.h"
#include "headers/sfml_stuff.h"

namespace agents
{

class diryStep
{
public:
    int index;
    double time;
    RVO::Vector2 position;
    RVO::Vector2 velocity;

    diryStep(RVO::Vector2 position, RVO::Vector2 velocity,double time, int index);

    bool operator==(const diryStep& other);
};

}
#endif // DIRYSTEP_H
