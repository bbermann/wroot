/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LuaScript.hpp
 * Author: bbermann
 *
 * Created on 24 de Julho de 2016, 20:50
 */

#ifndef LUASCRIPT_HPP
#define LUASCRIPT_HPP

#include <3rdParty/luawrapper/include/LuaContext.hpp>
#include <include/type/String.hpp>
#include <fstream>
#include <include/exceptions/lua/LuaScriptException.hpp>

class LuaScript {
public:
    explicit LuaScript(const String &script_path);

    virtual ~LuaScript();

    String getOutput();

    template<typename T = void>
    T executeScript() {
        try {
            auto reader = std::ifstream{this->script_path_.c_str()};

            return this->context.executeCode<T>(reader);
        } catch (const std::exception &exception) {
            throw LuaScriptException(
                    "LuaScript: failed executing the script '" + this->script_path_ + "', aborting.",
                    exception
            );
        }
    }

    template<typename T = void>
    T execute(const String &code) {
        try {
            return this->context.executeCode<T>(code.c_str());
        } catch (const std::exception &exception) {
            throw LuaScriptException(
                    "LuaScript: failed executing arbitrary lua code: '" + code + "', aborting.",
                    exception
            );
        }
    }

    LuaContext context;

protected:
    void registerFunctions();

    lua_State *state_{};
    String script_path_, last_error_, lua_output_;
};

#endif /* LUASCRIPT_HPP */

