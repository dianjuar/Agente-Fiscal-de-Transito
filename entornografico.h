#ifndef ENTORNOGRAFICO_H
#define ENTORNOGRAFICO_H

#include "headers/qt_stuff.h"
#include "headers/sfml_stuff.h"
#include "headers/rvo_stuff.h"
#include "Tools/sfml.h"

#include "mycanvas.h"


//--------protyping---------
class MyCanvas;

namespace entornoGrafico
{
    class mapa;
    class obstaculo;
}
//--------protyping---------

namespace entornoGrafico
{
    class mapa
    {
        enum cuadro {
                        libre     = (int) 0,
                        obstaculo = (int) 1
                    };

        sf::Texture libreTexture,obstaculoTexture;

        QString map_Str;
        std::vector< std::vector<sf::Sprite> > map_Spr;
        std::vector< std::vector<int> > map_Int;

        int nMap;

        void setup_map();
        void setup_scalesAndSizes(float canvasSize);

    public:

        static std::vector< entornoGrafico::obstaculo > obstaculos;

        mapa(QString map_Str, float map_longitudPorCuadro_REAL, float canvasSize, sf::Texture libreTexture, sf::Texture obstaculoTexture);

        static float spriteScale;
        static float spriteSize;
        static float spriteSizeOriginal;

        static float map_longitudPorCuadro_REAL;

        static float medidaReal2Pixel(float medidaReal);
        static float pixel2MedidaReal(float medidaPixel);

        void drawMapa(MyCanvas *render );
    };

    class obstaculo
    {

    public:
        std::vector<sf::Vector2f> pointsSF;

        obstaculo(sf::Vector2f point, float size);

        std::vector< RVO::Vector2 > getPoint_RVO();
    };

}

#endif // ENTORNOGRAFICO_H
