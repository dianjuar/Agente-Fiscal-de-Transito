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
        const QString cerrar =    "close";
        const QString conectado = "connect";

        //Mensajes To SMA
        const QString MeToSMA_Velocidades = "VelLR";
        const QString MeToSMA_Rotation = "Rot";

        //Mensajes From SMA
        const QString SMAtoMe_ExistenseNotification = "here";

        //---------------------------------------

        //Mensajes from ACO
        const QString ACOtoMe_EnviromentInfo = "EnvInfo";
        const QString ACOtoMe_NextStep = "nextS";

        //Mensaje to ACO
        const QString meToACO_SiguientePaso = "nextS";
    }
}

#endif // MENSAJES

