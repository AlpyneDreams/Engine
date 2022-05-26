#include <cstdio>

#include "engine/Engine.h"
#include "editor/Editor.h"

using namespace engine;
using namespace engine::editor;

int main(int argc, char* argv[]) {

#ifdef EDITOR
    Editor.Run();
#else
    Engine.Run();
#endif

    return 0;
}
