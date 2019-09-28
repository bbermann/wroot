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

class HttpServer;

class HttpResponse;

class Core {
public:
    Core();

    ~Core();

#ifndef NDEBUG
    static inline void debug(const std::string &text) {
        Core::out(text);
    }

    static inline void debugLn(const std::string &text) {
        Core::outLn(text);
    }
#else
    static inline void debug(const std::string &text) {}

    static inline void debugLn(const std::string &text) {}
#endif

    static void error(const std::string &text, int code);

    static void error(const std::string &text, const std::string &function = "");

    static void out(const std::string &text);

    static void outLn(const std::string &text);

    static void setEnvironment(int argc, const char *argv[]);

    static void stopServers();

    static void warning(const std::string &text, const std::string &function = "");

#ifdef WINDOWS
    static const int kWSockVersion = 2;
#endif

    static const unsigned int kMaxConnections = 10000, kBufferSize = 8192 * 10;

    static String ApplicationRoot;
    static String PathSeparator;
    static String ServerAddress;
    static String ServerName;
    static String ServerProtocol;
    static String DocumentRoot;
    static StringList Parameters;
    static StringList Plugins;
    static bool IsDebugging;
    static bool CompressedOutput;
    static bool Running;
    static unsigned int ThreadCount;
    static int ServerPort;
    static std::mutex ThreadMutex;
    static std::shared_ptr <HttpServer> Server;
    static std::vector <UrlRewriteRule> UrlRewriteRules;

private:
    static void readConfiguration();

    static void loadPlugins();

    static void printStartupCheck(const String &check, const String &value);

    static std::mutex outMutex;
};

#endif //CORE_HPP
