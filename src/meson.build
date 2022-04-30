
src = files(
    'main/Main.cpp',
    'platform/sdl/WindowSDL.cpp',
    'render/bgfx/RenderBGFX.cpp',
    #'render/vk/RenderVK.cpp',
    'render/pipelines/forward/Forward.cpp',

    'imgui/Common.cpp',
    'imgui/impl/imgui_impl_sdl.cpp',
    'imgui/impl/imgui_impl_bgfx.cpp',
)

engine = executable('engine', src,
    dependencies    : [fmt_dep, sdl_dep, bgfx_dep, vulkan_dep, vookoo_dep, hlslpp_dep, imgui_dep, entt_dep],
    install         : true,
    install_dir     : join_paths(meson.project_source_root(), 'runtime')
)
