#include "map.h"

std::vector<std::vector<int> > tools::map::QStringMat2IntMat(QString map_Str, int nMap)
{
    std::vector< std::vector<int> > map_Int;

    for (int i = 0; i < nMap; ++i)
    {
        QString QStrRow =  map_Str.split("\n", QString::SkipEmptyParts ).at(i);
        std::vector<int> rowVecInt;

        for (int j = 0; j < nMap; ++j)
        {
           int val = QStrRow.at(j).digitValue();
           rowVecInt.push_back( val );
        }

        map_Int.push_back(rowVecInt);
    }

    return map_Int;
}
