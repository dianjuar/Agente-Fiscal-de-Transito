#ifndef MATH_H
#define MATH_H

#include "headers/qt_stuff.h"
#include "headers/sfml_stuff.h"
#include "headers/rvo_stuff.h"
#include "headers/opencv_stuff.h"

#include "Tools/sfml.h"
#include "Agentes/agentpath.h"
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
        enum
        {
            Cuadrante_I     = 1,
            Cuadrante_II    = 2,
            Cuadrante_III   = 3,
            Cuadrante_IV    = 4
        };

        ////////////////////////////////////////////
        /// \brief The lineaRecta class
        ///
        class lineaRecta
        {
            void calcularDistancia();

            //para el calculo del angulo entre las 2 rectas es necesario ordenar primero las rectas.
        public:
            lineaRecta(float m, float b, sf::Vector2f A, sf::Vector2f B);
            lineaRecta(float m, float b);
            //lineaRecta(float m, float b, );
            lineaRecta(sf::Vector2f A, sf::Vector2f B);
            lineaRecta();

            float m;
            float b;
            float distanciaDelaRecta;
            sf::Vector2f puntoMedio;
            sf::Vector2f A,B;

            float puntoEnY(float puntoX);
            float puntoEnX(float puntoY);

            static void OrganizarRectas(lineaRecta &R1, lineaRecta &R2);
            static lineaRecta ejeX();
            static lineaRecta ejeX(sf::Vector2f puntoMedio);
            static bool isRectaR1(lineaRecta Recta ,lineaRecta const R1, lineaRecta const R2 );

            static float anguloEntre2Rectas(lineaRecta lA, lineaRecta lB,
                                            bool radianes=true,
                                            bool dibujar=false);

            bool isM_positivo();

            bool operator==(const lineaRecta& other);
            void operator=(const lineaRecta& other);
        };
    }


}

#endif // MATH_H
