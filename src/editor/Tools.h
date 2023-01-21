#pragma once

#include "engine/Engine.h"
#include "common/Common.h"
#include "math/Math.h"
#include "render/Render.h"
#include "imgui/Window.h"
#include "entity/components/Camera.h"
#include "entity/components/Transform.h"

namespace engine::editor
{
    /**
     * Editor manages the editor viewport, 
     * editor GUI windows, and general rendering.
     */
    inline class Tools
    {
    
    public:
    // Viewport //
        struct EditorCamera {
            Camera camera;
            Transform transform;
        } editorCamera;
    
        RenderSystem& Renderer = Engine.renderSystem;
        
        render::Shader* sh_Color;
        render::Shader* sh_Grid;

        render::RenderTarget* rt_SceneView;
        render::RenderTarget* rt_ObjectID;

        void ResizeViewport(uint width, uint height)
        {
            rt_SceneView->Resize(width, height);
            rt_ObjectID->Resize(width, height);
        }

        // Read object ID from scene view render target and update selection
        void PickObject(uint2 mouse);
        
        // Draw wireframe outline of selected object
        void DrawSelectionOutline(Mesh* mesh);
        void DrawSelectionOutline(Mesh* mesh, Transform& transform);
    
    // GUI //
        GUI::Window* console;
    
    // Tools Engine Loop //

        void Init();
        void Loop();
        void Shutdown();
        
    private:
        render::Render& r = Engine.Render;

    } Tools;
}