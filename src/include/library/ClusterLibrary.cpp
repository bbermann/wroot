#include "ClusterLibrary.hpp"
#include "../core/Core.hpp"
#include "nlohmann/json/src/json.hpp"
#include "include/database/CustomDatabase.hpp"
#include "include/repository/ClusterRepository.hpp"

using json = nlohmann::json;
using namespace std;
using namespace BBermann::WRoot::Database;

ClusterLibrary::ClusterLibrary() : CustomLibrary()
{
    this->responseType = "application/json";
}

ClusterLibrary::~ClusterLibrary()
{
    //TODO: Do something useful with the SQLiteDatabase driver (teoretically working now)
    //See: https://github.com/vincentlaucsb/sqlite-cpp
    auto db = Core::db();

    /*
    db->execute("CREATE TABLE IF NOT EXISTS users (" 
            "   id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "   token TEXT NOT NULL UNIQUE,"
            "   first_name TEXT NOT NULL,"
            "   last_name TEXT NOT NULL,"
            "   permission_id INTEGER NOT NULL DEFAULT 0"
            ")");

    db->execute("CREATE TABLE IF NOT EXISTS clusters (" 
            "   id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "   token TEXT NOT NULL UNIQUE,"
            "   name TEXT NOT NULL UNIQUE," 
            "   user_id INTEGER NOT NULL,"
            "   FOREIGN KEY(user_id) REFERENCES users(id)"
            ")");

    db->execute("CREATE TABLE IF NOT EXISTS cluster_nodes (" 
            "   id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "   token TEXT NOT NULL UNIQUE,"
            "   cluster_id INTEGER NOT NULL UNIQUE,"
            "   user_id INTEGER NOT NULL UNIQUE,"
            "   FOREIGN KEY(user_id) REFERENCES users(id),"
            "   FOREIGN KEY(cluster_id) REFERENCES clusters(id)"
            ")");

    db->execute("INSERT INTO users (first_name, last_name, permission_id) VALUES ('Bruno', 'Bermann', 1337)");
        
    ResultSet result = db->query("SELECT id FROM users WHERE permission_id = 1337");
    if (!result.empty()) {
        auto userId = result.first().at(0);
        
        std::stringstream query;
        query << "INSERT INTO clusters (name, user_id) VALUES ('cluster-0x', " << userId << ")";
        db->execute(query.str());

        result = db->query("SELECT id FROM clusters LIMIT 1");
        if (!result.empty()) 
        {
            auto clusterId = result.first().at(0);

            query.str("");
            query << "INSERT INTO cluster_nodes (token, user_id, cluster_id) VALUES ('1234', " << userId << ", " << clusterId << ")";
            db->execute(query.str());
        }
    }
    */
}

String ClusterLibrary::toString()
{
    json response;
    response["remoteAddress"] = request.get("REMOTE_ADDR");

    // TODO: See dev/cluster.txt and cluster-controller.js
    auto url = request.getUrl();
    response["query"] = url;

    // /cluster/{id}/command
    auto parts = url.explode("/");
    auto size = parts.size();

    if (size > 3 || size <= 1)
    {
        response["exception"] = json();
        response["exception"]["message"] = "Disallowed usage.";
        response["exception"]["code"] = 1;
    }
    else if (size > 1)
    {
        response["clusterId"] = parts.at(1);

        if (size > 2)
        {
            response["command"] = parts.at(2);

            auto params = request.getQuery();

            if (params.size() > 0)
            {
                response["params"] = json();

                for (auto param : params)
                {
                    response["params"][param.first] = param.second;
                }
            }
        }
    }

    //TODO: Implement ClusterNodeLibrary, adding V8 JS Engine - https://v8.dev/docs/embed

    return response.dump() + END_CONNECTION;
}