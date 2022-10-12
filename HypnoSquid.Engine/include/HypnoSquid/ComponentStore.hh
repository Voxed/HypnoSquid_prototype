#pragma once

#include "Entity.hh"
#include "IComponent.hh"
#include <cstdio>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <optional>
#include "ComponentStoreListener.hh"

namespace hs
{
    template <typename T>
    struct ComponentStore;

    template <>
    struct ComponentStore<IComponent>
    {
    public:
        bool Has(const Entity &entityId) const;
        std::unordered_set<Entity> GetEntitiesWith();
        IComponent &Get(const Entity &entityId);
        const IComponent &Get(const Entity &entityId) const;
        bool Remove(const Entity &entityId);

    protected:
        ComponentStore(ComponentStoreListener csl);

        bool add(Entity entityId, std::unique_ptr<IComponent> data);

        std::unordered_map<Entity, std::unique_ptr<IComponent>> storage;

        ComponentStoreListener csl;
    };

    template <typename T>
    struct ComponentStore : ComponentStore<IComponent>
    {
    public:
        ComponentStore(ComponentStoreListener csl) : ComponentStore<IComponent>(std::move(csl)){};

        bool Add(const Entity &entityId, std::unique_ptr<T> data)
        {
            bool returnCode = ComponentStore<IComponent>::add(entityId, std::move(data));

            return returnCode;
        }

        template <typename... Args>
        bool Emplace(const Entity &entityId, Args... args)
        {
            // Might slow down a bit, but add shouldnt be spammed on singletons anyway so should be OK.
            if constexpr (HS_HAS_TAG(T, SingletonComponent))
                if (storage.size() >= 1)
                    throw "Attempted to add more than one entity to a singleton component store.";

            bool returnCode = ComponentStore<IComponent>::add(entityId, std::make_unique<T>(args...));


            return returnCode;
        }

        T &Get(const Entity &entityId)
        {
            return static_cast<T &>(ComponentStore<IComponent>::Get(entityId));
        }

        const T &Get(const Entity &entityId) const
        {
            return static_cast<T &>(ComponentStore<IComponent>::Get(entityId));
        }

        bool Has(const Entity &entityId)
        {
            return ComponentStore<IComponent>::Has(entityId);
        }

        bool Remove(const Entity &entityId)
        {
            return ComponentStore<IComponent>::Remove(entityId);
        }

        std::unordered_set<Entity> GetEntitiesWith()
        {
            return ComponentStore<IComponent>::GetEntitiesWith();
        }

        std::optional<Entity> GetSingletonEntity()
        {
            static_assert(HS_HAS_TAG(T, SingletonComponent), "Attempted to get singleton entity from a non singleton component store.");

            if (storage.size() > 0)
                return storage.begin()->first;
            else
                return {};
        }

        std::optional<T *> GetSingletonStorage()
        {
            static_assert(HS_HAS_TAG(T, SingletonComponent), "Attempted to get singleton storage from a non singleton component store.");

            if (storage.size() > 0)
                return static_cast<T *>((storage.begin()->second).get());
            else
                return {};
        }

        /*T &operator[](const Entity& e) { return Get(e); }
        const T &operator[](const Entity& e) const { return Get(e); }*/
    };

    template <typename T>
    using CS = ComponentStore<T>;

}