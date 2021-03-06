#include "pcconnection.h"
#include "connectioncontainer.h"

PCConnection::PCConnection(NetworkSocket *_socket, std::string user, std::string pass)
    : BaseConnection(_socket, TYPE_PC, user, pass)
{

}


void PCConnection::redistributePacket(Packet *packet)
{
    // TODO
    // rewrite request id handling

    std::vector<AndroidConnection*> conns = sConnections.getAndroidConnections(this);
    if (conns.empty())
        return;

    for (std::vector<AndroidConnection*>::iterator itr = conns.begin(); itr != conns.end(); ++itr)
    {
        (*itr)->getSocket()->send(*packet);
    }
}

bool PCConnection::handlePacket(Packet */*packet*/)
{
    return false;
}
