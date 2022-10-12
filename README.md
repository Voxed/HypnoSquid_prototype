# I'll let the source speak for itself for now :)

HypnoSquid.Game/src/main.cc
```cpp
#include <HypnoSquid.hh>
#include <HypnoSquid.Core.hh>
#include <string>
#include <iostream>

struct PlayerComponent : public hs::IComponent
{
    HS_COMPONENT(Game, Player)
    float x = 0, y = 0;
};

struct WorldDataComponent : public hs::IComponent
{
    HS_COMPONENT(Game, WorldData)
    HS_TAG(SingletonComponent)

    float gravity = 0;
};

struct TestMessage : public hs::IMessage
{
    HS_MESSAGE(Game, Test)
};

class PhysicsSystem : public hs::ISystem
{
public:
    HS_SYSTEM(Game, Physics)

private:
    hs::ComponentStore<PlayerComponent> *player;

    void Start()
    {
        World().GetComponentStores(player);
        Bus().AddCallback(this, &PhysicsSystem::Update);
    }

    void Update(const hs::core::MessageUpdate &msg)
    {
        for (const auto &p : player->GetEntitiesWith())
            player->Get(p).y += msg.delta;
    }
};

class TestSystem : public hs::ISystem
{
public:
    HS_SYSTEM(Game, Test)

private:
    hs::ComponentStore<PlayerComponent> *player;
    hs::ComponentStore<WorldDataComponent> *worldData;

    void Start() override
    {
        World().GetComponentStores(player, worldData);
        World().CreateEntity().EmplaceTo(*player);
        Bus().AddCallbacks(
            this,
            &TestSystem::Update,
            &TestSystem::Update2);
        World().CreateEntity().EmplaceTo(*worldData);
    }

    void Update(const hs::core::MessageUpdate &msg)
    {
        Bus().Send(TestMessage{});

        if (auto data = worldData->GetSingletonStorage())
        {
            std::cout << data.value()->gravity << std::endl;
            data.value()->gravity += 1;
        }
    }

    void Update2(const TestMessage &msg)
    {
        for (const auto &p : player->GetEntitiesWith())
            std::cout << "Player position again: { x: " << player->Get(p).x
                      << ", y: " << player->Get(p).y << " }" << std::endl;
    }
};

auto main(int argc, const char **argv) -> int
{
    // Assemble and start the engine.
    hs::EngineBuilder()
        .EmplaceAll<
            PhysicsSystem,
            TestSystem,
            hs::core::SystemMainLoop>()
        .Build()
        .Start();
}
```