- RTTI generation could be improved, or at least streamline rtti.py
  - Enum names !!!!

- Editor should eventually link engine as a static lib to reduce compile times
  - So don't rely on ifdef too much!
- For defines (PLATFORM_*, EDITOR), should they be either 1/0, or #ifdef-based

- Replace entt::registry::view<...>().each() with better semantics

- Change how we handle singletons?
  - Sharing name between type and instance is not ideal
  - Centralized global variable list?
  - inline struct {...} Name; ?
  - old fashioned singleton?
    - Some kind of template bullshit?
  - static classes?
  - namespaces??

- Should we not indent for `namespace engine {`?
    - Maybe use 2-space indent?

- Threaded renderer
- Further troubleshoot mouse input lag for imgui (current solution: set bgfx maxFrameLatency = 1)
    - See issues: ocornut/imgui#1805, ocornut/imgui#2938, bkaradzic/bgfx#2015

SDL Business:
- Should WindowSDL PollEvents in PreUpdate or in Update?
- Is SDL_RenderPresent or SDL_UpdateWindowSurface or anything ever needed?
- Is SDL_WINDOW_VULKAN necessary

- BGFX debug text and debug stats mode

Math
- floatN aliases for vecN?
- left or right handed?
- Y or Z up?
- NDC Z is from -1 or 0?

Console
- keybinds in general
- aliases
- flags
- autocomplete

Input
- Analog inputs
- Input actions
- Interop with console commands / bind command
- Pretty names ("Left Mouse Button" / "LMB") and icons 
- Steam Input ?
- Hashed strings? (UDL literals) For actions or for literal keys?
- Keyboard, Mouse, Gamepad, multi-devices
- Get all Key Codes, Gamepad Buttons, etc.
