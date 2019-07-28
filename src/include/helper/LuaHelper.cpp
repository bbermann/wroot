/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   LuaHelper.cpp
 * Author: bbermann
 *
 * Created on 24 de Julho de 2016, 20:50
 */

#include "LuaHelper.hpp"
#include "../core/Core.hpp"

LuaHelper::LuaHelper(String script_path) {
    this->state_ = luaL_newstate();
    luaL_openlibs(this->state_);

    //Adiciona funções membro ao script lua.
    this->registerMemberFunctions();

    this->script_path_ = script_path;
}

LuaHelper::LuaHelper(const LuaHelper &orig) {
}

LuaHelper::~LuaHelper() {
    lua_close(this->state_);
}

void LuaHelper::registerMemberFunctions() {
    //print
    lua_pushlightuserdata(this->state_, this);
    lua_pushcclosure(this->state_, &LuaHelper::lua_print, 1);
    lua_setglobal(this->state_, "print");
}

String LuaHelper::executeScript() {
    if (!this->loadFile()) {
        return
                "- Failed to open LUA script (<b>" + String::toUpper(this->script_path_) + "</b>)!<br/>" + ENDL +
                "1) Please verify if the file exists.<br/>" + ENDL +
                "2) If the file exists please verify the syntax of script.<br/><br/>" + ENDL +
                "<b>Descrição do Erro:</b> " + this->getError() + "<br/>" + ENDL;
    }

    this->lua_output_.clear();

    if (lua_pcall(this->state_, 0, LUA_MULTRET, 0) == LUA_OK) {
        return this->lua_output_;
    } else {
        return this->getError();
    }
}

/*String LuaHelper::getScriptReturn()
{
if (lua_gettop(this->state_))
{
//Remover da pilha a string de retorno...
return lua_tostring(this->state_, lua_gettop(this->state_));
}
}*/

bool LuaHelper::loadFile() {
    String script_path = Core::ApplicationPath + Core::PathSeparator + "lua" + Core::PathSeparator;
    script_path.append(this->script_path_);

    return luaL_loadfile(this->state_, script_path.c_str()) == LUA_OK;
}

String LuaHelper::getError() {
    this->last_error_ = lua_tostring(state_, -1);
    lua_pop(this->state_, 1);

    return this->last_error_;
}

void LuaHelper::print(String msg) {
    this->lua_output_.append(msg);
}

int LuaHelper::lua_print(lua_State *lua_state) {
    String msg = lua_tostring(lua_state, 1);

    //Obtém o objeto relacionado com esta chamada estática (para segurança multithread)
    LuaHelper *obj = (LuaHelper *) lua_touserdata(lua_state, lua_upvalueindex(1));
    obj->print(msg);

    return 0;
}
