#pragma once

#include "math/Math.h"
#include "Transform.h"
#include "entity/Behavior.h"

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

    public:
        // World to camera matrix
        mat4x4 ViewMatrix()
        {
            // TODO: Cache this
            Transform& transform = GetOrAddComponent<Transform>();
            vec3 a = glm::radians(transform.GetEulerAngles());
            mat4x4 view = glm::eulerAngleZXY(-a.z, -a.x, -a.y);
            view = glm::translate(view, -transform.position);
            return view;
        }

    public:
        // Projection matrix
        mat4x4 ProjMatrix()
        {
            // TODO: Cache this
            return glm::perspectiveLH_ZO(glm::radians(fieldOfView), aspectRatio, near, far);
        }

    public:
        void SetMatrices(render::Render& r)
        {
            auto view = ViewMatrix();
            auto proj = ProjMatrix();
            r.SetViewTransform(view, proj);
        }
    };
}