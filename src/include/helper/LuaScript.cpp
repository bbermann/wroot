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
#include <include/exceptions/lua/LuaScriptException.hpp>

LuaScript::LuaScript(const String &script_path) : script_path_(script_path) {
    this->registerFunctions();
}

LuaScript::~LuaScript() = default;

String LuaScript::getOutput() {
    return this->lua_output_;
}

void LuaScript::registerFunctions() {
    this->context.writeFunction(
        "print",
        [this](const std::string &msg) {
            this->lua_output_.append(msg);
        }
    );
}