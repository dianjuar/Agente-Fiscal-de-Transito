#ifndef MATH_H
#define MATH_H

#include "headers/qt_stuff.h"
#include "headers/sfml_stuff.h"
#include "headers/rvo_stuff.h"
#include "headers/opencv_stuff.h"

#include "Tools/sfml.h"
#include "entornografico.h"
#include "math.h"

//----------prototyping---------------
namespace entornoGrafico
{
    class obstaculo;
}
//----------prototyping---------------

namespace tools
{
    namespace math
    {
        double distanceBetween2Points( RVO::Vector2 A,
                                       RVO::Vector2 B);
        double distanceBetween2Points( sf::Vector2f A,
                                       sf::Vector2f B);

        int cuadranteDeUnPunto(sf::Vector2f p, sf::Vector2f reference);
        int cuadranteDeUnPunto(RVO::Vector2 p, RVO::Vector2 reference);
        void printMat(cv::Mat);

        int pasosParaLlegarA(int mirada, int miradaNueva, bool horario);
        int cuantosGradosGiraryHaciaDonde(int mirada, int miradaNueva);


        enum
        {
            Cuadrante_I     = 1,
            Cuadrante_II    = 2,
            Cuadrante_III   = 3,
            Cuadrante_IV    = 4
        };
    }


}

#endif // MATH_H
