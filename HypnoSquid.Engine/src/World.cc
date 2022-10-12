#include "HypnoSquid/World.hh"

namespace hs
{

    Entity World::CreateEntity()
    {
        return nextEntityId++;
    }

    ComponentStore<IComponent> &World::getComponentStore(const std::string &module_name, const std::string &component_name, std::function<std::unique_ptr<ComponentStore<IComponent>>()> data)
    {
        if(!componentIds.contains(module_name) || !componentIds[module_name].contains(component_name))
            addComponentStore(module_name, component_name, data());
        return *(componentIds.at(module_name).at(component_name));
    }

    void World::addComponentStore(const std::string &module_name, const std::string &component_name, std::unique_ptr<ComponentStore<IComponent>> data)
    {
        if (!componentIds.contains(module_name))
            componentIds[module_name] = std::unordered_map<std::string, std::unique_ptr<ComponentStore<IComponent>>>();
        componentIds[module_name][component_name] = std::move(data);
        std::cout << "[C] Component type \"" << component_name << "\" added!" << std::endl;
    }

}