#pragma once
#include <experimental/optional>
#include "include/core/Core.hpp"
#include "include/database/CustomDatabase.hpp"
#include "include/model/BaseModel.hpp"

namespace BBermann::WRoot::Repository
{

class BaseRepository
{
public:
  BaseRepository(std::string tableName) : db(Core::db)
  {
    this->tableName = tableName;
  }

  virtual ~BaseRepository();

  virtual bool insert(BaseModel model) = 0;

  virtual bool update(BaseModel model) = 0;

<<<<<<< Updated upstream
    virtual std::experimental::optional<BaseModel> find(int id) = 0;
=======
  virtual std::optional<BaseModel> find(int id) = 0;
>>>>>>> Stashed changes

protected:
  std::shared_ptr<CustomDatabase> const db;
  std::string tableName;
};

} // namespace BBermann::WRoot::Repository