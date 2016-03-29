#include "entornografico.h"

using namespace entornoGrafico;

float mapa::spriteScale                 = 0;
float mapa::spriteSize                  = 0;
float mapa::spriteSizeOriginal          = 0;
float mapa::map_longitudPorCuadro_REAL  = 0;
bool  mapa::haveCuadrosIncansables      = false;
int   mapa::distancia_seguraObstaculo   = 0;


std::vector< entornoGrafico::c::obstaculo* >            mapa::C_obstaculos;
std::vector< entornoGrafico::c::libreInalcansable* >    mapa::C_libresIncalcansables;
std::vector< entornoGrafico::c::libre* >                mapa::C_libres;
c::inico *mapa::C_inicio;
c::fin   *mapa::C_fin;


sf::Vector2f mapa::inicio_Point     = sf::Vector2f(0,0);
sf::Vector2f mapa::llegada_Point    = sf::Vector2f(0,0);

mapa::mapa(QString map_Str,
           float map_longitudPorCuadro_REAL,
           float canvasSize,
           sf::Texture libreTexture, sf::Texture obstaculoTexture, sf::Texture inicioTexture, sf::Texture llegadaTexture)
{
    this->map_Str = map_Str;
    this->map_longitudPorCuadro_REAL = map_longitudPorCuadro_REAL;

    nMap = map_Str.split("\n", QString::SkipEmptyParts ).size();

    this->libreTexture = libreTexture;
    this->obstaculoTexture = obstaculoTexture;
    this->inicioTexture = inicioTexture;
    this->llegadaTexture = llegadaTexture;

    setup_scalesAndSizes(canvasSize);
    setup_map();

    distancia_seguraObstaculo = 0;
}

float mapa::medidaReal2Pixel(float medidaReal)
{
    return spriteSize*medidaReal/map_longitudPorCuadro_REAL;
}

float mapa::pixel2MedidaReal(float medidaPixel)
{
    return medidaPixel*map_longitudPorCuadro_REAL/spriteSize;
}

void mapa::set_CuadrosInalcalsables(float agentDiameter_REAL)
{
    if( map_longitudPorCuadro_REAL < agentDiameter_REAL )
    {
        haveCuadrosIncansables = true;
        distancia_seguraObstaculo = (int) (agentDiameter_REAL/map_longitudPorCuadro_REAL);
    }
}

void mapa::drawMapa( simulacion *render )
{
    for (int i = 0; i < nMap; ++i)
        for (int j = 0; j < nMap; ++j)
        {
            sf::Sprite *cuadro = dynamic_cast<sf::Sprite*> ( map[i][j] ) ;
            render->RenderWindow::draw( *cuadro );
        }
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
        std::vector< c::cuadro* > rowMap_cuadro;

        for (int j = 0; j < nMap; ++j)
        {
            int ID_cuado = QStrRow.at(j).digitValue();

            sf::Sprite SP_cuadro;
            SP_cuadro.setScale( spriteScale,spriteScale );
            SP_cuadro.setPosition( j*spriteSize , i*spriteSize );

            sf::Vector2f point =  sf::Vector2f(j*spriteSize,i*spriteSize);

            c::cuadro *cuadro;


            switch (ID_cuado)
            {
                case ID_obstaculo:
                    SP_cuadro.setTexture(obstaculoTexture);

                    cuadro = new c::obstaculo( ID_obstaculo, SP_cuadro, point, spriteSize  );
                    C_obstaculos.push_back(  dynamic_cast< c::obstaculo* >( cuadro ) );

                break;

                case ID_libre:
                    SP_cuadro.setTexture(libreTexture);

                    cuadro = new c::obstaculo( ID_libre, SP_cuadro, point, spriteSize  );
                    C_libres.push_back( dynamic_cast< c::libre* >( cuadro ) );
                break;

                case ID_inicio:
                    SP_cuadro.setTexture(inicioTexture);

                    cuadro = new c::inico( ID_inicio, SP_cuadro, point, spriteSize );
                    C_inicio = dynamic_cast< c::inico* >( cuadro );
                break;

                case ID_fin:
                    SP_cuadro.setTexture(llegadaTexture);

                    cuadro = new c::fin( ID_fin, SP_cuadro, point, spriteSize );
                    C_fin = dynamic_cast< c::fin* >( cuadro );
                break;
            }

            rowMap_cuadro.push_back( cuadro );
        }

        map.push_back(rowMap_cuadro);
    }
}

//////////////////////////////////////////////////////////
std::vector<RVO::Vector2> c::cuadro::getPoint_RVO()
{
    std::vector< RVO::Vector2 >pointsRVO;

    for (int i = 0; i < aristas.size(); ++i)
        pointsRVO.push_back( tools::sfml::sfVector2f_2_rvoVector2( aristas[i] ) );

    return pointsRVO;
}

c::cuadro::cuadro(int id, sf::Sprite sprt, sf::Vector2f point, float size):
    sf::Sprite( sprt ),
    point(point),
    id(id)
{
    /*
     * Add (polygonal) obstacles, specifying their vertices in counterclockwise
     * order.
     */
    aristas.push_back( point );
    aristas.push_back( sf::Vector2f( point.x + size, point.y ) );
    aristas.push_back( sf::Vector2f( point.x + size, point.y + size) );
    aristas.push_back( sf::Vector2f( point.x, point.y + size ) );
}

///////////////////
c::obstaculo::obstaculo(int id, sf::Sprite sprt, sf::Vector2f point, float size):
    cuadro(id, sprt,point,size)
{
}

c::libre::libre(int id, sf::Sprite sprt, sf::Vector2f point, float size):
    cuadro(id, sprt,point,size)
{
}

c::libreInalcansable::libreInalcansable(int id, sf::Sprite sprt, sf::Vector2f point, float size):
    cuadro(id, sprt,point,size)
{
}

c::inico::inico(int id, sf::Sprite sprt, sf::Vector2f point, float size):
    cuadro(id, sprt,point,size)
{
}

c::fin::fin(int id, sf::Sprite sprt, sf::Vector2f point, float size):
    cuadro(id, sprt,point,size)
{
}

