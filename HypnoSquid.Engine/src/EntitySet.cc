#include <utility>

#include "HypnoSquid/EntitySet.hh"

namespace hs {
    EntitySet::EntitySet() = default;

    EntitySet::EntitySet(std::unordered_set<Entity> entities) : entities(std::move(entities)) {

    }

    EntitySet::EntitySet(std::initializer_list<Entity> entities) : EntitySet::EntitySet(
            std::unordered_set<Entity>(entities)) {
    }

    std::unordered_set<Entity>::iterator EntitySet::begin() {
        return entities.begin();
    }

    std::unordered_set<Entity>::iterator EntitySet::end() {
        return entities.end();
    }

    bool EntitySet::Has(const Entity &entity) const {
        return entities.contains(entity);
    }

    EntitySet EntitySet::With(const IEntityCollection& coll) const {
        EntitySet copy = *this;
        erase_if(copy.entities, [&coll](const Entity& e) {
            return !coll.Has(e);
        });
        return copy;
    }

}