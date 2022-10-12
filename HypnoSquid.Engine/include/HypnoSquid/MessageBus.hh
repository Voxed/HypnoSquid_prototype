#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include "IMessage.hh"
#include <functional>
#include "MessageBusListener.hh"

namespace hs
{

    class MessageBus
    {
        MessageBusListener listener;
    public:

        MessageBus(MessageBusListener listener) : listener(std::move(listener)) {

        }

        template <typename T>
        requires Message<T>
        void Send(const T &message)
        {
            listener.On(message);
        }

    };

}