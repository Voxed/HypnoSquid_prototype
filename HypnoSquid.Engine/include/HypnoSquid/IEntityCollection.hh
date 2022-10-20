#pragma once

#include <unordered_set>
#include "Entity.hh"

namespace hs {

    class IEntityCollection {
    public:
        /// Check whether or not an entity exists inside the collection.
        /// \param entity [in] The entity to check existence for.
        /// \return A bool representing whether or not the entity exists.
        virtual bool Has(const Entity &entity) const = 0;
    };

}