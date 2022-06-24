#pragma once

#include "entity/Entity.h"
#include "math/Math.h"

namespace engine
{
    struct Transform : Component
    {
        vec3 position;
        quat rotation;
        vec3 scale;

        void SetEulerAngles(vec3 degrees)
        {
            rotation = quat(glm::radians(degrees));
        #if defined(EDITOR)
            // Cache Euler angles. This avoids weird behavior when performing
            // round trip conversions from Euler -> Quaternion -> Euler in
            // the inspector.
            eulerAngles = degrees;
            eulerAnglesRotation = rotation;
        #endif
        }

        vec3 GetEulerAngles()
        {
        #if defined(EDITOR)
            // Return cached euler angles in edit mode
            // as long as the current rotation has not changed.
            if (rotation == eulerAnglesRotation) [[likely]]
                return eulerAngles;
            
            // If eulerAngles are not cached then recalculate them
            eulerAnglesRotation = rotation;
            eulerAngles = glm::degrees(glm::eulerAngles(rotation));
            return eulerAngles;
        #endif
            // TODO: Normalize quaternion?
            return glm::degrees(glm::eulerAngles(rotation));
        }

    private:
    #if defined(EDITOR)
        // Cached euler angles in degrees
        vec3 eulerAngles;

        // Rotation from cached eulerAngles
        quat eulerAnglesRotation;
    #endif

    };
}