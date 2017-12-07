#include "Core.hpp"
#include "../helper/ConsoleLineHelper.hpp"
#include <mutex>

using namespace std;

Core::Core()
{
}

Core::~Core()
{
}

void Core::debug(string text)
{
    if (Core::IsDebugging)
    {
        out(text);
    }
}

void Core::debugLn(string text)
{
    debug(text + ENDL);
}

void Core::error(string text, int code)
{
    String tmp = "ERROR";

    if (code > 0)
    {
        tmp.append(" ");
        tmp.append(to_string(code));
    }

    tmp.append(": ");
    tmp.append(text);

    outLn(tmp);
    exit(code);
}

void Core::error(string text, string function)
{
    String tmp = "ERROR";

    if (!function.empty())
    {
        tmp.append(" in ");
        tmp.append(function);
    }

    tmp.append(": ");
    tmp.append(text);
    outLn(tmp);

    exit(-1);
}

void Core::out(string text)
{
    cout << text.c_str();
}

void Core::outLn(string text)
{
    out(text + ENDL);
}

void Core::setEnvironment(int argc, const char* argv[])
{
    Core::ServerName = "wRoot";
    Core::ServerProtocol = "HTTP/1.1";

#ifdef DEBUG
    Core::IsDebugging = true;
#else
    Core::IsDebugging = false;
#endif

#ifdef WINDOWS
    Core::PathSeparator = "\\";
#else
    Core::PathSeparator = "/";
#endif

    Core::ApplicationPath = String(argv[0]);

    //TODO: Read all user configurations from wroot.json
    Core::ServerPort = 8000;

    StringList arrPath = Core::ApplicationPath.explode(Core::PathSeparator);
    Core::ExecutablePath = arrPath.at(arrPath.size() - 1);

    //Get Application Path Without Executable Name
    String::replace(Core::ApplicationPath, Core::ExecutablePath, "");

    for (int i = 1; i < argc; i++)
    {
        Core::Parameters.insert(Core::Parameters.end(), argv[i]);
    }

    //Desabilitar quando for necessário atender mais do que o limite de conexões, colocando em uma fila de espera
    Core::out("- Using thread safety code: ");
    Core::SafeThreads = true;
    Core::outLn((string) "[" + (Core::SafeThreads ? "Yes" : "No") + "]");

    Core::out("- Using compressed output: ");
    Core::UseCompressedOutput = true;
    Core::outLn((string) "[" + (Core::UseCompressedOutput ? "Yes" : "No") + "]");

    //TODO: Implementar
    Core::out("- Using browser cache: ");
    Core::UseBrowserCache = false;
    Core::outLn((string) "[" + (Core::UseBrowserCache ? "Yes" : "No") + "]");

    //Iniciar o Google Chrome ao chamar HttpServer::start();
    Core::out("- Call browser on start: ");
    Core::CallBrowserOnStart = false;
    Core::outLn((string) "[" + (Core::CallBrowserOnStart ? "Yes" : "No") + "]");

    Core::out("- Counting hardware threads: ");
    Core::ThreadCount = thread::hardware_concurrency();
    Core::outLn("[" + to_string(Core::ThreadCount) + "]");

    ConsoleLineHelper server_address_resolver("curl --silent ipinfo.io/ip");
    Core::out("- Getting external IP Address: ");
    Core::ServerAddress = server_address_resolver.executeStdOut().trim();
    Core::outLn("[" + Core::ServerAddress + "]");
}

void Core::getStackTrace(int trace_count_max)
{
    Core::stopServers();
}

void Core::stopServers()
{
    Core::Running = false;
    Core::ThreadMutex.lock();
    this_thread::sleep_for(chrono::seconds(10));
}

void Core::warning(string text, string function)
{
    String tmp = "WARNING";

    if (!function.empty())
    {
        tmp.append(" in ");
        tmp.append(function);
    }

    tmp.append(": ");
    tmp.append(text);
    outLn(tmp);
}
