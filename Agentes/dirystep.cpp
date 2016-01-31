#include "dirystep.h"

using namespace agents;

diryStep::diryStep(RVO::Vector2 position, RVO::Vector2 velocity, double time, int index):
    position(position),
    velocity(velocity),
    time(time),
    index(index)
{

}

bool diryStep::operator==(const diryStep &other)
{
    return this->position == other.position &&
           this->time == other.time;
}
