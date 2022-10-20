/// \file ComponentStore.hh
/// \author Voxed (https://www.github.com/voxed)

#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <optional>

#include "ComponentStoreListener.hh"
#include "Entity.hh"
#include "IComponent.hh"
#include "utility.hh"
#include "EntitySet.hh"

#include "IEntityCollection.hh"

namespace hs {
    template<typename T>
    struct ComponentStore;

    /// A component store containing relations between entities and a singular type of component.
    template<>
    struct ComponentStore<IComponent> : public IEntityCollection{
    public:
        /// Check whether or not an entity has a relation inside the component store.
        /// \param entity [in] The entity to check existence for.
        /// \return A bool representing whether or not the relation exists.
        bool Has(const Entity &entity) const override;

        /// Get all entities with relations inside of the component store.
        /// \return The entities with relations.
        virtual EntitySet All() const;

        /// Get the component data related to an entity within the component store.
        /// \param entity [in] The related entity.
        /// \return The related component data.
        virtual IComponent &Get(const Entity &entity);

        /// Get the constant component data related to an entity within a constant component store.
        /// \param entity [in] The related entity.
        /// \return The constant related component data.
        virtual const IComponent &Get(const Entity &entity) const;

        /// Remove the component relation within the component store containing a specified entity.
        /// \param entity [in] The entity which is contained within the relation.
        /// \return A bool representing whether or not the entity <u>previously</u> existed within the component store.
        virtual bool Remove(const Entity &entityId);
    protected:
        explicit ComponentStore(ComponentStoreListener csl);

        bool add(Entity entity, std::unique_ptr<IComponent> data);

        bool addSingleton(Entity entity, std::unique_ptr<IComponent> data);

        std::optional<Entity> getSingletonEntity();

        std::optional<const IComponent *> getSingletonStorage() const;

        std::optional<IComponent *> getSingletonStorage();

        std::unordered_map<Entity, std::unique_ptr<IComponent>> storage;

        ComponentStoreListener csl;
    };

    /// A component store containing relations between entities and a singular type of component.
    /// \tparam T The component type contained within the relations of this component store.
    template<typename T>
    struct ComponentStore : ComponentStore<IComponent> {
    public:
        /// Construct a component store.
        /// \param csl [in] A listener which will receive events from the component store.
        explicit ComponentStore(ComponentStoreListener csl) : ComponentStore<IComponent>(std::move(csl)) {};

        /// Add a component relation to the component store between an entity and some component data.
        /// \param entity [in] The entity to be contained within the relation.
        /// \param data [in] The component data to be contained within the relation. Ownership will be transferred.
        /// \return A bool representing whether or not the entity <u>previously</u> did <u>not</u> exist within the
        /// component store.
        bool Add(const Entity &entity, std::unique_ptr<T> data) {
            return ComponentStore<IComponent>::add(entity, std::move(data));
        }

        /// Emplace a component relation into the component store between an entity and some component data.
        /// \tparam Args Variadic argument types.
        /// \param entity [in] The entity to be contained within the relation.
        /// \param args [in] The arguments to pass to the component data constructor which will be contained within the
        /// relation.
        /// \return A bool representing whether or not the entity <u>previously</u> did <u>not</u> exist within the
        /// component store.
        template<typename... Args>
        bool Emplace(const Entity &entity, Args... args) {
            if constexpr (HS_HAS_TAG(T, SingletonComponent))
                return ComponentStore<IComponent>::addSingleton(entity, std::make_unique<T>(args...));
            return ComponentStore<IComponent>::add(entity, std::make_unique<T>(args...));
        }

        /// Get the component data related to an entity within a component store.
        /// \param entityId [in] The related entity.
        /// \return The related component data.
        T &Get(const Entity &entityId) override {
            return *static_cast<T *>(&ComponentStore<IComponent>::Get(entityId));
        }

        /// Get the constant component data related to an entity within a constant component store.
        /// \param entityId [in] The related entity.
        /// \return The constant related component data.
        const T &Get(const Entity &entityId) const override {
            return *static_cast<const T *>(&ComponentStore<IComponent>::Get(entityId));
        }

        /// Get the entity contained within the single relation of this singleton component store.
        /// \return The entity contained within the single relation, {} if no relation exists.
        std::optional<Entity> GetSingletonEntity() {
            static_assert(HS_HAS_TAG(T, SingletonComponent),
                    "Attempted to get singleton entity from a non singleton component store.");

            return getSingletonEntity();
        }

        /// Get the component data contained within the single relation of this singleton component store.
        /// \return The component data contained within the single relation, {} if no relation exists.
        std::optional<T *> GetSingletonStorage() {
            static_assert(HS_HAS_TAG(T, SingletonComponent),
                    "Attempted to get singleton storage from a non singleton component store.");

            return ComponentStore<IComponent>::getSingletonStorage().transform(staticCast<T *, IComponent *>);
        }

        /// Get the component data contained within the single relation of this singleton component store.
        /// \return The component data contained within the single relation, {} if no relation exists.
        std::optional<const T *> GetSingletonStorage() const {
            static_assert(HS_HAS_TAG(T, SingletonComponent),
                    "Attempted to get singleton storage from a non singleton component store.");

            return ComponentStore<IComponent>::getSingletonStorage().transform(
                    staticCast<const T *, const IComponent *>);
        }
    };

    /// Alias for ComponentStore<T>
    template<typename T>
    using CS = ComponentStore<T>;
}