#include "HypnoSquid/ComponentStore.hh"

namespace hs
{

    ComponentStore<IComponent>::ComponentStore(ComponentStoreListener csl) : csl(csl) {}

    bool ComponentStore<IComponent>::add(Entity entityId, std::unique_ptr<IComponent> data)
    {
        if (storage.contains(entityId))
            return false;

        storage.insert(std::make_pair(entityId, std::move(data)));

        csl.OnCreation(entityId);

        return true;
    }

    IComponent &ComponentStore<IComponent>::Get(const Entity &entityId)
    {
        return *storage[entityId];
    }

    const IComponent &ComponentStore<IComponent>::Get(const Entity &entityId) const
    {
        return *(storage.at(entityId));
    }

    bool ComponentStore<IComponent>::Has(const Entity &entityId) const
    {
        return storage.contains(entityId);
    }

    bool ComponentStore<IComponent>::Remove(const Entity &entityId)
    {
        if (!storage.contains(entityId))
            return false;

        csl.OnDestruction(entityId);

        storage.erase(entityId);

        return true;
    }

    std::unordered_set<Entity> ComponentStore<IComponent>::GetEntitiesWith()
    {
        std::unordered_set<Entity> entities;
        for (const auto &item : storage)
        {
            entities.insert(item.first);
        }
        return entities;
    }

}