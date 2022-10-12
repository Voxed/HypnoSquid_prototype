#pragma once

#include "Entity.hh"

#include <unordered_set>
#include <cstdint>
#include <iterator>
#include <memory>

#include "ComponentStore.hh"
#include "IComponent.hh"

namespace hs
{

    class EntitySet
    {
    public:
        EntitySet(std::unordered_set<Entity> entities);
        EntitySet(std::initializer_list<Entity> entities);
        EntitySet GetWith(ComponentStore<IComponent> &componentType);
        std::unordered_set<Entity>::iterator begin();
        std::unordered_set<Entity>::iterator end();

    private:
        std::unordered_set<Entity> entities;
    };
    static_assert(std::ranges::range<EntitySet>);

}