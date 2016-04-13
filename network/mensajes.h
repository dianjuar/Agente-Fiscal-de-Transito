#ifndef MENSAJES
#define MENSAJES

#include "QString"

namespace network
{
    namespace mensajes
    {
        //Mensajes Standar
        const QString divisor =   "->";
        const QString divisor_2 = "_";
        const QString divisor_3 = "#";
        const QString cerrar =    "close";
        const QString conectado = "connect";

        //Mensajes To SMA
        const QString MeToSMA_Velocidades = "VelLR";
        const QString MeToSMA_Rotation = "Rot";
        const QString MeToSMA_CDT = "CDT";
        const QString SMAtoMe_CDT_Terminada = "CDTf";

        //Mensajes From SMA
        const QString SMAtoMe_ExistenseNotification = "here";

        //---------------------------------------

        //Mensajes from ACO
        const QString ACOtoMe_EnviromentInfo = "EnvInfo";
        const QString ACOtoMe_NextStep = "nextS";
        const QString ACOtoMe_setVelocidad = "SVel";

        //Mensaje to ACO
        const QString MeToACO_SiguientePaso = "nextS";
        const QString MeToACO_UnrecheableSteps = "unrSteps";
        const QString MeToACO_NONEUnrecheableSteps = "NONEunrSteps";
    }
}

#endif // MENSAJES

