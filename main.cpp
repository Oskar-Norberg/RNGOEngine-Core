#include "Engine.h"
#include "Scene/Test/TestScene.h"

int main()
{
    Engine engine;
    engine.LoadScene<TestScene>();

    engine.Run();

    return 0;
}
