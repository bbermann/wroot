#ifndef CORE_HPP
#define CORE_HPP

#define LINE_BREAK String("\n")
#define CARRIAGE_RETURN String("\r")

#ifdef WINDOWS
#define ENDL String("\r\n")
#else
#define ENDL String("\n")
#endif

#define END_CONNECTION String("\r\n\r\n")

#include <mutex>
#include <thread>
#include "../type/String.hpp"

class HttpServer;

class Core
{
public:
    Core();
    ~Core();
    static void debug(std::string text);
    static void debugLn(std::string text);
    static void error(std::string text, int code = 0);
    static void error(std::string text, std::string function);
    static void getStackTrace(int trace_count_max = 32);
    static void out(std::string text);
    static void outLn(std::string text);
    static void setEnvironment(int argc, const char* argv[]);
    static void stopServers();
    static void warning(std::string text, std::string function);
    
    static const int kWSockVersion = 2;
    static const unsigned int kMaxConnections = 10000, kBufferSize = 8192;

    static String ApplicationPath;
    static String ExecutablePath;
    static String PathSeparator;
    static StringList Parameters;
    static bool IsDebugging;
    static bool SafeThreads;
    static bool UseCompressedOutput;
    static bool UseBrowserCache;
    static bool CallBrowserOnStart;
    static bool Running;
    static int ThreadCount;
    static size_t HttpServerPortNumber;
    static std::mutex ThreadMutex;
    static std::shared_ptr<HttpServer> Server;
};

#endif //CORE_HPP
