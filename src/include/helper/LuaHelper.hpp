/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LuaHelper.hpp
 * Author: bbermann
 *
 * Created on 24 de Julho de 2016, 20:50
 */

#ifndef LUAHELPER_HPP
#define LUAHELPER_HPP

#include "../include/core/Core.hpp"
#include "../3rdParty/lua/src/lua.hpp"

class LuaHelper
{
public:
    LuaHelper(String script_path);
    LuaHelper(const LuaHelper& orig);
    virtual ~LuaHelper();

    String executeScript();
    void print(String msg);
    
    static int lua_print(lua_State* lua_state);
    static int lua_println(lua_State* lua_state);
    
protected:
    String getError();
    bool loadFile();
    void registerMemberFunctions();
    
    lua_State* state_;
    String script_path_, last_error_, lua_output_;
};

#endif /* LUAHELPER_HPP */

