/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   LuaScript.cpp
 * Author: bbermann
 *
 * Created on 24 de Julho de 2016, 20:50
 */

#include "LuaScript.hpp"
#include "../exceptions/lua/LuaScriptException.hpp"
#include "../core/Core.hpp"

LuaScript::LuaScript(const String &script_path) {
    this->state_ = luaL_newstate();
    luaL_openlibs(this->state_);

    //Adiciona funções membro ao script lua.
    this->registerUserData();

    this->script_path_ = script_path;
}

LuaScript::LuaScript(const LuaScript &orig) {
}

LuaScript::~LuaScript() {
    lua_close(this->state_);
}

void LuaScript::registerUserData() {
    //print
    lua_pushlightuserdata(this->state_, this);
    lua_pushcclosure(this->state_, &LuaScript::lua_print, 1);
    lua_setglobal(this->state_, "print");
}

String LuaScript::executeScript(const String &entrypoint) {
    if (!this->loadFile()) {
        throw LuaScriptException("- Failed to open LUA script (" + this->script_path_ + ")!" + ENDL +
                                 "1) Please verify if the file exists." + ENDL +
                                 "2) If the file exists please verify the syntax of script." + ENDL +
                                 "Error description: " + this->getError()
        );
    }

    this->lua_output_.clear();

    // Run the previously open file
    if (lua_pcall(this->state_, 0, LUA_MULTRET, 0) != LUA_OK) {
        throw LuaScriptException(this->getError());
    }

    if (!entrypoint.empty() && luaL_dostring(this->state_, entrypoint.c_str()) != LUA_OK) {
        throw LuaScriptException(this->getError());
    }

    return this->lua_output_;
}

bool LuaScript::loadFile() {
    return luaL_loadfile(this->state_, this->script_path_.c_str()) == LUA_OK;
}

String LuaScript::getError() {
    this->last_error_ = lua_tostring(this->state_, -1);
    lua_pop(this->state_, 1);

    return this->last_error_;
}

void LuaScript::print(String msg) {
    this->lua_output_.append(msg);
}

int LuaScript::lua_print(lua_State *lua_state) {
    String msg = lua_tostring(lua_state, 1);

    //Obtém o objeto relacionado com esta chamada estática (para segurança multithread)
    auto *obj = (LuaScript *) lua_touserdata(lua_state, lua_upvalueindex(1));
    obj->print(msg);

    return 0;
}
