#include "HypnoSquid/EntitySet.hh"

namespace hs
{

    EntitySet::EntitySet(std::unordered_set<Entity> entities) : entities(entities)
    {
    }

    EntitySet::EntitySet(std::initializer_list<Entity> entities) : EntitySet::EntitySet(std::unordered_set<Entity>(entities))
    {
    }

    EntitySet EntitySet::GetWith(ComponentStore<IComponent> &componentType)
    {
        std::unordered_set<Entity> result;

        auto a = componentType.GetEntitiesWith();
        auto b = this->entities;

        if (a.size() > b.size())
        {
            std::swap(a, b);
        }

        for (auto i : a)
        {
            if (b.contains(i))
                result.insert(i);
        }

        return EntitySet(result);
    }

    std::unordered_set<Entity>::iterator EntitySet::begin()
    {
        return entities.begin();
    }

    std::unordered_set<Entity>::iterator EntitySet::end()
    {
        return entities.end();
    }

}