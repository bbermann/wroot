/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DAO.cpp
 * Author: bbermann
 * 
 * Created on 6 de Janeiro de 2016, 20:49
 */

#include "DAO.hpp"

DAO::DAO(Database database_type, CustomDatabase* database_ptr, PrimaryKey pk) {
    //Internal use only.
    database_type_ = database_type;
    database_ptr_ = database_ptr;
    pk_ = pk;
}

DAO::~DAO() {
    
}

void DAO::Get(String pk_value) {
    database_ptr_->ExecuteQuery("SELECT * FROM " + this->pk_.first + " WHERE " + this->pk_.second + " = " + pk_value);
}

bool DAO::Create(DTO& obj) {
    return false;
}

int DAO::Update(DTO& obj) {
    return false;
}

bool DAO::Delete(DTO& obj) { 
    return false;
}