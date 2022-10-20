#include <utility>

#include "HypnoSquid/ComponentStore.hh"

namespace hs {

    ComponentStore<IComponent>::ComponentStore(ComponentStoreListener csl) : csl(std::move(csl)) {}

    bool ComponentStore<IComponent>::add(Entity entity, std::unique_ptr<IComponent> data) {
        if (storage.contains(entity))
            return false;

        storage.insert(std::make_pair(entity, std::move(data)));

        csl.OnCreation(entity);

        return true;
    }

    bool ComponentStore<IComponent>::addSingleton(hs::Entity entity, std::unique_ptr<IComponent> data) {
        if (!storage.empty())
            throw std::length_error("Attempted to add more than one entity to a singleton component store.");

        return add(entity, std::move(data));
    }

    IComponent &ComponentStore<IComponent>::Get(const Entity &entityId) {
        return *storage[entityId];
    }

    const IComponent &ComponentStore<IComponent>::Get(const Entity &entityId) const {
        return *(storage.at(entityId));
    }

    bool ComponentStore<IComponent>::Has(const Entity &entityId) const {
        return storage.contains(entityId);
    }

    bool ComponentStore<IComponent>::Remove(const Entity &entityId) {
        if (!storage.contains(entityId))
            return false;

        csl.OnDestruction(entityId);

        storage.erase(entityId);

        return true;
    }

    EntitySet ComponentStore<IComponent>::All() const {
        std::unordered_set<Entity> entities;
        for (const auto &item: storage) {
            entities.insert(item.first);
        }
        return EntitySet(entities);
    }

    std::optional<Entity> ComponentStore<IComponent>::getSingletonEntity() {
        if (!storage.empty())
            return storage.begin()->first;
        else
            return {};
    }

    std::optional<const IComponent *> ComponentStore<IComponent>::getSingletonStorage() const {
        if (!storage.empty())
            return (storage.begin()->second).get();
        else
            return {};
    }

    std::optional<IComponent *> ComponentStore<IComponent>::getSingletonStorage() {
        if (!storage.empty())
            return (storage.begin()->second).get();
        else
            return {};
    }

}