#pragma once
#include <optional>
#include "include/core/Core.hpp"
#include "include/database/CustomDatabase.hpp"
#include "include/model/BaseModel.hpp"

namespace BBermann::WRoot::Repository
{

class BaseRepository
{
  public:
    BaseRepository() : db(Core::db())
    {
    }

    virtual ~BaseRepository();

    virtual bool insert(BaseModel model) = 0;

    virtual bool update(BaseModel model) = 0;

    virtual std::optional<BaseModel> find(int id) = 0;

  protected:
    const CustomDatabase *db;
};

} // namespace BBermann::WRoot::Repository