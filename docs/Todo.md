- Should we not indent for `namespace engine {`?
    - Maybe use 2-space indent?
- Replace HLSLPP with GLM and just make floatN aliases

- Threaded renderer
- Further troubleshoot mouse input lag for imgui (current solution: set bgfx maxFrameLatency = 1)
    - See issues: ocornut/imgui#1805, ocornut/imgui#2938, bkaradzic/bgfx#2015

SDL Business:
- Should WindowSDL PollEvents in PreUpdate or in Update?
- Is SDL_RenderPresent or SDL_UpdateWindowSurface or anything ever needed?
- Is SDL_WINDOW_VULKAN necessary

- BGFX debug text and debug stats mode