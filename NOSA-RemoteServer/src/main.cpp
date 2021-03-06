#include "remotecore.h"
#include <iostream>
#include <unistd.h>
#include "defines.h"

int main()
{
    int result = 0;
    LOOP_LINE
    {
        RemoteCore core(SOCKET_PORT);
        bool success = core.socketCreated();
        if (success)
        {
            std::cout << "Socket succesfully binded." << std::endl;
            result = core.run();
            core.cleanSockets();
        }
        ON_NO_SOCKET
        {
            std::cout << "Next socket binding attempt will be in " << RETRY_DELAY << " seconds" << std::endl;
            sleep(RETRY_DELAY);
        }
    }
    return result;
}

