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



////////////////////////////////////////////////////////////////////////////////

void math::lineaRecta::calcularDistancia()
{
    distanciaDelaRecta = distanceBetween2Points(A,B);
}

void math::lineaRecta::OrganizarRectas(math::lineaRecta &R1, math::lineaRecta &R2)
{
    float m1 = R1.m;
    float m2 = R2.m;

    if( m1 >= 0 && m2 >=0 ) //primer caso que ambas sean positivas, m2 será el mayor
    {
        if( m1 > m2 )
        {
            math::lineaRecta aux = R2;
            R2 = R1;
            R1 = aux;
        }
        return;
    }
    else
    {
        if( ( m1 < 0 && m2 > 0 ) || ( m1 > 0 && m2 < 0 ) ) //segundo caso que una sea positiva y otra negativa, m2 será el negativo
        {
            if(m1 < 0)
            {
                math::lineaRecta aux = R2;
                R2 = R1;
                R1 = aux;
            }
            return;
        }
        else
        {
            if( m1 < 0 && m2 < 0 ) //tercer caso que ambas sean negativas, m2 será el que tenga el menos valor absoluto
            {
                if( abs(m1) < abs(m2) )
                {
                    math::lineaRecta aux = R2;
                    R2 = R1;
                    R1 = aux;
                }
            }
        }//else
    }//else
}

math::lineaRecta math::lineaRecta::ejeX()
{
    return lineaRecta(0,0);
}

math::lineaRecta math::lineaRecta::ejeX(sf::Vector2f puntoMedio)
{
    //Point(0,rectaRobot.puntoMedio.y),
    //Point(1000,rectaRobot.puntoMedio.y)
    int distancia = 40;
    return lineaRecta(sf::Vector2f(puntoMedio.x - distancia, puntoMedio.y),
                      sf::Vector2f(puntoMedio.x + distancia, puntoMedio.y));
}

bool math::lineaRecta::isRectaR1( math::lineaRecta Recta, math::lineaRecta R1, math::lineaRecta R2)
{
    OrganizarRectas(R1,R2);

    return Recta == R1;
}

math::lineaRecta::lineaRecta(float m, float b, sf::Vector2f A, sf::Vector2f B)
{
    this->m = m;
    this->b = b;
    this->A = A;
    this->B = B;

    puntoMedio = sf::Vector2f( (A.x+B.x)/2 , (A.y+B.y)/2 );
    calcularDistancia();
}

math::lineaRecta::lineaRecta(float m, float b)
{
    this->m = m;
    this->b = b;
}

math::lineaRecta::lineaRecta(sf::Vector2f A, sf::Vector2f B)
{
    this->A = A;
    this->B = B;

    m = (float)(this->B.y*-1 - this->A.y*-1)/(this->B.x - this->A.x);

        if( abs(m) == std::numeric_limits<float>::infinity())
        {
            m = std::numeric_limits<int>::max();
            b = -1* std::numeric_limits<int>::max();
        }

    b = (float)(this->A.y*-1 - this->A.x*m);

    puntoMedio = sf::Vector2f( (this->A.x+this->B.x)/2 , (this->A.y+this->B.y)/2 );
    calcularDistancia();
}

math::lineaRecta::lineaRecta()
{
    this->m = std::numeric_limits<float>::min();
    this->b = std::numeric_limits<float>::min();

    this->A = sf::Vector2f( std::numeric_limits<int>::min(),
                     std::numeric_limits<int>::min() );

    this->B  = sf::Vector2f( std::numeric_limits<int>::min(),
                      std::numeric_limits<int>::min() );
}

float math::lineaRecta::puntoEnY(float puntoX)
{
    return m*puntoX + b;
}

float math::lineaRecta::puntoEnX(float puntoY)
{
    return (puntoY-b) / m;
}

float math::lineaRecta::anguloEntre2Rectas(lineaRecta lA, lineaRecta lB,
                                           bool radianes,
                                           bool dibujar)
{
    //siempre R1 será la recta del robot
    math::lineaRecta rectaRobot = lA;
    math::lineaRecta rectaDestino = lB;

    OrganizarRectas(lA,lB);

    /*if(tools::general::DEBUG)
    {
        qDebug()<<"------------------------";
        qDebug()<<"m1"<<lA.m<<" - "<<"m2"<<lB.m;
        qDebug()<<"------------------------";
    }*/

    float m1 = lA.m;
    float m2 = lB.m;

    /*if( general::DEBUG &&
        (lA.m == std::numeric_limits<int>::max() || lB.m == std::numeric_limits<int>::max()) )
        qDebug()<<"infinito";*/

    long double tangTeta = ((long double)(m2-m1))/((long double)(1.0+m2*m1));

    float teta = ( atan( tangTeta )*(180.0/M_PI) );

    return teta/( radianes ? 180.0/M_PI:1)*(rectaRobot==lA ? 1:-1);
}

bool math::lineaRecta::isM_positivo()
{
    return m > 0;
}

bool math::lineaRecta::operator==(const math::lineaRecta &other)
{
    return this->m == other.m &&
           this->b == other.b &&
           this->A == other.A &&
           this->B == other.B;
}

void math::lineaRecta::operator=(const math::lineaRecta &other)
{
    this->A = other.A;
    this->B= other.B;
    this->puntoMedio = other.puntoMedio;

    this->b = other.b;
    this->m = other.m;
    this->distanciaDelaRecta = other.distanciaDelaRecta;
}


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
