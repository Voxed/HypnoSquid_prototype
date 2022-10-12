#pragma once

#include <unordered_set>
#include <string>

#include "World.hh"
#include "MessageBus.hh"

#include "metadata.hh"

namespace hs
{

    class ISystem
    {
    public:
        virtual void Start() = 0;

    protected:
        hs::MessageBus &Bus() { return *messageBus; }
        hs::World &World() { return *world; }

    private:
        hs::World *world;
        hs::MessageBus *messageBus;

        friend class Engine;
    };

    template <class T>
    concept System =
        std::is_base_of<ISystem, T>::value &&
        HS_HAS_ATTR(T, module_name) &&
        HS_HAS_ATTR(T, system_name);

#define HS_SYSTEM(module, system) \
    HS_ATTR(module_name, module)  \
    HS_ATTR(system_name, system)

}