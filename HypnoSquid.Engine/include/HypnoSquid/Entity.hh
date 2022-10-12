#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include "IComponent.hh"

namespace hs
{
    class Entity;
}

template <>
struct std::hash<hs::Entity>;

namespace hs
{
    template <typename T>
    class ComponentStore;

    class World;

    class Entity
    {
    public:
        operator uint64_t() const { return id; }

        /**
         * Convenience function to emplace the entity on a store.
         */
        template <typename T, typename... Args>
        requires Component<T>
            Entity &EmplaceTo(ComponentStore<T>& cs, Args... args)
        {
            cs.Emplace(*this, args...);
            return *this;
        }

        /**
         * Convenience function to add the entity to a store.
         */
        template <typename T, typename... Args>
        requires Component<T>
            Entity &AddTo(ComponentStore<T>& cs, std::unique_ptr<T> component)
        {
            cs.Add(*this, std::move(component));
            return *this;
        }

    private:
        Entity(uint64_t id) : id(id) {}

        uint64_t id;

        friend World;
        friend std::hash<Entity>;
        /*friend bool operator==(const Entity &lhs, const Entity &rhs)
        {
            return lhs.id == rhs.id;
        }*/
        // dk if this is necessary for unordered_map?
    };

}

template <>
struct std::hash<hs::Entity>
{
    std::size_t operator()(hs::Entity const &s) const noexcept
    {
        return std::hash<uint64_t>{}(s.id);
    }
};