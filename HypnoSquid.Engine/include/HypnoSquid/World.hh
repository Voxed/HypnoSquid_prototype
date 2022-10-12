#pragma once

#include "Entity.hh"

#include <cstdint>
#include "IComponent.hh"

#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <cstdint>
#include "ComponentStore.hh"
#include <vector>
#include <iostream>
#include <cstdio>
#include <functional>
#include "metadata.hh"

namespace hs
{

    class World
    {
    public:
        Entity CreateEntity();

    private:
        decltype(Entity::id) nextEntityId;

    public:
        template <typename T>
        requires Component<T>
            ComponentStore<T>
        &GetComponentStore()
        {
            return static_cast<ComponentStore<T> &>(getComponentStore(HS_GET_ATTR(T, module_name), HS_GET_ATTR(T, component_name), std::make_unique<ComponentStore<T>>));
        }

        template <typename... Args>
        void GetComponentStores(ComponentStore<Args> *&...args)
        {
            ([&]
             { args = &GetComponentStore<Args>(); }(),
             ...);
        }

    private:
        ComponentStore<IComponent> &getComponentStore(const std::string &module_name, const std::string &component_name, std::function<std::unique_ptr<ComponentStore<IComponent>>()> data);
        void addComponentStore(const std::string &module_name, const std::string &component_name, std::unique_ptr<ComponentStore<IComponent>> data);

        std::unordered_map<std::string, std::unordered_map<std::string, std::unique_ptr<ComponentStore<IComponent>>>> componentIds;
    };

}