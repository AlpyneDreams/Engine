#pragma once

#include "math/Math.h"
#include "Transform.h"
#include "entity/Behavior.h"

#include "platform/Window.h"
#include "render/Render.h"

namespace engine
{
    struct Camera : Behavior, RequireComponent<Transform>
    {
        // Horizontal field of view in degrees
        float fieldOfView = 90.0f;

        // Aspect ratio W/H
        float aspectRatio = 16.0f / 9.0f;

        float near = 0.1f;
        float far  = 1000.f;

        // If not null then camera only renders to this texture
        render::RenderTarget* renderTarget = nullptr;

    public:
        // World to camera matrix
        mat4x4 ViewMatrix()
        {
            if (overrideViewMatrix)
                return view;
            
            // TODO: Cache this
            Transform& transform = GetOrAddComponent<Transform>();
            vec3 a = glm::radians(transform.GetEulerAngles());
            mat4x4 view = glm::eulerAngleZXY(-a.z, -a.x, -a.y);
            view = glm::translate(view, -transform.position);
            return view;
        }

        // Set custom world to camera matrix
        void SetViewMatrix(mat4x4& m)
        {
            overrideViewMatrix = true;
            view = m;
        }

        void ResetViewMatrix() {
            overrideViewMatrix = false;
        }

    public:
        float AspectRatio()
        {
            vec2 size;
            if (renderTarget) {
                size = renderTarget->GetSize();
            } else {
                size = Window::main->GetSize();
            }

            return size.x / size.y;
        }

        // Projection matrix
        mat4x4 ProjMatrix()
        {
            if (overrideProjMatrix)
                return proj;

            // TOOD: Cache this
            aspectRatio = AspectRatio();
            proj = glm::perspectiveLH_ZO(glm::radians(fieldOfView), aspectRatio, near, far);
            return proj;
        }

        // Set custom projection matrix
        void SetProjMatrix(mat4x4& m)
        {
            overrideProjMatrix = true;
            view = m;
        }

        void ResetProjMatrix() {
            overrideProjMatrix = false;
        }

    public:
        void SetMatrices(render::Render& r)
        {
            auto view = ViewMatrix();
            auto proj = ProjMatrix();
            r.SetViewTransform(view, proj);
        }

    private:
        // World to camera matrix
        mat4x4 view;

        // Projection matrix
        mat4x4 proj;

        bool overrideViewMatrix = false;
        bool overrideProjMatrix = false;

    };
}