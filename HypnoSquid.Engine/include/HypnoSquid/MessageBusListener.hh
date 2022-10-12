#pragma once

#include <unordered_map>
#include "IMessage.hh"
#include <string>
#include <vector>
#include <functional>

namespace hs
{

    class MessageBusListener
    {

    public:
        MessageBusListener(std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::function<void(const IMessage &)>>>> callbacks) : callbacks(callbacks)
        {
        }

        template <typename T>
        requires Message<T>
        void On(const T &message)
        {
            for (const auto &cb : callbacks[HS_GET_ATTR(T, module_name)][HS_GET_ATTR(T, message_name)])
            {
                cb(message);
            }
        }

    private:

        std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::function<void(const IMessage &)>>>> callbacks;

        friend class MessageBus;
    };

}