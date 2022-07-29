#pragma once

#include "entity/Common.h"
#include "entity/Entity.h"

namespace engine::editor
{
    // TODO: Multiple selections.
    inline struct Selection
    {
    private:
        EntityID selected = EntityNull;

    public:
        EntityID Active() {
            return selected;
        }

        bool Selected(EntityID ent) {
            return selected == ent;
        }

        bool Empty() {
            return selected == EntityNull;
        }

        void Select(EntityID ent) {
            selected = ent;
        }

        void Deselect(EntityID ent) {
            if (selected == ent) {
                selected = EntityNull;
            }
        }

        void Clear() {
            selected = EntityNull;
        }

        void Toggle(EntityID ent) {
            if (selected == ent) {
                selected = EntityNull;
            } else {
                selected = ent;
            }
        }
    } Selection;
}