#include "include/core/Core.hpp"
#include "include/model/BaseModel.hpp"
#include <experimental/optional>

namespace BBermann::WRoot::Database {

    class ResultSet : public std::vector<BaseModel> {
    public:
        std::experimental::optional <BaseModel> first() {
            if (this->size() > 0) {
                return this->at(0);
            }

            return {};
        }

        std::experimental::optional <BaseModel> last() {
            if (this->size() > 0) {
                return this->at(this->size() - 1);
            }

            return {};
        }
    };

} // namespace BBermann::WRoot::Database