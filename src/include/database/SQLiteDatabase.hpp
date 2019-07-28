#pragma once

#include "sqlite-cpp/src/sqlite_cpp.h"
#include "CustomDatabase.hpp"

namespace BBermann::WRoot::Database {

    class SQLiteDatabase : public CustomDatabase {
    public:
        SQLiteDatabase();

        virtual ~SQLiteDatabase();

        virtual void execute(std::string command) override;

        virtual ResultSet query(std::string query) override;

        //TODO: Move to CustomDatabase (templates methods can't be virtual)
        template<typename T>
        ResultSet query(std::string query) {
            ResultSet response;
            std::vector <SQLite::SQLField> row;

            auto result = db.query(query);

            if (result.num_cols() > 0) {
                StringList columnNames = result.get_col_names();

                // Iterate through rows
                while (result.next(row)) {
                    T model;

                    // Iterate through columns
                    for (int idx = 0; idx < row.size(); idx++) {
                        auto column = row.at(idx);
                        auto columnName = columnNames[idx];
                        auto columnType = model.getColumnMappingType(columnName);

                        switch (columnType) {
                            case TypeMapping::Text:
                                const char *strVal = column.get<const char *>();
                                model.set(columnName, strVal);
                                break;

                            case TypeMapping::Integer:
                                int intVal = column.get<int>();
                                model.set(columnName, intVal);
                                break;

                            case TypeMapping::Double:
                                double doubleVal = column.get<double>();
                                model.set(columnName, doubleVal);
                                break;

                            case TypeMapping::Unknown:
                            default:
                                const char *unknownVal = column.get<const char *>();
                                model.set(columnName, unknownVal);
                                break;
                        }
                    }

                    response.push_back(model);
                }
            }

            return response;
        }

        SQLite::Conn db;
    };

} // namespace BBermann::WRoot::Database