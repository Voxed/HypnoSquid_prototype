#include "HypnoSquid.Core/SystemMainLoop.hh"

#include <thread>

namespace hs::core
{

    void SystemMainLoop::Start()
    {
        auto prevTick = std::chrono::high_resolution_clock::now();
        while (true)
        {
            std::chrono::duration<double> delta = std::chrono::high_resolution_clock::now() - prevTick;
            prevTick = std::chrono::high_resolution_clock::now();
            Bus().Send(MessageUpdate{
                .delta = delta.count()});

            // TODO: Should not be here. Just for testing!!
            // Loop should run unbounded, other systems should take care of blocking until frame times etc...
            std::this_thread::sleep_for(std::chrono::milliseconds(16) - (std::chrono::high_resolution_clock::now() - prevTick));
        }
    }

}