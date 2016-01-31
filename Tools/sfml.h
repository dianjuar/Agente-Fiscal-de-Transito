#ifndef SFML_H
#define SFML_H

#include "headers/qt_stuff.h"
#include "headers/sfml_stuff.h"
#include "headers/rvo_stuff.h"

namespace tools
{

namespace sfml
{
    bool loadTexture(sf::Texture &texture, QString path);
    RVO::Vector2 sfVector2f_2_rvoVector2( sf::Vector2f );
    sf::Vector2f rvoVector2_2_sfVector2f( RVO::Vector2 );
}

}
#endif // SFML_H
