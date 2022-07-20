# ENGINE

<img width="961" alt="editor_4LhbOctQWX" src="https://user-images.githubusercontent.com/3376691/179876459-36eae6dc-5d7c-43b9-a834-0d9fc675294b.png">

This is engineering. Most of this code was written under the influence, so do not read too much into it. This codebase is not intended to have an orthodox structure or coherency. There are many branches with significant changes.

Observations:
- Vulkan is an incredibly obtuse API.
- BGFX is \~*quirky*\~

## Dependencies ##

- [Meson](https://mesonbuild.com/) is required to build.
  - [Python 3](https://www.python.org/) may also be required if Meson cannot find it for you.

Handled by Meson (via pkgconfig or WrapDB):
- [fmt](https://fmt.dev/)
- [SDL](https://www.libsdl.org/)
- [GLM](https://github.com/g-truc/glm)
- [Dear ImGui](https://github.com/ocornut/imgui)
- [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo) (for now)
- [EnTT](https://github.com/skypjack/entt) (for now)
- ~~[Vulkan](https://www.vulkan.org/)~~ (currently not used)

Included as Git submodules:
- [bgfx](https://github.com/bkaradzic/bgfx) (with [bx](https://github.com/bkaradzic/bx) and [bimg](https://github.com/bkaradzic/bimg))
- ~~[Vookoo](https://github.com/andy-thomason/Vookoo)~~ (currently not used)

Already included in repo:
- [tinyobjloader](https://github.com/tinyobjloader/tinyobjloader)
- [imgui_impl_bgfx.cpp](https://gist.github.com/RichardGale/6e2b74bc42b3005e08397236e4be0fd0)
- [Material Design Icons Font](https://materialdesignicons.com/)
