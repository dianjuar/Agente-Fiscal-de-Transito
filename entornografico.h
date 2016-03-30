#ifndef ENTORNOGRAFICO_H
#define ENTORNOGRAFICO_H

#include "headers/qt_stuff.h"
#include "headers/sfml_stuff.h"
#include "headers/rvo_stuff.h"
#include "Tools/sfml.h"
#include "Tools/map.h"

#include "simulacion.h"


//--------protyping---------
class simulacion;

namespace entornoGrafico
{
    class mapa;

    namespace c
    {
        class cuadro;
        class obstaculo;
        class libre;
        class libreInalcansable;
        class inico;
        class fin;
    }
}
//--------protyping---------

namespace entornoGrafico
{
    class mapa
    {
        sf::Texture libreTexture,obstaculoTexture,inicioTexture,llegadaTexture, libreInalTexture;

        QString map_Str;
        std::vector< std::vector< c::cuadro* > > map;
        int nMap;


        ///
        /// \brief distancia_seguraObstaculo cuantos cuadros son seguros sin que el agente choque con un obstaculo
        ///
        static int distancia_seguraObstaculo;

        ///
        /// \brief haveCuadrosIncansables indica si es necesario verificar si un agente cabe dentro de un cuadro
        ///
        static bool haveCuadrosIncansables;

        void setup_map();
        void setup_scalesAndSizes(float canvasSize);

        ///
        /// \brief setup_unrecheableFreeSquares. Determine if is necesary to set up it.
        /// if is necesary, will set up the unreacheable fixed spaces of the anget and
        /// determine minimal square distance between and obstacle.
        ///
        void setup_secureDistance();

        ///
        /// \brief build_inalcansableSquare build the necesary squere. It will be unrecheable or a free squeare.
        /// \param matINT.                  The int mat.
        /// \param SPpoint                  Point of the sprite. The position in the screen.
        /// \param i
        /// \param j
        /// \return                         It will be unrecheable or a free squeare.
        ///
        c::cuadro *build_inalcansableSquare(std::vector< std::vector<int> > matINT, sf::Sprite SP_cuadro, sf::Vector2f SPpoint, int i, int j);

    public:

        ///
        /// \brief The ID_cuadros enum. ID's of the squares' map
        ///
        enum ID_cuadros {
                        ID_libre        = (int) 0,
                        ID_obstaculo    = (int) 1,
                        ID_inicio       = (int) 2,
                        ID_fin          = (int) 3,
                        ID_inalcansable = (int) 4
                    };

        ///
        /// \brief C_obstaculos lista de todos los obstaculos
        ///
        static std::vector< c::obstaculo* >             C_obstaculos;

        ///
        /// \brief C_libresIncalcansables lista de todos los libres inalcansables estaticos
        ///
        static std::vector< c::libreInalcansable* >     C_libresIncalcansables;

        ///
        /// \brief C_libes lista de todos los cuadros libres
        ///
        static std::vector< c::libre* >                 C_libres;

        ///
        /// \brief C_inicio cuadro de inicio
        ///
        static c::inico                                *C_inicio;

        ///
        /// \brief C_fin cuadro final
        ///
        static c::fin                                  *C_fin;

        mapa(QString map_Str, float map_longitudPorCuadro_REAL, float canvasSize,
             sf::Texture libreTexture, sf::Texture obstaculoTexture, sf::Texture inicioTexture, sf::Texture llegadaTexture, sf::Texture libreInalTex);

        static float spriteScale;
        static float spriteSize;
        static float spriteSizeOriginal;

        static sf::Vector2f llegada_Point;
        static sf::Vector2f inicio_Point;

        static float map_longitudPorCuadro_REAL;

        static float medidaReal2Pixel(float medidaReal);
        static float pixel2MedidaReal(float medidaPixel);

        static void set_CuadrosInalcalsables(float agentDiameter_REAL);

        void drawMapa(simulacion *render );
    };

    //******************** CUADROS ************************

    ///
    /// namespace contenedor de todos los cuadros
    ///
    namespace c
    {
        ///
        /// \brief Cuadro base.
        ///
        class cuadro: public sf::Sprite
        {
        public:
            int id;
            sf::Vector2f point;
            std::vector<sf::Vector2f> aristas;

            std::vector< RVO::Vector2 > getPoint_RVO();

            cuadro( int id, sf::Sprite sprt, sf::Vector2f point, float size = entornoGrafico::mapa::spriteSize);
        };

        ///
        /// \brief Cuadro obstáculo
        ///
        class obstaculo: public cuadro
        {
        public:
            obstaculo(int id, sf::Sprite sprt, sf::Vector2f point);
        };

        ///
        /// \brief Cuadro libbre
        ///
        class libre: public cuadro
        {
        public:
            libre(int id, sf::Sprite sprt, sf::Vector2f point);
        };

        ///
        /// \brief Cuadros libres los cuales el robot no puede alcanzar debio a que si lo hace choca con algo.
        ///
        class libreInalcansable: public libre
        {
        public:
            libreInalcansable(int id, sf::Sprite sprt, sf::Vector2f point);
        };

        ///
        /// \brief Cuadro de inicio
        ///
        class inico: public cuadro
        {
        public:
            inico(int id, sf::Sprite sprt, sf::Vector2f point);
        };

        ///
        /// \brief Cuadro fin
        ///
        class fin: public cuadro
        {
        public:
            fin(int id, sf::Sprite sprt, sf::Vector2f point);
        };

    }

}

#endif // ENTORNOGRAFICO_H
