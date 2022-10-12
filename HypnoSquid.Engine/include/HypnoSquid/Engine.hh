#pragma once

#include "World.hh"
#include <memory>
#include "ISystem.hh"
#include <iostream>
#include "MessageBus.hh"
#include "MessageBusListenerBuilder.hh"

namespace hs
{
    class EngineBuilder;

    class Engine
    {
    public:
        void Start();

    private:
        Engine(std::vector<std::unique_ptr<ISystem>> systems, MessageBusListenerBuilder msl, ComponentStoreListenerBuilder csls);

        World world;
        MessageBus messageBus;
        std::vector<std::unique_ptr<ISystem>> systems;

        friend EngineBuilder;
    };

    class EngineBuilder
    {
    private:
        struct SystemInfo
        {
            std::string name;
            std::string moduleName;
            bool final = false;
        };

    public:
        template <typename T, typename... Args>
        requires System<T>
            EngineBuilder &Emplace(Args... args)
        {
            SystemInfo systemInfo;
            if constexpr (HS_HAS_TAG(T, FinalSystem))
                systemInfo.final = true;
            systemInfo.name = HS_GET_ATTR(T, system_name);
            systemInfo.moduleName = HS_GET_ATTR(T, module_name);
            add(std::make_unique<T>(args...), systemInfo);
            return *this;
        }

        template <typename... Ts>
        EngineBuilder &EmplaceAll()
        {
            ([&]
             { Emplace<Ts>(); }(),
             ...);

            return *this;
        }

        Engine Build()
        {
            MessageBusListenerBuilder msl;
            ComponentStoreListenerBuilder csls;

            for (auto &s : this->systems)
            {
                s->Configure(msl, csls);
            }


            return Engine(std::move(systems), std::move(msl), std::move(csls));
        }

    private:
        void add(std::unique_ptr<ISystem> system, const SystemInfo &systemInfo);

        bool finalized = false;
        std::vector<std::unique_ptr<ISystem>> systems;
        std::unordered_map<std::string, std::unordered_set<std::string>> addedSystems;
    };

}