#include "Engine.h"
#include "Scene/Test/TestScene.h"
#include "Systems/TestSystem.h"
#include "Systems/TestSystem2.h"
#include "Systems/TestSystem3.h"
#include "Systems/TestSystem4.h"

int main()
{
    Engine engine;
    engine.LoadScene<TestScene>();

    auto& systemScheduler = engine.GetSystemScheduler();

    systemScheduler.RegisterSystem<TestSystem4>("TestSystem4", {}, {"TestSystem3"});
    systemScheduler.RegisterSystem<TestSystem2>("TestSystem2", {}, {"TestSystem"});
    systemScheduler.RegisterSystem<TestSystem>("TestSystem");
    systemScheduler.RegisterSystem<TestSystem3>("TestSystem3", {}, {"TestSystem2"});;

    engine.Run();

    return 0;
}
