#include <cstdio>

#include "engine/Engine.h"

using namespace engine;

int main(int argc, char* argv[]) {

    Engine engine;
    engine.Init();
    engine.CreateWindow();
    engine.InitRender();

    // Run the main loop
    engine.Start();

    engine.Shutdown();

    return 0;
}
