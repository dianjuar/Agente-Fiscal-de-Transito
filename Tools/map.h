#ifndef MAP_H
#define MAP_H

#include "headers/qt_stuff.h"


namespace tools
{
    namespace map
    {
        std::vector< std::vector<int> > QStringMat2IntMat(QString map_Str , int nMap);
    }
}
#endif // MAP_H
