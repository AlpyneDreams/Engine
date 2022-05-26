fs = import('fs')
python = import('python').find_installation('python3')

src = [
    'main/Main.cpp',
    'platform/sdl/WindowSDL.cpp',
    'render/bgfx/RenderBGFX.cpp',
    #'render/vk/RenderVK.cpp',
    'render/pipelines/forward/Forward.cpp',

    'imgui/Common.cpp',
    'imgui/impl/imgui_impl_sdl.cpp',
    'imgui/impl/imgui_impl_bgfx.cpp',
]

# Build meta files
meta_files = []
foreach file : src
    meta_files += custom_target(
        fs.name(file) + '.meta.cpp',
        output: fs.name(file) + '.meta.cpp',
        input: ['../tools/rtti.py', file],
        command: [python, '@INPUT@', '@OUTPUT@']
    )
endforeach
meta_dep = declare_dependency(sources: meta_files)


engine = executable('engine', files(src),
    dependencies    : [meta_dep, fmt_dep, sdl_dep, bgfx_dep, vulkan_dep, vookoo_dep, hlslpp_dep, imgui_dep, entt_dep],
    install         : true,
    install_dir     : join_paths(meson.project_source_root(), 'runtime'),
    win_subsystem   : 'console',
)
