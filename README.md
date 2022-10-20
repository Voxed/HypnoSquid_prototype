# I'll let the source speak for itself for now :)

HypnoSquid.Game/src/main.cc
```cpp
#include <HypnoSquid.hh>
#include <HypnoSquid.Core.hh>
#include <iostream>

struct PlayerComponent : public hs::IComponent
{
    HS_COMPONENT(Game, Player)
    float x = 0, y = 0;

    friend std::ostream &operator<<(std::ostream &os, const PlayerComponent &s)
    {
        return (os << "PlayerComponent {x: " << s.x << ", y: " << s.y << "}");
    }
};

struct WorldComponent : public hs::IComponent
{
    HS_COMPONENT(Game, WorldData)
    HS_TAG(SingletonComponent)
    float gravity = 0;

    friend std::ostream &operator<<(std::ostream &os, const WorldComponent &s)
    {
        return (os << "WorldComponent {gravity: " << s.gravity << "}");
    }
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
    hs::CS<PlayerComponent> *player;
    hs::CS<WorldComponent> *world;

    void Configure(
        hs::MessageBusListenerBuilder &msl,
        hs::ComponentStoreListenerBuilder &csls) override
    {
        msl.AddCallback(this, &PhysicsSystem::Update);
        csls.AddCallback<PlayerComponent>(
            [this](const hs::Entity &e)
            {
                std::cout << "Created: " << player->Get(e) << std::endl;
            },
            [this](const hs::Entity &e)
            {
                std::cout << "Destroyed: " << player->Get(e) << std::endl;
            });
    }

    void Start()
    {
        World().GetComponentStores(player, world);
    }

    void Update(const hs::core::MessageUpdate &msg)
    {
        for (const auto &p : player->All())
            if (auto data = world->GetSingletonStorage())
                player->Get(p).y += msg.delta * data.value()->gravity;
    }
};

class TestSystem : public hs::ISystem
{
public:
    HS_SYSTEM(Game, Test)
private:
    hs::CS<PlayerComponent> *player;
    hs::CS<WorldComponent> *world;

    void Configure(
        hs::MessageBusListenerBuilder &msl,
        hs::ComponentStoreListenerBuilder &csls) override
    {
        msl.AddCallbacks(this,
                         &TestSystem::Update,
                         &TestSystem::Update2);
    }

    void Start() override
    {
        World().GetComponentStores(player, world);
        World().CreateEntity().EmplaceTo(*player);
        World().CreateEntity().EmplaceTo(*world);
    }

    void Update(const hs::core::MessageUpdate &msg)
    {
        Bus().Send(TestMessage{});
        if (auto data = world->GetSingletonStorage())
        {
            std::cout << *(data.value()) << std::endl;
            data.value()->gravity += 1000 * msg.delta;
        }

        for (const auto &e : player->All())
        {
            if (player->Get(e).y > 2000)
                player->Remove(e);
        }
    }

    void Update2(const TestMessage &msg)
    {
        for (const auto &p : player->All())
            std::cout << player->Get(p) << std::endl;
    }
};

int main()
{
    // Assemble and start the engine.
    hs::EngineBuilder()
        .EmplaceAll<PhysicsSystem,
                    TestSystem,
                    hs::core::SystemMainLoop>()
        .Build()
        .Start();
}
```