/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   InformationSchema.cpp
 * Author: bbermann
 * 
 * Created on 6 de Janeiro de 2016, 20:30
 */

#include "InformationSchema.hpp"

InformationSchema::InformationSchema(Database database_type, CustomDatabase* database_ptr)
: DAO(database_type, database_ptr, PK), DTO() {
}

InformationSchema::InformationSchema(const InformationSchema& orig)
: DAO(orig), DTO(orig) {
}

InformationSchema::InformationSchema(const DTO& orig)
: DAO(Database::None, (CustomDatabase*) nullptr, PK), DTO(orig) {
}

InformationSchema::~InformationSchema() {
}

void InformationSchema::Get(String pk_value) {
    DAO::Get(pk_value);

    //TODO: Popular este objeto manualmente com as informações geradas pelo Get da DAO.
}

bool InformationSchema::Create(DTO& obj) {
    return false;
}

int InformationSchema::Update(DTO& obj) {
    return false;
}

bool InformationSchema::Delete(DTO& obj) {
    return false;
}