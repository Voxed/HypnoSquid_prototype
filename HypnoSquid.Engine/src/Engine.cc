#include "HypnoSquid/Engine.hh"

namespace hs
{

    Engine::Engine(std::vector<std::unique_ptr<ISystem>> systems) : systems(std::move(systems)) {
        for(auto &s : this->systems) {
            s->world = &world;
            s->messageBus = &messageBus;
        }
    }

    void Engine::Start()
    {
        std::cout << "Engine started." << std::endl;
        for (auto &s : systems)
        {
            s->Start();
        }
    }

    void EngineBuilder::add(std::unique_ptr<ISystem> system, const SystemInfo &systemInfo)
    {
        if (addedSystems[systemInfo.moduleName].contains(systemInfo.name))
            throw "System " + systemInfo.moduleName + " already added!";

        addedSystems[systemInfo.moduleName].insert(systemInfo.name);

        if (finalized)
            throw "Engine has already been finalized.";
            
        finalized |= systemInfo.final;

        std::cout << "[S] System \"" << systemInfo.name << "\" added!" << std::endl;

        systems.push_back(std::move(system));
    }

}