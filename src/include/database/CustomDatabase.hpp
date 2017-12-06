#pragma once
#include "../type/String.hpp"

enum Database {
    None = 0,
    PostgreSQL = 1,
    Access = 2,
    MySQL = 3,
    Oracle = 4,
};

class CustomDatabase
{
public:
	CustomDatabase(Database database_type, String hostname, unsigned port, String username, String password, String database);
	virtual ~CustomDatabase();
	
	//Virtual pure decl
	virtual bool IsOpen() = 0;
	virtual bool Open() = 0;
	virtual void Close() = 0;
        virtual bool Prepare(String query) = 0;
        virtual void* ExecuteQuery(String query) = 0;

	//Getter
	unsigned GetPort();
	String GetHost();
	String GetUser();
	String GetPassword();
	String GetDatabase();
        Database GetDatabaseType();

	//Setter
	void SetPort(unsigned port);
	void SetHost(String hostname);
	void SetUser(String username);
	void SetPassword(String password);
	void SetDatabase(String database);
        void SetDatabaseType(Database database);

protected:
	unsigned connection_port_;
	String connection_host_, connection_user_, connection_pass_, 
		database_name_, database_schema_;
        Database database_type_;
};

