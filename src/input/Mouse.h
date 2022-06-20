#pragma once

#include "math/Math.h"
#include "input/InputMap.h"

namespace engine
{
    enum class MouseButton {
        Left = 0, Right = 1, Middle = 2, X1 = 3, X2 = 4, Max = 64
    };
    
    /** Interface to mouse input.
     *
     * To control cursor appearence and behavior
     * use engine::Cursor in platform/Cursor.h
     */
    inline struct Mouse
    {
        using enum MouseButton;

        // True if mouse button is currently held down
        bool GetButton(MouseButton btn) const { return buttons.GetButton(btn); }
        // True if mouse button was depressed this frame
        bool GetButtonDown(MouseButton btn) const { return buttons.GetButtonDown(btn); }
        // True if mouse button was released this frame
        bool GetButtonUp(MouseButton btn) const { return buttons.GetButtonUp(btn); }

        // Relative (delta) motion this frame
        Vector2 GetMotion() const { return motion; }

        void SetButton(MouseButton btn, bool down) { buttons.SetButton(btn, down); }
        void SetMotion(Vector2 delta) { motion = delta; }

        void Update()
        {
            buttons.Update();

            // Clear mouse delta
            motion = Vector2(0, 0);
        }

    private:
        PressMap<MouseButton> buttons;
        Vector2 motion;

    } Mouse;

    // TODO: Gamepad(s)
}