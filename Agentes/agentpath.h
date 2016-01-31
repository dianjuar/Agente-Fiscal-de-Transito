#ifndef AGENTPATH_H
#define AGENTPATH_H

#include "headers/sfml_stuff.h"
#include "Tools/math.h"
#include "Agentes/agent.h"
#include "Agentes/dirystep.h"
#include "entornografico.h"

//----------prototyping---------------
namespace tools
{
    namespace math
    {
        class lineaRecta;
    }
}
//----------prototyping---------------

namespace agents
{

class agentPath: public sf::RectangleShape
{
    static float radio;

    tools::math::lineaRecta *rect;
    float distancia;
    float giro;

public:    
    diryStep *A;
    diryStep *B;

    agentPath( diryStep *A,  diryStep *B);

    bool areInTheSameTime(agentPath p );
    static bool pathAreInTheSameTime(agentPath a, agentPath b);

    bool collideWith( agentPath b );
    static bool pathCollideWith(agentPath a, agentPath b );

    bool operator==(const agentPath& other);

};

}

#endif // AGENTPATH_H
