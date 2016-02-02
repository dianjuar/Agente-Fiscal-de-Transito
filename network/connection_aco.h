#ifndef ACO_H
#define ACO_H

#include "network/MultiPropuseSockets.h"

namespace network
{
    namespace connections
    {

    class ACO: public ::network::Client
    {
    public:
        ACO(QString add);
    };

    }
}

#endif // ACO_H
