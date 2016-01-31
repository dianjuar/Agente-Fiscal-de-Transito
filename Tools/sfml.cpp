#include "sfml.h"
#include <map>

using namespace tools;

bool sfml::loadTexture(sf::Texture &texture, QString path)
{
    if (!texture.loadFromFile(path.toUtf8().data()))
    {
        qDebug() << "Loading error";
        return false;
    }
    else
    {
        qDebug() << "Image "<<path<<" was loaded fine";
        return true;
    }
}

RVO::Vector2 sfml::sfVector2f_2_rvoVector2(sf::Vector2f v)
{
    return RVO::Vector2( v.x, v.y );
}

sf::Vector2f sfml::rvoVector2_2_sfVector2f(RVO::Vector2 v)
{
    return sf::Vector2f( v.x(), v.y() );
}

////////////////////////////////////////////////////////////////
