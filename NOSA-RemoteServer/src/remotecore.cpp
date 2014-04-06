#include "remotecore.h"
#include <iostream>
#include "helper.h"
#include "connectioncontainer.h"
#include <unistd.h>
#include "socketclosedexception.h"

#ifndef _WIN32
    #include <pthread.h>
    #include <sys/socket.h>
    #include <netdb.h>
#endif

RemoteCore::RemoteCore(int port)
    : BaseCore(), server(NULL)
{
    if (!prepareSockets())
        return;

    server = ServerSocket::createSocket(port);
}

RemoteCore::~RemoteCore()
{
    if (!sConnections.empty())
        for(ConnectionContainer::iterator itr = sConnections.begin(); itr.hasMore(); itr.next())
            delete itr.get();

    delete server;
}

int RemoteCore::run()
{
    BaseConnection* connection = NULL;
    while (server && server->isOpen())
    {
        NetworkSocket* socket = server->acceptConnection();
        Packet firstPacket = socket->readPacket();
        SocketPacket soPa(socket, &firstPacket);
        try
        {
            connection = BaseConnection::estabilishConnection(&soPa);
        }
        catch (SocketClosedException& /*e*/)
        {
            std::cout << "Exception occured in estabilishing connection. Socket is beeing closed." << std::endl;
            if (connection)
            {
                delete connection;
                connection = NULL;
            }
        }

        if (!connection)
        {
            delete socket;
            continue;
        }
        else
        {
            soPa.respond("CONNECTED");
        }

        sConnections.insert(connection);

        startThread(connection);
    }

    return 0;
}

void RemoteCore::startThread(BaseConnection *connection)
{
#ifdef _WIN32
    _beginthreadex(NULL, 0, RemoteCore::handleConnection, connection, 0, NULL);
#else
    pthread_t t;
    pthread_create(&t, NULL, RemoteCore::handleConnection, connection);
#endif
}

THREAD_RETURN_TYPE RemoteCore::handleConnection(void* data)
{
    BaseConnection* connection = static_cast<BaseConnection*>(data);
    connection->read();
    delete connection;
    endThread();
}

