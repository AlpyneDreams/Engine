
src = files(
    'main/Main.cpp',
    #'platform/glfw/WindowGLFW.cpp',
    #'platform/sdl/WindowSDL.cpp',
    'platform/raylib/Window.cpp',
    #'render/bgfx/RenderBGFX.cpp',
    #'render/vk/RenderVK.cpp',
    'render/raylib/Render.cpp',
    'render/pipelines/forward/Forward.cpp'

)

engine = executable('engine', src,
    dependencies    : [sdl_dep, bgfx_dep, vulkan_dep, vookoo_dep, hlslpp_dep, raylib_dep],
    install         : true,
    install_dir     : join_paths(meson.project_source_root(), 'runtime')
)
