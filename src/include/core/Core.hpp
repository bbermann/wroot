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
#define TAB String("\t")

#include <mutex>
#include <thread>
#include "../type/String.hpp"
#include "../type/UrlRewriteRule.hpp"

namespace BBermann::WRoot::Database {
    class CustomDatabase;
}

class HttpServer;
class HttpResponse;

using BBermann::WRoot::Database::CustomDatabase;

class Core
{
  public:
    Core();
    ~Core();
    static void debug(std::string text);
    static void debugLn(std::string text);
    static void error(std::string text, int code = 0);
    static void error(std::string text, std::string function);
    static void out(std::string text);
    static void outLn(std::string text);
    static void setEnvironment(int argc, const char *argv[]);
    static void stopServers();
    static void warning(std::string text, std::string function);
    static HttpResponse httpError(unsigned short statusCode);

#ifdef WINDOWS
    static const int kWSockVersion = 2;
#endif

    static const unsigned int kMaxConnections = 10000, kBufferSize = 8192 * 10;
    
    static String ApplicationPath;
    static String ExecutablePath;
    static String PathSeparator;
    static String ServerAddress;
    static String ServerName;
    static String ServerProtocol;
    static String DocumentRoot;
    static StringList Parameters;
    static bool IsDebugging;
    static bool CompressedOutput;
    static bool CallBrowserOnStart;
    static bool Running;
    static int ThreadCount;
    static int ServerPort;
    static std::mutex ThreadMutex;
    static std::shared_ptr<HttpServer> Server;
    static std::vector<UrlRewriteRule> UrlRewriteRules;
    static const std::shared_ptr<CustomDatabase> db;

  private:
    static void readConfiguration();
    static void checkPrint(String check, String value);
    static std::mutex outMutex;
};

#endif //CORE_HPP
