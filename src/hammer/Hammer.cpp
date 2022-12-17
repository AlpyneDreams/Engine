
#include "hammer/Hammer.h"
#include "hammer/KeyValues.h"

#include "console/Console.h"
#include "engine/Engine.h"
#include "imgui/ConsoleWindow.h"
#include "hammer/gui/Layout.h"
#include "hammer/gui/Viewport.h"

#include "common/Filesystem.h"

#include <cstring>


namespace engine::hammer
{
    void Hammer::Open(const char* path)
    {
        Console.Log("Open: '{}'", path);
        if (!fs::exists(path)) {
            Console.Error("Error: file '{}' does not exist", path);
            return;
        }
        
        std::string vmf = fs::readFile(path);

        // TODO: Use rain to fill this struct!

        KeyValues kv = KeyValues::Parse(vmf);
        auto& versioninfo = kv["versioninfo"];

        map = VMF();
        map.editorversion = versioninfo["editorversion"];
        map.editorbuild = versioninfo["editorbuild"];
        map.mapversion = versioninfo["mapversion"];
        map.formatversion = versioninfo["formatversion"];
        map.prefab = versioninfo["prefab"];

        // For each solid
        for (auto& solid : kv["world"].each("solid"))
        {
            Console.Log("solid({})", solid["id"]);
            for (auto& side : solid.each("side"))
            {
                Console.Log("side({})", side["id"]);
            }
        }

        /*auto& world = root.childs["world"];
        
        auto versioninfo = root.childs["versioninfo"];
        map = VMF();
        map.editorversion = std::stoi(versioninfo->attribs["editorversion"]);
        map.editorbuild = std::stoi(versioninfo->attribs["editorbuild"]);
        map.mapversion = std::stoi(versioninfo->attribs["mapversion"]);*/
        
    }

    void Hammer::Run()
    {
        Editor.Init();

        // Add hammer systems...
        Engine.systems.AddSystem<hammer::Keybinds>();
        Engine.systems.AddSystem<hammer::Layout>();
        Editor.console       = &Engine.systems.AddSystem<GUI::ConsoleWindow>();
        viewport = &Engine.systems.AddSystem<Viewport>();

        Open("/home/alpyne/Desktop/test.vmf");
        Editor.Loop();
        Editor.Shutdown();
    }
}

int main(int argc, char* argv[])
{
    using namespace engine::hammer;
    Hammer.Run();
}
