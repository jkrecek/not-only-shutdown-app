#include "pccore.h"
#include <iostream>
#include <unistd.h>
#include "defines.h"

int doWork()
{
    int result = 0;
    LOOP_LINE
    {
        PCCore core;
        result = core.run();
        core.cleanSockets();

    #ifndef MODULE_NO_LOOP
        switch(core.getState()) {
            case STATE_DISCONNECTED:
            case STATE_SUCCESS:
                std::cout << "Next connection attempt will be in " << RETRY_DELAY << " seconds" << std::endl;
                sleep(RETRY_DELAY);
                break;

            case STATE_INVALID_CONFIG:
            case STATE_CONNECTED:
            default:
                return result;
        }
    #endif

    }

    return result;
}

#ifdef __WIN32

#include "windows.h"

class outbuf : public std::streambuf {
public:
    outbuf()
    {
        setp(0, 0);
    }
    virtual int_type overflow(int_type c = traits_type::eof()) {
        return fputc(c, stdout) == EOF ? traits_type::eof() : c;
    }
};

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int cmdShow)
{
    StringVector sv = Helper::split(lpCmdLine, '-');
    bool minimized = false;
    for (StringVector::iterator itr = sv.begin(); itr != sv.end(); ++ itr)
    {
        if (*itr == "minimized" || *itr == "m")
        {
            minimized = true;
            break;
        }
    }

    if (minimized)
    {
        return doWork();
    }
    else
    {
        if(AllocConsole())
        {
            freopen("CONOUT$", "w", stdout);
            SetConsoleTitle(SERVER_APP_NAME);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
        }

        outbuf ob;
        std::streambuf *sb = std::cout.rdbuf(&ob);

        int result = doWork();

        std::cout.rdbuf(sb);
        return result;
    }
}
#else
int main()
{
    return doWork();
}

#endif




