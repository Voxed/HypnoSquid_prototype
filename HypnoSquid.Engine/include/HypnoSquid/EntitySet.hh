#pragma once

#include "Entity.hh"

#include <unordered_set>
#include <cstdint>
#include <iterator>
#include <memory>

#include "IComponent.hh"
#include "IEntityCollection.hh"

namespace hs {

    class EntitySet : public IEntityCollection {
    public:
        EntitySet();

        explicit EntitySet(std::unordered_set<Entity> entities);

        EntitySet(std::initializer_list<Entity> entities);

        EntitySet With(const IEntityCollection &coll) const;

        bool Has(const Entity &entity) const override;

        std::unordered_set<Entity>::iterator begin();

        std::unordered_set<Entity>::iterator end();

    private:
        std::unordered_set<Entity> entities;
    };

    static_assert(std::ranges::range<EntitySet>);

}