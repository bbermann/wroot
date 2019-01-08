#pragma once
#include "sqlite-cpp/src/sqlite_cpp.h"
#include "CustomDatabase.hpp"

namespace BBermann::WRoot::Database
{

class SQLiteDatabase : public CustomDatabase
{
  public:
    SQLiteDatabase();
    virtual ~SQLiteDatabase();

    virtual void execute(std::string command) override;
    virtual ResultSet query(std::string query) override;

    SQLite::Conn db;
};

} // namespace BBermann::WRoot::Database