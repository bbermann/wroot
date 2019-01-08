#pragma once
#include "include/core/Core.hpp"
#include "include/database/CustomDatabase.hpp"
#include "include/repository/BaseRepository.hpp"
#include <optional>

namespace BBermann::WRoot::Repository
{
class ClusterRepository : public BaseRepository
{
  public:
    ClusterRepository()
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

    virtual std::optional<BaseModel> find(int id)
    {
        //this->db->find(id);

        return {};
    }
};  

} // namespace BBermann::WRoot::Repository