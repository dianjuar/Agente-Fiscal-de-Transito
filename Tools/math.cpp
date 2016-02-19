#include "math.h"

using namespace tools;

double math::distanceBetween2Points(RVO::Vector2 A, RVO::Vector2 B)
{
    //sqrt( qPow(A.x - B.x, 2) + qPow(A.y - B.y, 2) );
    return qPow( qPow(A.x() - B.x(), 2) + qPow(A.y() - B.y(), 2) , 0.5f);
}

double math::distanceBetween2Points(sf::Vector2f A, sf::Vector2f B)
{
    //sqrt( qPow(A.x - B.x, 2) + qPow(A.y - B.y, 2) );
    return qPow( qPow(A.x - B.x, 2) + qPow(A.y - B.y, 2) , 0.5f);
}

int math::cuadranteDeUnPunto(sf::Vector2f p, sf::Vector2f reference)
{
    p = p-reference;

    if( p.x >= 0 && p.y >= 0)
        return math::Cuadrante_I;

    if( p.x <= 0 && p.y >= 0)
        return math::Cuadrante_II;

    if( p.x <= 0 && p.y <= 0)
        return math::Cuadrante_III;

    return Cuadrante_IV;
}

int math::cuadranteDeUnPunto(RVO::Vector2 p, RVO::Vector2 reference)
{
    return math::cuadranteDeUnPunto( tools::sfml::rvoVector2_2_sfVector2f(p),
                                     tools::sfml::rvoVector2_2_sfVector2f(reference) );
}

/////////////////////////////////////////////

void math::printMat(cv::Mat m)
{
    qDebug()<<"....";
    for(int i=0; i < m.rows; i++)
    {
        QString p;
        for (int j = 0; j < m.cols; ++j)
        {
            p+=QString::number(m.at<float>(i,j))+"\t";
        }

        qDebug()<<p;
    }
    qDebug()<<"....";
}


int math::pasosParaLlegarA(int mirada, int miradaNueva, bool horario)
{
    int cont = 0;

    if(horario)
        for (int i = 0; i < 8; i++)
        {
            if(mirada == miradaNueva)
                break;

            mirada++;
            cont++;

            if(mirada == 8)
                mirada=0;
        }
    else
        for (int i = 8; i >= 0; i--)
        {
            if(mirada == miradaNueva)
                break;

            mirada--;
            cont++;

            if(mirada == -1)
                mirada = 7;
        }

    return cont;
}


int math::cuantosGradosGiraryHaciaDonde(int mirada, int miradaNueva)
{
    int grados = 0;

    if(mirada != miradaNueva)
    {
        int pasosH = pasosParaLlegarA(mirada, miradaNueva, true);
        int pasosAH = pasosParaLlegarA(mirada, miradaNueva, false);

        int pasos = pasosH < pasosAH ? pasosH:pasosAH;

        grados = pasos*45  * (pasosH < pasosAH ? -1:1);

    }

    return grados;
}
