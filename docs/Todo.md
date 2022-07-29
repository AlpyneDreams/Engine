- Scene loading - (de)serialization

- Move contents of entity/ to scene/

- Global null object?
- Common.h: ulong? usize? size? Size?

- Entity systems
  - Traditional: Hierarchy of inheritance-based entities.
  - ECS: Entities are containers for data-based components.
  - Unity: Hierarchy of GameObjects, each with Transform + other components.
  - Unreal: Actors consist of a root component and child components
  - Godot: Hierarchy of inheritance-based nodes.
  - Idea: Hierarchy of single-purpose components
    - Could call transform component Entity
    - If only Entity can hold components then this is more of a presentation change.

- Inspector GUI::ItemLabel is not connected to its item
  - Maybe it's possible to make ImGui do left-aligned labels by default?

- RTTI generation could be improved, or at least streamline rtti.py
  - Probably generate one CPP or header for whole project.
    - There is currently some redundancy and it doesn't always update right
  - Enum names !!!!
  - Base classes
    - Then add GetComponents<Base>()
  - Fix RTTI<X> has already been instantiated bug
  - Allow generating VertexLayout from structs
    - Could define attributes with an Attribute<T, ...> class or some other way
      of adding attributes/metadata to fields. Or just go by name.
  - Could generate VTable<T> with this method.

- Editor should eventually link engine as a static lib to reduce compile times

- Change how we handle singletons?
  - Sharing name between type and instance is not ideal
  - Centralized global variable list?
  - inline struct {...} Name; ?
    - This can create ambiguity sometimes !!
      - Can typically be resolved by specifying namespace such as in the lambda in Editor::Run
  - old fashioned singleton?
    - Some kind of template bullshit?
  - static classes?
  - namespaces??

- Should we not indent for `namespace engine {`?
    - Maybe use 2-space indent?

Renderer
- Threaded renderer
- Inheritance and downcasting feels icky
  - Template polymorphism or just per-build implementations
- Surface interface as base for Window and RenderTarget
- Choose default camera scale FOV base aspect ratio
  - Currently using 16:9. Source uses 4:3. Does Unity use something?
 
- Further troubleshoot mouse input lag for imgui (current solution: set bgfx maxFrameLatency = 1)
    - See issues: ocornut/imgui#1805, ocornut/imgui#2938, bkaradzic/bgfx#2015

SDL Business:
- Should WindowSDL PollEvents in PreUpdate or in Update?
- Is SDL_RenderPresent or SDL_UpdateWindowSurface or anything ever needed?
- Is SDL_WINDOW_VULKAN necessary

- BGFX debug text and debug stats mode

Math
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
- Unity new input system features
  - Action maps (e.g. could be for Player, UI, Vehicle, whatever...)
  - Schemes (e.g. Gamepad, Keyboard+Mouse, Xbox, PS)
  - Generic gamepad
  - Interactions (e.g. Tap, Hold)
  - PlayerInput component sends messages to the gameobject or its children
