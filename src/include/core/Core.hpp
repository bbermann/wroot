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
#include "../type/UrlRewriteRule.hpp"

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
    static void readConfiguration();
    static void setEnvironment(int argc, const char *argv[]);
    static void stopServers();
    static void warning(std::string text, std::string function);

#ifdef WINDOWS
    static const int kWSockVersion = 2;
#endif

    static const unsigned int kMaxConnections = 10000, kBufferSize = 8192 * 10;
    static constexpr const char *kApplicationName = "wRoot";

    static String ApplicationPath;
    static String ExecutablePath;
    static String PathSeparator;
    static String ServerAddress;
    static String ServerName;
    static String ServerProtocol;
    static String DocumentRoot;
    static StringList Parameters;
    static bool IsDebugging;
    static bool UseCompressedOutput;
    static bool UseBrowserCache;
    static bool CallBrowserOnStart;
    static bool Running;
    static int ThreadCount;
    static int ServerPort;
    static std::mutex ThreadMutex;
    static std::shared_ptr<HttpServer> Server;
    static std::vector<UrlRewriteRule> UrlRewriteRules;

  private:
    static void checkPrint(String check, String value);

    static std::mutex outMutex;
};

#endif //CORE_HPP
