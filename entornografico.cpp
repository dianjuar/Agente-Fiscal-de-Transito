#include "entornografico.h"

using namespace entornoGrafico;

float mapa::spriteScale                 = 0;
float mapa::spriteSize                  = 0;
float mapa::spriteSizeOriginal          = 0;

float mapa::map_longitudPorCuadro_REAL  = 0;

std::vector< entornoGrafico::obstaculo > mapa::obstaculos;

mapa::mapa(QString map_Str,
           float map_longitudPorCuadro_REAL,
           float canvasSize,
           sf::Texture libreTexture, sf::Texture obstaculoTexture)
{
    this->map_Str = map_Str;
    this->map_longitudPorCuadro_REAL = map_longitudPorCuadro_REAL;

    nMap = map_Str.split("\n", QString::SkipEmptyParts ).size();

    this->libreTexture = libreTexture;
    this->obstaculoTexture = obstaculoTexture;

    setup_scalesAndSizes(canvasSize);
    setup_map();
}

float mapa::medidaReal2Pixel(float medidaReal)
{
    return spriteSize*medidaReal/map_longitudPorCuadro_REAL;
}

float mapa::pixel2MedidaReal(float medidaPixel)
{
    return medidaPixel*map_longitudPorCuadro_REAL/spriteSize;
}

void mapa::drawMapa( MyCanvas *render )
{
    for (int i = 0; i < nMap; ++i)
        for (int j = 0; j < nMap; ++j)
            render->RenderWindow::draw(map_Spr[i][j]);
}

void mapa::setup_scalesAndSizes(float canvasSize)
{
    this->spriteSizeOriginal = libreTexture.getSize().x;
    spriteSize = canvasSize/nMap;
    spriteScale = spriteSize/spriteSizeOriginal; //regla de 3
}

void mapa::setup_map()
{
    for (int i = 0; i < nMap; ++i)
    {
        QString QStrRow =  map_Str.split("\n", QString::SkipEmptyParts ).at(i);
        std::vector<int> rowVecInt;
        std::vector<sf::Sprite> rowMap_Spr;

        for (int j = 0; j < nMap; ++j)
        {
            int val = QStrRow.at(j).digitValue();
            rowVecInt.push_back( val );

            sf::Sprite cuadro;
            //cuadro.setPosition( j*spriteSize , i*spriteSize );
            cuadro.setScale( spriteScale,spriteScale );

            switch (val)
            {
                case obstaculo:
                    cuadro.setTexture(obstaculoTexture);
                    obstaculos.push_back( entornoGrafico::obstaculo( sf::Vector2f(j*spriteSize,i*spriteSize),
                                                                     spriteSize) );

                    cuadro.setPosition( sf::Vector2f(j*spriteSize,i*spriteSize));
                break;

                case libre:
                    cuadro.setTexture(libreTexture);
                    cuadro.setPosition( j*spriteSize , i*spriteSize );
                break;
            }

            rowMap_Spr.push_back( cuadro );
        }

        map_Spr.push_back(rowMap_Spr);
        map_Int.push_back(rowVecInt);
    }
}

//////////////////////////////////////////////////////////
obstaculo::obstaculo(sf::Vector2f point, float size)
{
    /*
     * Add (polygonal) obstacles, specifying their vertices in counterclockwise
     * order.
     */

    pointsSF.push_back( point );
    pointsSF.push_back( sf::Vector2f( point.x + size, point.y ) );
    pointsSF.push_back( sf::Vector2f( point.x + size, point.y + size) );
    pointsSF.push_back( sf::Vector2f( point.x, point.y + size ) );
}

std::vector<RVO::Vector2> obstaculo::getPoint_RVO()
{
    std::vector< RVO::Vector2 >pointsRVO;

    for (int i = 0; i < pointsSF.size(); ++i)
        pointsRVO.push_back( tools::sfml::sfVector2f_2_rvoVector2( pointsSF[i] ) );

    return pointsRVO;
}
