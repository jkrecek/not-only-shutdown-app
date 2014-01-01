#ifndef ANDROIDCONNECTION_H
#define ANDROIDCONNECTION_H

#include "baseconnection.h"

class AndroidConnection : public BaseConnection
{
public:
    AndroidConnection(NetworkSocket* _socket);

    void redistributePacket(Packet* packet);
};

#endif // ANDROIDCONNECTION_H
