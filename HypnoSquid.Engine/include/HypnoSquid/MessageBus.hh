#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include "IMessage.hh"
#include <functional>

namespace hs
{

    class MessageBus
    {
    public:
        template <typename T>
        requires Message<T>
        void AddCallback(std::function<void(const T &)> cb)
        {
            callbacks[HS_GET_ATTR(T, module_name)][HS_GET_ATTR(T, message_name)].push_back([cb](const IMessage &rawMessage)
                                                                                           { cb(static_cast<const T &>(rawMessage)); });
        }

        template <typename U, typename T>
        requires Message<U>
        void AddCallback(T *thiz, void (T::*cb)(const U &))
        {
            AddCallback<U>(std::bind(cb, thiz, std::placeholders::_1));
        }

        template <typename T, typename... U>
        void AddCallbacks(T *thiz, void (T::*...cb)(const U &))
        {
            ([&]
             { AddCallback<U, T>(thiz, cb); }(),
             ...);
        }

        template <typename T>
        requires Message<T>
        void Send(const T &message)
        {
            for (const auto &cb : callbacks[HS_GET_ATTR(T, module_name)][HS_GET_ATTR(T, message_name)])
            {
                cb(message);
            }
        }

    private:
        std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::function<void(const IMessage &)>>>> callbacks;
    };

}