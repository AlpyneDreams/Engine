#pragma once

#include "common/Common.h"
#include "common/Reflection.h"
#include "imgui/Window.h"
#include "editor/Selection.h"
#include "entity/Entity.h"
#include "entity/components/Transform.h"
#include "imgui/Common.h"
#include "editor/Icons.h"

#include "console/Console.h"
#include "math/Math.h"

#include <misc/cpp/imgui_stdlib.h>
#include <string>
#include <type_traits>

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

            for (auto&& [id, component] : ent.GetComponents())
            {
                using namespace refl;

                Class* cls = Class::Get(id);

                if (!cls) {
                    ImGui::Text("Component: %u", id);
                    continue;
                }

                // Draw component inspector, remove component if removed
                if (!DrawComponentInspector(*cls, ent, component))
                {
                    ent.RemoveComponent(id);
                    continue;
                }
            }
            
            if (ImGui::Button("Add Component")) {
                ent.AddComponent<Transform>();
            }
        }

        bool DrawComponentInspector(refl::Class& component, Entity& ent, Component* obj)
        {
            using namespace refl;

            // Create a unique ID for each component on each entity version
            // so that every unique component has its own collapsing header.
            auto id = std::to_string((uintmax_t)ent.handle.entity());
            auto ver = std::to_string(ent.handle.registry()->current(ent));
            auto label = std::string(component.displayName) + "##" + id + "/" + ver;

            if (ComponentIcons.contains(component.type.hash())) {
                label = ComponentIcons[component.type.hash()] + " " + label;
            }

            bool visible = true;
            bool expanded = ImGui::CollapsingHeader(label.c_str(), &visible, ImGuiTreeNodeFlags_DefaultOpen);

            // X button pressed - remove component
            if (!visible) {
                ent.RemoveComponent(component.type.hash());
                return false;
            }

            if (!expanded)
                return true;

            // TODO: Custom inspector registration system
            switch (component.type.hash())
            {
                case TypeHash<Transform>:
                    Inspect((Transform*)obj);
                    break;
                default:
                    DrawComponent(component, ent, obj);
                    break;
            }
            
            return true;
        }
    
    protected:

        // Default Inspector
        void DrawComponent(refl::Class& component, Entity& ent, void* obj)
        {
            using namespace refl;
            for (Field& field : component.fields) {
                ImGui::PushID(&field);
                Input(field.displayName, field.type.hash(), field.GetPointer<void>(obj));
                ImGui::PopID();
            }
        }

        void Inspect(auto* t) { Inspect(*t); }

        // Basic Custom Inspectors

        void Inspect(Transform& t)
        {
            Input("Position", t.position);

            Vector3 euler = t.GetEulerAngles();
            if (Input("Rotation", euler)) {
                t.SetEulerAngles(euler);
            }

            Input("Scale", t.scale);
        }
    public:

        static bool Input(const char* name, auto& ref)
        {
            return Input(name, refl::TypeHash<std::remove_reference_t<decltype(ref)>>, &ref);
        }

        static bool Input(const char* name, refl::Hash type, void* ptr)
        {
            using namespace refl;

            GUI::ItemLabel(name);

            #define INPUT_POINTER(Type, Value, Input) \
                case TypeHash<Type>:                  \
                    return ImGui::Input(name, (Value*)ptr);
            
            #define INPUT_TYPE(Type, Input) INPUT_POINTER(Type, Type, Input)

            #define INPUT_SCALAR(Type, Scalar) \
                case TypeHash<Type>:           \
                    return ImGui::InputScalar(name, ImGuiDataType_ ## Scalar, (Type*)ptr);
            
            switch (type)
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
                    Quaternion* q = (Quaternion*)ptr;
                    // TODO: Improve euler -> quaternion -> euler roundtrip
                    Vector3 angles = glm::eulerAngles(*q);
                    Vector3 d = glm::degrees(angles);
                    if (ImGui::InputFloat3(name, &d[0])) {
                        *q = Quaternion(glm::radians(d));
                        return true;
                    }
                    return false;
                }
                case TypeHash<Rect>: {
                    Rect* rect = (Rect*)ptr;

                    bool changed = false;
                    if (ImGui::BeginTable(name, 2))
                    {
                        ImGui::TableNextColumn();
                        changed |= ImGui::InputFloat("X", &rect->x);
                        ImGui::TableNextColumn();
                        changed |= ImGui::InputFloat("Y", &rect->y);
                        ImGui::TableNextColumn();
                        changed |= ImGui::InputFloat("W", &rect->w);
                        ImGui::TableNextColumn();
                        changed |= ImGui::InputFloat("H", &rect->h);

                        ImGui::EndTable();
                    }

                    return changed;
                }
                default: {
                    ImGui::Text("%s", name);
                    return false;
                }
            }
        }
    };
}
