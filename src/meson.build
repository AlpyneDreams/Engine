
src = files(
    'main/Main.cpp',
    #'platform/glfw/WindowGLFW.cpp',
    'platform/sdl/WindowSDL.cpp',
    'render/bgfx/RenderBGFX.cpp',
    #'render/vk/RenderVK.cpp',
    'render/pipelines/forward/Forward.cpp',
    'assets/models/importers/OBJ.cpp'
)

engine = executable('engine', src,
    dependencies    : [sdl_dep, bgfx_dep, vulkan_dep, vookoo_dep, hlslpp_dep, tinyobj_dep],
    install         : true,
    install_dir     : join_paths(meson.project_source_root(), 'runtime')
)
