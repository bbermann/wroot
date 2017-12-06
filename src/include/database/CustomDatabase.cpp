#include "CustomDatabase.hpp"
#include "../helper/ValidationHelper.hpp"

CustomDatabase::CustomDatabase(Database database_type, String hostname, unsigned port, String username, String password, String database) {
    SetDatabaseType(database_type);
    SetPort(port);
    SetHost(hostname);
    SetUser(username);
    SetPassword(password);
    SetDatabase(database);
}

CustomDatabase::~CustomDatabase() {
}

//Getter

unsigned CustomDatabase::GetPort() {
    return connection_port_;
}

String CustomDatabase::GetHost() {
    return connection_host_;
}

String CustomDatabase::GetUser() {
    return connection_user_;
}

String CustomDatabase::GetPassword() {
    return connection_pass_;
}

String CustomDatabase::GetDatabase() {
    return database_name_;
}

Database CustomDatabase::GetDatabaseType() {
    return database_type_;
}

//Setter

void CustomDatabase::SetPort(unsigned port) {
    Validation::set(connection_port_, port, ValidationCondition::IsPositive);
}

void CustomDatabase::SetHost(String hostname) {
    Validation::set(connection_host_, hostname, ValidationCondition::NotEmpty);
}

void CustomDatabase::SetUser(String username) {
    Validation::set(connection_user_, username, ValidationCondition::NotEmpty);
}

void CustomDatabase::SetPassword(String password) {
    Validation::set(connection_pass_, password, ValidationCondition::NotEmpty);
}

void CustomDatabase::SetDatabase(String database) {
    Validation::set(database_name_, database, ValidationCondition::NotEmpty);
}

void CustomDatabase::SetDatabaseType(Database database_type) {
    database_type_ = database_type;
}