#include "SQLiteDatabase.hpp"

using BBermann::WRoot::Database::SQLiteDatabase;
using BBermann::WRoot::Database::ResultSet;

SQLiteDatabase::SQLiteDatabase() : db("wroot.db")
{
}

SQLiteDatabase::~SQLiteDatabase()
{
}

void SQLiteDatabase::execute(std::string command)
{
    return db.exec(command);
}

ResultSet SQLiteDatabase::query(std::string query)
{
    ResultSet response;
    StringList row;

    auto results = db.query(query);
    
    while (results.next(row))
    {
        response.push_back(row);
    }

    return response;
}