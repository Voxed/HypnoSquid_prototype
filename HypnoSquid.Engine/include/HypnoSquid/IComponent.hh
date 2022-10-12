#pragma once

#include <concepts>
#include <string>

#include "metadata.hh"

namespace hs
{
    class IComponent
    {
    };

    template <class T>
    concept Component =
        std::is_base_of<IComponent, T>::value &&
        HS_HAS_ATTR(T, module_name) &&
        HS_HAS_ATTR(T, component_name);

    /*
     * These defines are 110% syntax sugar. They are in no way necessary.
     */

#define HS_COMPONENT(module, component) \
    HS_ATTR(module_name, module)        \
    HS_ATTR(component_name, component)

}