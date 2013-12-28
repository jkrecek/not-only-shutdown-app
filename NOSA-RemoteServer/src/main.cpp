#include "configuration.h"
#include "remotecore.h"
#include <iostream>

#define PORT 3691

int main()
{
    Configuration::loadFile("D:\\Dropbox\\Dev\\ShutdownApp\\config.conf");
    RemoteCore core(PORT);
    int result = core.run();
#ifdef _WIN32
    WSACleanup();
#endif
    return result;
}

