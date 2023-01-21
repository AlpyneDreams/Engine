#include "View3D.h"

namespace engine::editor
{
    struct SceneView : public View3D
    {
        SceneView() : View3D(ICON_MC_IMAGE_SIZE_SELECT_ACTUAL, "Scene", 512, 512, true) {}   
    };
}
