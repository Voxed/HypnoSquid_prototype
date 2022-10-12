#pragma once

#include <type_traits>
#include "metadata.hh"

namespace hs
{

    class IMessage
    {
    };

    template <class T>
    concept Message =
        std::is_base_of<IMessage, T>::value &&
        HS_HAS_ATTR(T, module_name) &&
        HS_HAS_ATTR(T, message_name);

#define HS_MESSAGE(module, message) \
    HS_ATTR(module_name, module)    \
    HS_ATTR(message_name, message)  

}