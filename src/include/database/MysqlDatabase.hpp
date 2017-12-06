#pragma once

#include "CustomDatabase.hpp"
#include <stdlib.h>
#include <iostream>

#include "../../3rdParty/mysql/include/mysql_connection.h"
#include "../../3rdParty/mysql/include/cppconn/driver.h"
#include "../../3rdParty/mysql/include/cppconn/exception.h"
#include "../../3rdParty/mysql/include/cppconn/resultset.h"
#include "../../3rdParty/mysql/include/cppconn/statement.h"
#include "../../3rdParty/mysql/include/cppconn/prepared_statement.h"

using namespace sql;

class MysqlDatabase : public CustomDatabase
{
public:
	MysqlDatabase(String hostname, unsigned port, String username, String password, String database);
	~MysqlDatabase();
		virtual void* ExecuteQuery(String query) override;
        virtual void Close() override;
	virtual bool IsOpen() override;
	virtual bool Open() override;
        
        Connection* conn;

private:
	Driver* driver = nullptr;
	ResultSet* result_set_ = nullptr;
	Statement* statement = nullptr;
	PreparedStatement* prepared_statement_ = nullptr;
	bool is_open_ = false;
        
    virtual bool Prepare(String query) override;
};