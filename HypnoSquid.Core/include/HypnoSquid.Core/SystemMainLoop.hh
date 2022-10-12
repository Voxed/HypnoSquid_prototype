#pragma once

#include <HypnoSquid.hh>

#include <chrono>

namespace hs::core
{

    struct MessageUpdate : public IMessage
    {
        HS_MESSAGE(Core, MessageUpdate)
    
        double delta;
    };

    class SystemMainLoop : public ISystem
    {
    public:
        HS_SYSTEM(Core, SystemMainLoop)
        HS_TAG(FinalSystem)

    private:
        void Start() override;
    };

}