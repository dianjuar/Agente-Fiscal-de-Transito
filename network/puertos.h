#ifndef PUERTOS_H
#define PUERTOS_H

namespace network
{
    namespace connections
    {

        namespace puertos
        {
            ///
            /// \brief Recibe_SMA - puerto por donde recibirá la información del SMA.
            ///
            int Recibe_SMA = 5001;

            ///
            /// \brief ACO - puerto por donde se conectará al ACO.
            ///
            int ACO = 5001;
        }
    }
}

#endif // PUERTOS_H
