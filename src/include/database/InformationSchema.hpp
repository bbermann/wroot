/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   InformationSchema.hpp
 * Author: bbermann
 *
 * Created on 6 de Janeiro de 2016, 20:30
 */

#ifndef INFORMATIONSCHEMA_HPP
#define INFORMATIONSCHEMA_HPP

#include "../database/CustomDatabase.hpp"
#include "DAO.hpp"

class InformationSchema;

class InformationSchema : public DAO, public DTO {
public:
    InformationSchema(Database database_type, CustomDatabase* database_ptr);
    InformationSchema(const InformationSchema& orig);
    InformationSchema(const DTO& orig);
    virtual ~InformationSchema();
    
    virtual void Get(String pk_value) override;
    virtual bool Create(DTO& obj) override;
    virtual int Update(DTO& obj) override;
    virtual bool Delete(DTO& obj) override;
    
private:
    PrimaryKey PK = PrimaryKey("column_name", "text");
};

#endif /* INFORMATIONSCHEMA_HPP */