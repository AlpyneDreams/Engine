#pragma once

#include "common/Enum.h"
#include "console/Console.h"

namespace engine
{
    // TODO: All the key codes. Should be basically 1:1 with SDL, maybe with some exceptions?
    enum class Key : size_t
    {
        Backspace = '\b', Tab = '\t',
        Return = '\r', Enter = '\r', Escape = 27,
        Grave = '`', BackQuote = '`', BackTick = '`',
        //Zero = '0', One, Two, Three, Four, Five, Six, Seven, Eight, Nine, Minus = '-', Equals = '=',
        A = 'a', B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        Max = 512
    };
    
    inline struct Input
    {
        // Utilities to check keys on any keyboard

        // True if key is currently held down
        bool GetKey(Key key) const { return keys[key]; }
        // True if key was depressed this frame
        bool GetKeyDown(Key key) const { return keysDown[key]; }
        // True if key was released this frame
        bool GetKeyUp(Key key) const { return keysUp[key]; }

    protected:
        // Called by Window::PreUpdate
        void SetKey(Key key, bool down)
        {
            if (down)
                keysDown[key] = true;
            else
                keysUp[key] = true;

            keys[key] = down;
        }

        // 
        void Update()
        {
            // Keyups and keydowns only persist for one frame.
            keysDown.Clear();
            keysUp.Clear();
        }
    private:
        EnumSet<Key> keys;
        EnumSet<Key> keysDown;
        EnumSet<Key> keysUp;

        friend class Window;

    } Input;

    // TODO: Mouse/Mice, Gamepad(s)
}