#pragma once

#include "common/Common.h"
#include "common/Reflection.h"
#include "imgui/Window.h"
#include "editor/Selection.h"
#include "entity/Entity.h"
#include "entity/components/Transform.h"
#include "imgui/Common.h"

#include "console/Console.h"

#include <misc/cpp/imgui_stdlib.h>

namespace engine::editor
{
    struct Inspector : public GUI::Window
    {
        Inspector() : GUI::Window("Inspector", 512, 512, true) {}

        void Draw() override
        {
            Entity ent = Selection.Active();
            if (!ent)
                return;

            ImGui::Text("Entity %d", ent.handle.entity());

            for (auto&& [id, storage] : ent.handle.registry()->storage())
            {
                using namespace reflect;

                if (!storage.contains(ent))
                    continue;
                
                void* obj = storage.get(ent);

                switch (id)
                {
                    case TypeHash<Transform>:
                        DrawComponent<Transform>(ent, storage.get(ent));
                        break;
                    default:
                        ImGui::Text("Component: %lld", id);
                        break;
                }
            }
            
            if (ImGui::Button("Add Component")) {
                ent.AddComponent<Transform>();
            }
            
        }

        template <class T>
        void DrawComponent(Entity& ent, void* obj)
        {
            using namespace rtti;
            Class component = RTTI<T>;

            static bool visible = true;
            if (!ImGui::CollapsingHeader(component.displayName, &visible, ImGuiTreeNodeFlags_DefaultOpen)) {
                return;
            }

            // X button pressed - remove component
            if (!visible) {
                ent.RemoveComponent<T>();
                visible = true;
                return;
            }

            for (Field& field : component.fields)
            {
                auto name = field.displayName;

                GUI::ItemLabel(name);

                #define INPUT_POINTER(Type, Value, Input) \
                    case TypeHash<Type>: {      \
                        ImGui::Input(name, field.GetPointer<Value>(obj)); \
                        break;                  \
                    }
                
                #define INPUT_TYPE(Type, Input) INPUT_POINTER(Type, Type, Input)

                #define INPUT_SCALAR(Type, Scalar) \
                    case TypeHash<Type>: {      \
                        ImGui::InputScalar(name, ImGuiDataType_ ## Scalar, field.GetPointer<Type>(obj)); \
                        break;                  \
                    }
                
                switch (field.type.hash())
                {
                    INPUT_TYPE(float, InputFloat);
                    INPUT_TYPE(double, InputDouble);
                    INPUT_TYPE(std::string, InputText);
                    INPUT_SCALAR(int8,   S8);
                    INPUT_SCALAR(uint8,  U8);
                    INPUT_SCALAR(int16,  S16);
                    INPUT_SCALAR(uint16, U16);
                    INPUT_SCALAR(int32,  S32);
                    INPUT_SCALAR(uint32, U32);
                    INPUT_SCALAR(int64,  S64);
                    INPUT_SCALAR(uint64, U64);
                    INPUT_POINTER(Vector4, float, InputFloat4);
                    INPUT_POINTER(Vector3, float, InputFloat3);
                    INPUT_POINTER(Vector2, float, InputFloat2);

                    case TypeHash<Quaternion>: {
                        // TODO: Euler angles
                        ImGui::InputFloat3(name, field.GetPointer<float>(obj));
                        break;
                    }
                    default: {
                        ImGui::Text(name);
                        break;
                    }
                }
            }
        }

    };
}
