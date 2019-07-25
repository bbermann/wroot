#pragma once
#include "include/core/Core.hpp"
#include "include/database/CustomDatabase.hpp"
#include "include/model/Cluster.hpp"
#include "include/repository/BaseRepository.hpp"
#include <experimental/optional>

namespace BBermann::WRoot::Repository
{
class ClusterRepository : public BaseRepository
{
  public:
    ClusterRepository() : BaseRepository("clusters")
    {
    }

    virtual ~ClusterRepository()
    {
    }

    virtual bool insert(BaseModel model)
    {
        return false;
    }

    virtual bool update(BaseModel model)
    {
        return false;
    }

    virtual std::experimental::optional<BaseModel> find(int id)
    {
        auto result = this->db->query("SELECT * FROM " + this->tableName + " WHERE id = " + std::to_string(id) + " LIMIT 1");
        return result.first();
    }
};

} // namespace BBermann::WRoot::Repository