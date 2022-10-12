#pragma once

#include "IComponent.hh"
#include <functional>
#include "Entity.hh"
#include "ComponentStoreListener.hh"

namespace hs
{

    class ComponentStoreListenerBuilder
    {

    public:
        template <typename T>
        requires Component<T>
        void AddCallback(std::function<void(const Entity&)> creationCb, std::function<void(const Entity&)> destructionCb)
        {
            callbacks[HS_GET_ATTR(T, module_name)][HS_GET_ATTR(T, component_name)].push_back(std::make_pair(creationCb, destructionCb));
        }

        template <typename U, typename T>
        requires Component<U>
        void AddCallback(T *thiz, void (T::*creationCb)(const Entity&), void (T::*destructionCb)(const Entity&))
        {
            AddCallback<U>(
                std::bind(creationCb, thiz, std::placeholders::_1),
                std::bind(destructionCb, thiz, std::placeholders::_1));
        }

    private:
        std::unordered_map<std::string, std::unordered_map<std::string, ComponentStoreListener>> buildListeners() {
            std::unordered_map<std::string, std::unordered_map<std::string, ComponentStoreListener>> listeners;
            for(auto& _p : callbacks)
                for(auto& p : _p.second)
                    listeners[_p.first].insert(std::make_pair(p.first, ComponentStoreListener(p.second)));
            return listeners;
        }


        std::unordered_map<std::string,
                           std::unordered_map<std::string,
                                              std::vector<std::pair<std::function<void(const Entity&)>,
                                                                    std::function<void(const Entity&)>>>>>
            callbacks;

        friend class Engine;
    };

}