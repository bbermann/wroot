#include "MysqlDatabase.hpp"
#include "../core/Core.hpp"

MysqlDatabase::MysqlDatabase(String hostname, unsigned port, String username, String password, String database)
: CustomDatabase(Database::MySQL, hostname, port, username, password, database) {
    driver = get_driver_instance();
}

MysqlDatabase::~MysqlDatabase() {
}

bool MysqlDatabase::IsOpen() {
    return is_open_;
}

bool MysqlDatabase::Open() {
    conn = driver->connect("tcp://" + GetHost(), GetUser(), GetPassword());
    conn->setSchema(GetDatabase());
    statement = conn->createStatement();

    is_open_ = !conn->isClosed();

    return is_open_;
}

void MysqlDatabase::Close() {
    conn->close();
}

bool MysqlDatabase::Prepare(String query) {
    try {
        prepared_statement_ = conn->prepareStatement(query);
    } catch (std::exception e) {
        Core::debugLn(e.what());
        return false;
    }

    return true;
}

void* MysqlDatabase::ExecuteQuery(String query) {
	ResultSet* rstSet = nullptr;

	if (!Prepare(query)) return rstSet;

	try {
		rstSet = prepared_statement_->executeQuery();
	}
	catch (std::exception e) {
		Core::error(e.what());
	}

	return (void*)rstSet;
}