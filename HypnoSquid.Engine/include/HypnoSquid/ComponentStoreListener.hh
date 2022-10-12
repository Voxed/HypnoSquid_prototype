#pragma once

#include <functional>
#include <vector>
#include "Entity.hh"

namespace hs
{

    class ComponentStoreListener
    {

    public:
        ComponentStoreListener(std::vector<std::pair<std::function<void(const Entity&)>,
                                                     std::function<void(const Entity&)>>>
                                   callbacks) : callbacks(callbacks)
        {
        }

        void OnCreation(const Entity& entity)
        {
            for (const auto &p : callbacks)
            {
                p.first(entity);
            }
        }

        void OnDestruction(const Entity& entity)
        {
            for (const auto &p : callbacks)
            {
                p.second(entity);
            }
        }

    private:
        std::vector<std::pair<std::function<void(const Entity&)>,
                              std::function<void(const Entity&)>>>
            callbacks;
    };

}