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
            using namespace hlslpp;

            Transform& transform = GetComponent<Transform>();
            float4x4 view = float4x4(transform.rotation);
            view._m30_m31_m32 = transform.position;

            frustum f = frustum::field_of_view_y(radians(float1(fieldOfView)), aspectRatio, 0.1f, 100.f);
            float4x4 proj = float4x4::perspective(projection(f, zclip::zero));

            r.SetViewTransform(view, proj);
        }
    };
}