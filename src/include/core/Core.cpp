#include "Core.hpp"
#include <nlohmann/json.hpp>
#include <mutex>
#include <iostream>
#include <optional>
#include <filesystem>
#include <include/helper/LuaScript.hpp>
#include <include/exceptions/lua/LuaScriptException.hpp>

using json = nlohmann::json;

String Core::ApplicationRoot;
String Core::PathSeparator;
String Core::ServerAddress;
String Core::ServerName;
String Core::ServerProtocol;
String Core::ServerListenAddress;
String Core::DocumentRoot;
StringList Core::Parameters;
StringList Core::Plugins;
bool Core::IsDebugging;
size_t Core::ServerPort;
size_t Core::RequestTimeout;
std::mutex Core::ThreadMutex;
std::vector<UrlRewriteRule> Core::UrlRewriteRules;
std::mutex Core::outMutex;
StringMap Core::Cache;

Core::Core() = default;

Core::~Core() = default;

void Core::error(const std::string &text, int code) {
    String tmp = "## ERROR ##";

    if (code > 0) {
        tmp.append(" " + std::to_string(code));
    }

    tmp.append(ENDL + TAB + text);

    outLn(tmp);
    exit(code);
}

void Core::error(const std::string &text, const std::string &function) {
    String tmp = "## ERROR ##";

    if (!function.empty()) {
        tmp.append(" in ");
        tmp.append(function);
    }

    tmp.append(ENDL + TAB + text);

    outLn(tmp);
    exit(-1);
}

void Core::out(const std::string &text) {
    Core::outMutex.lock();
    std::cout << text.c_str();
    Core::outMutex.unlock();
}

void Core::outLn(const std::string &text) {
    out(text + ENDL);
}

void Core::readConfiguration() {
    String configFile("wroot.json");
    Core::printStartupCheck("Configuration file", configFile);

    std::ifstream reader(configFile.c_str());

    //The JSON string parser which will interpret the config file
    json config;

    if (reader.is_open() && reader.good()) {
        reader >> config;
    } else {
        Core::error("Can't open file " + configFile + "!");
    }

    auto server = config["server"];

    Core::ServerName = server["name"];
    Core::ServerPort = server["port"];
    Core::ServerListenAddress = server["listen"];

    Core::DocumentRoot = server["document_root"];
    if (Core::DocumentRoot.endsWith("/")) {
        Core::DocumentRoot = Core::DocumentRoot.substr(0, Core::DocumentRoot.size() - 1);
    }

    if (server["request_timeout"].empty()) {
        Core::RequestTimeout = 60;
    } else {
        Core::RequestTimeout = server["request_timeout"];
    }
    Core::printStartupCheck("Request timeout", std::to_string(Core::RequestTimeout));

    auto urlRewriteRulesParser = config["url_rewrite"];
    for (auto ruleParser : urlRewriteRulesParser) {
        String input = ruleParser["input"];

        UrlRewriteRule rule;
        rule.input = std::regex(input.c_str());
        rule.output = ruleParser["output"];

        Core::UrlRewriteRules.insert(Core::UrlRewriteRules.end(), rule);
    }

    Core::loadPlugins();

    Core::ServerAddress = Core::ServerListenAddress + ":" + std::to_string(Core::ServerPort);
    Core::printStartupCheck("Listening on", Core::ServerAddress);
}

void Core::loadPlugins() {
    String pluginsRoot = Core::ApplicationRoot + "/plugins";

    for (auto &file: std::filesystem::recursive_directory_iterator(pluginsRoot.c_str())) {
        if (file.path().extension() == ".lua") {
            String pluginPath = (String)file.path();
            String status = "OK";

            try {
                LuaScript luaScript(pluginPath);
                luaScript.executeScript();
                luaScript.execute("test()");

                Core::Plugins.push_back(pluginPath);
            } catch (const LuaScriptException &exception) {
                status = "FAILED";

                Core::warning(exception.what());
            }

            Core::printStartupCheck("Loading '" + pluginPath + "'", status);
        }
    }
}

void Core::setEnvironment(int argc, const char *argv[]) {
    Core::outLn("Loading wRoot, please wait a second...");

#ifndef NDEBUG
    Core::IsDebugging = true;
#else
    Core::IsDebugging = false;
#endif
    Core::printStartupCheck("Debug Mode", Core::IsDebugging ? "Enabled" : "Disabled");

#ifdef WINDOWS
    Core::PathSeparator = "\\";
#else
    Core::PathSeparator = "/";
#endif

    Core::ApplicationRoot = String(argv[0]);
    Core::ServerProtocol = "HTTP/1.1";

    StringList arrPath = Core::ApplicationRoot.explode(Core::PathSeparator);
    const auto executableName = arrPath.at(arrPath.size() - 1);

    //Get Application Root Path Without Executable Name
    String::replace_last(Core::ApplicationRoot, Core::PathSeparator + executableName, "");

    for (int i = 1; i < argc; i++) {
        Core::Parameters.insert(Core::Parameters.end(), argv[i]);
    }

    Core::readConfiguration();

    Core::outLn("Listening for incoming connections...");
}

void Core::stopServers() {
    Core::ThreadMutex.lock();
    std::this_thread::sleep_for(std::chrono::seconds(10));
}

void Core::warning(const std::string &text, const std::string &function) {
    String tmp = "## WARNING ##";

    if (!function.empty()) {
        tmp.append(" in ");
        tmp.append(function);
    }

    tmp.append(ENDL + TAB + text);
    outLn(tmp);
}

void Core::printStartupCheck(const String &check, const String &value) {
    Core::outLn("- " + check + ": [" + value + "]");
}