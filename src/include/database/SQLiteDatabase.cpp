#include "SQLiteDatabase.hpp"
#include "include/database/TypeMapping.hpp"

using BBermann::WRoot::Database::ResultSet;
using BBermann::WRoot::Database::SQLiteDatabase;
using BBermann::WRoot::Database::TypeMapping;

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
    std::vector<SQLite::SQLField> row;

    auto result = db.query(query);

    if (result.num_cols() > 0)
    {
        StringList columnNames = result.get_col_names();

        // Iterate through rows
        while (result.next(row))
        {
            //TODO: This can't be a object... study how to pass pointer to type or initialize type inside with type as parameter
            BaseModel model;

            // Iterate through columns
            for (size_t idx = 0; idx < row.size(); idx++)
            {
                auto column = row.at(idx);
                auto columnName = columnNames[idx];
                auto columnType = column.type();

                switch (columnType)
                {
                case SQLITE3_TEXT:
                case SQLITE_BLOB:
                {
                    const char *strVal = column.get<const char *>();
                    model.set(columnName, strVal);
                    break;
                }

                case SQLITE_INTEGER:
                {
                    int* intVal = column.get<int*>();
                    model.set(columnName, intVal);
                    break;
                }

                case SQLITE_FLOAT:
                {
                    float* floatVal = column.get<float*>();
                    model.set(columnName, floatVal);
                    break;
                }

                case SQLITE_NULL:
                default:
                {
                    const char *unknownVal = column.get<const char *>();
                    model.set(columnName, unknownVal);
                    break;
                }
                }
            }

            response.push_back(model);
        }
    }

    return response;
}