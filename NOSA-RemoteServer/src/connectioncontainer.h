#ifndef CONNECTIONCONTAINER_H
#define CONNECTIONCONTAINER_H

#include "helper.h"
#include "androidconnection.h"
#include "pcconnection.h"
#include "singleton.h"

//typedef  ConnectionIterator;

typedef std::vector<BaseConnection*> ConnectionVector;

class ConnectionContainer
{
public:
    class iterator
    {
    public:
        iterator(ConnectionVector** vector);
        void next();
        bool hasMore();
        BaseConnection* get();

    private:
        ConnectionVector** vector;
        ConnectionVector::iterator current;
    };

    ConnectionContainer() {}

    iterator begin();

    void insert(BaseConnection* connection);
    bool get(std::vector<BaseConnection*>& connections);
    PCConnection* getPCConnection(AndroidConnection* con);
    std::vector<AndroidConnection*> getAndroidConnections(PCConnection* con);

private:
    ConnectionVector container[TYPE_COUNT];
};

#define sConnections Singleton<ConnectionContainer>::getInstance()

#endif // CONNECTIONCONTAINER_H
