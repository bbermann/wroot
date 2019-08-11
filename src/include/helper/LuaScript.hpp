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

#include "../core/Core.hpp"

extern "C" {
#include "../../3rdParty/lua/lua.h"
#include "../../3rdParty/lua/lualib.h"
#include "../../3rdParty/lua/lauxlib.h"
}

class LuaScript {
public:
    explicit LuaScript(const String &script_path);

    LuaScript(const LuaScript &orig);

    virtual ~LuaScript();

    String executeScript(const String &entrypoint = "");

    void print(String msg);

    static int lua_print(lua_State *lua_state);

protected:
    String getError();

    bool loadFile();

    void registerUserData();

    lua_State *state_{};
    String script_path_, last_error_, lua_output_;
};

#endif /* LUASCRIPT_HPP */

