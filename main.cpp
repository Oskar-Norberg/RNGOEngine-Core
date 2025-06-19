#include "Engine.h"
#include "Scene/Test/TestScene.h"
#include "Systems/TestSystem.h"
#include "Systems/TestSystem2.h"

int main()
{
    Engine engine;
    engine.LoadScene<TestScene>();

    auto& systemScheduler = engine.GetSystemScheduler();
    systemScheduler.RegisterSystem<TestSystem>("TestSystem");
    systemScheduler.RegisterSystem<TestSystem2>("TestSystem2", {}, {"TestSystem"});

    engine.Run();

    return 0;
}
