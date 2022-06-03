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

        void SetMatrices(render::Render& r)
        {
            Transform& transform = GetComponent<Transform>();
            Matrix4x4 view = Matrix4x4(transform.rotation);
            view[3].xyz = transform.position;

            Matrix4x4 proj = glm::perspectiveLH_ZO(glm::radians(fieldOfView), aspectRatio, 0.1f, 100.f);

            r.SetViewTransform(view, proj);
        }
    };
}