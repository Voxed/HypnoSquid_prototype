#pragma once

#include <unordered_set>
#include <string>

#include "World.hh"
#include "MessageBus.hh"
#include "MessageBusListener.hh"
#include "MessageBusListenerBuilder.hh"
#include "ComponentStoreListenerBuilder.hh"

#include "metadata.hh"

namespace hs {

    class ISystem {
    public:
        virtual void Configure(hs::MessageBusListenerBuilder &msl, ComponentStoreListenerBuilder &csls) = 0;

        virtual void Start() = 0;

        virtual ~ISystem() = default;

    protected:
        hs::MessageBus &Bus() {
            if (!started)
                throw "Attempted to access message bus before system start.";
            return *messageBus;
        }

        hs::World &World() {
            if (!started)
                throw "Attempted to access world before system start.";
            return *world;
        }

    private:
        void initialize(hs::World &world, hs::MessageBus &messageBus) {
            started = true;
            this->world = &world;
            this->messageBus = &messageBus;
        }

        hs::World *world = nullptr;
        hs::MessageBus *messageBus = nullptr;
        bool started = false;

        friend class Engine;
    };

    template<class T>
    concept System =
    std::is_base_of<ISystem, T>::value &&
    HS_HAS_ATTR(T, module_name) &&
    HS_HAS_ATTR(T, system_name);

#define HS_SYSTEM(module, system) \
    HS_ATTR(module_name, module)  \
    HS_ATTR(system_name, system)

}