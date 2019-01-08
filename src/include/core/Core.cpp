#include "Core.hpp"
#include "../helper/ConsoleLineHelper.hpp"
#include "nlohmann/json/src/json.hpp"
#include "include/core/HttpResponse.hpp"
#include "include/database/SQLiteDatabase.hpp"
#include <mutex>
#include <fstream>

using json = nlohmann::json;
using namespace std;
using namespace BBermann::WRoot::Database;

String Core::ApplicationPath;
String Core::ExecutablePath;
String Core::PathSeparator;
String Core::ServerAddress;
String Core::ServerName;
String Core::ServerProtocol;
String Core::DocumentRoot;
StringList Core::Parameters;
bool Core::IsDebugging;
bool Core::CompressedOutput;
bool Core::CallBrowserOnStart;
bool Core::Running;
int Core::ThreadCount;
int Core::ServerPort;
std::mutex Core::ThreadMutex;
std::shared_ptr<HttpServer> Core::Server;
std::vector<UrlRewriteRule> Core::UrlRewriteRules;
std::mutex Core::outMutex;
CustomDatabase* Core::db_ = new SQLiteDatabase();

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
    String tmp = "## ERROR ##";

    if (code > 0)
    {
        tmp.append(" " + to_string(code));
    }

    tmp.append(ENDL + TAB + text);

    outLn(tmp);
    exit(code);
}

void Core::error(string text, string function)
{
    String tmp = "## ERROR ##";

    if (!function.empty())
    {
        tmp.append(" in ");
        tmp.append(function);
    }

    tmp.append(ENDL + TAB + text);
    
    outLn(tmp);
    exit(-1);
}

void Core::out(string text)
{
    Core::outMutex.lock();
    cout << text.c_str();
    Core::outMutex.unlock();
}

void Core::outLn(string text)
{
    out(text + ENDL);
}

void Core::readConfiguration()
{
    String configFile(Core::ExecutablePath + ".json");
    Core::checkPrint("Reading configuration file", configFile);

    ifstream reader(configFile.c_str());
    
    //The JSON string parser which will interpret the config file
    json config;

    if (reader.is_open() && reader.good())
    {
        reader >> config;
    }
    else
    {
        Core::error("Can't open file " + configFile + "!");
    }

    auto server = config["server"];
    
    Core::ServerName = server["name"];
    Core::ServerPort = server["port"];

    Core::DocumentRoot = server["document_root"];
    if (!Core::DocumentRoot.endsWith("/"))
    {
        Core::DocumentRoot.append("/");
    }

    Core::CompressedOutput = server["compressed_output"];
    Core::checkPrint("Using compressed output", (Core::CompressedOutput ? "Yes" : "No"));

    Core::CallBrowserOnStart = server["desktop_server"];
    Core::checkPrint("Call browser on start", (Core::CallBrowserOnStart ? "Yes" : "No"));

    Core::ThreadCount = thread::hardware_concurrency();
    Core::checkPrint("Checking CPU cores", to_string(Core::ThreadCount));

    auto urlRewriteRulesParser = config["url_rewrite"];
    for (auto ruleParser : urlRewriteRulesParser)
    {
        String input = ruleParser["input"];

        UrlRewriteRule rule;
        rule.input = std::regex(input.c_str());
        rule.output = ruleParser["output"];
        
        Core::UrlRewriteRules.insert(Core::UrlRewriteRules.end(), rule);
    }

    Core::ServerAddress = Core::ServerName;
    Core::checkPrint("Retrieving remote IP address", Core::ServerAddress);
}

void Core::setEnvironment(int argc, const char *argv[])
{
    Core::outLn("Loading wRoot, please wait a second...");

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
    Core::ServerProtocol = "HTTP/1.1";

    StringList arrPath = Core::ApplicationPath.explode(Core::PathSeparator);
    Core::ExecutablePath = arrPath.at(arrPath.size() - 1);

    //Get Application Path Without Executable Name
    String::replace(Core::ApplicationPath, Core::ExecutablePath, "");

    for (int i = 1; i < argc; i++)
    {
        Core::Parameters.insert(Core::Parameters.end(), argv[i]);
    }

    Core::readConfiguration();
    Core::outLn("Done.");
}

void Core::stopServers()
{
    Core::Running = false;
    Core::ThreadMutex.lock();
    this_thread::sleep_for(chrono::seconds(10));
}

void Core::warning(string text, string function)
{
    String tmp = "## WARNING ##";

    if (!function.empty())
    {
        tmp.append(" in ");
        tmp.append(function);
    }

    tmp.append(ENDL + TAB + text);
    outLn(tmp);
}

void Core::checkPrint(String check, String value)
{
    Core::outLn("- " + check + ": [" + value + "]");
}

CustomDatabase* Core::db()
{
    return Core::db_;
}

HttpResponse Core::httpError(unsigned short statusCode)
{
    HttpResponse response;
    response.status = statusCode;
    response.compressOutput = true;
    return response;
}