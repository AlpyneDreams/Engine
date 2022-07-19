#pragma once

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

        void Select(EntityID ent) {
            selected = ent;
        }

        void Deselect(EntityID ent) {
            if (selected == ent) {
                selected = EntityNull;
            }
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