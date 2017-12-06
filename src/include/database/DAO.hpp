/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DAO.hpp
 * Author: bbermann
 *
 * Created on 6 de Janeiro de 2016, 20:49
 */

#ifndef DAO_HPP
#define DAO_HPP

#include "DTO.hpp"
#include "../database/CustomDatabase.hpp"

typedef std::pair<String, String> PrimaryKey;

class DAO {
    
public:
    DAO(Database database_type, CustomDatabase* database_ptr, PrimaryKey pk);
    virtual ~DAO();
    virtual void Get(String pk_value);
    virtual bool Create(DTO& obj);
    virtual int Update(DTO& obj);
    virtual bool Delete(DTO& obj);

private:
    Database database_type_;
    CustomDatabase* database_ptr_;
    PrimaryKey pk_;
};

#endif /* DAO_HPP */

