#include "agentpath.h"

using namespace agents;

float agentPath::radio;

agentPath::agentPath(diryStep *A,  diryStep *B):
    A(A),
    B(B),    
    distancia(tools::math::distanceBetween2Points( A->position, B->position ))
{
    radio = agent::radius+agent::zonaSegura*0.85;

    setFillColor( sf::Color(0,0,0,128) );
    setPosition( A->position.x(), A->position.y() );

    setSize(sf::Vector2f(distancia + radio*2,
                         radio*2));
    setOrigin( radio, radio  );


    //calculo del angulo
    rect = new tools::math::lineaRecta( tools::sfml::rvoVector2_2_sfVector2f(A->position),
                                        tools::sfml::rvoVector2_2_sfVector2f(B->position));


    giro = (tools::math::cuadranteDeUnPunto( A->position, B->position ) == tools::math::Cuadrante_II ||
            tools::math::cuadranteDeUnPunto( A->position, B->position ) == tools::math::Cuadrante_III ? 0:180 ) - tools::math::lineaRecta::anguloEntre2Rectas( tools::math::lineaRecta::ejeX(), *rect, false  );

    setRotation(giro);
}

bool agentPath::collideWith(agentPath b)
{
    return this->getGlobalBounds().intersects(b.getGlobalBounds());
}

bool agentPath::areInTheSameTime(agentPath p)
{
    double StartA = this->A->time;
    double EndA   = this->B->time;
    double StartB = p.A->time;
    double EndB   = p.B->time;

     //(StartA <= EndB) and (EndA >= StartB)
     if((StartA <= EndB) && (EndA >= StartB))
     {
         return true;
     }

     return false;
}

bool agentPath::pathAreInTheSameTime(agentPath a, agentPath b)
{
    return a.areInTheSameTime(b);
}

bool agentPath::pathCollideWith(agentPath a, agentPath b)
{
    return a.collideWith(b);
}

bool agentPath::operator==(const agentPath &other)
{
    return this->A == other.A &&
            this->B == other.B;
}


