#pragma once

#include "include/core/Core.hpp"
#include "include/database/ResultSet.hpp"

namespace BBermann::WRoot::Database {

    class CustomDatabase {
    public:
        CustomDatabase() {
        }

        virtual ~CustomDatabase() {
        }

        // Pure virtuals
        virtual void execute(std::string command) = 0;

        virtual ResultSet query(std::string query) = 0;
    };

} // namespace BBermann::WRoot::Database