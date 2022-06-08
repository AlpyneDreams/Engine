fs = import('fs')
python = import('python').find_installation('python3')

engine_src = [
    'main/Main.cpp',
    'platform/sdl/WindowSDL.cpp',
    'render/bgfx/RenderBGFX.cpp',
    #'render/vk/RenderVK.cpp',
    'render/pipelines/Forward.cpp',

    'imgui/Common.cpp',
    'imgui/impl/imgui_impl_sdl.cpp',
    'imgui/impl/imgui_impl_bgfx.cpp',
]

editor_src = [
    'editor/Editor.cpp'
]

# Build meta files
engine_meta = []
editor_meta = []
foreach file : engine_src + editor_src
    target = custom_target(
        fs.name(file) + '.meta.cpp',
        output: fs.name(file) + '.meta.cpp',
        input: ['../tools/rtti.py', file],
        command: [python, '@INPUT@', '@OUTPUT@']
    )
    editor_meta += [target]
    if engine_src.contains(file)
        engine_meta += [target]
    endif
endforeach

engine_deps = [
    fmt_dep,
    sdl_dep,
    bgfx_dep,
    #vulkan_dep,
    #vookoo_dep,
    glm_dep,
    imgui_dep,
    entt_dep
]

# TODO: Shared static libs between engine and editor

engine = executable('engine', files(engine_src), engine_meta,
    dependencies    : engine_deps,
    install         : true,
    install_dir     : join_paths(meson.project_source_root(), 'runtime'),
    win_subsystem   : 'console',
)

editor = executable('editor', files(engine_src), files(editor_src), editor_meta,
    dependencies    : engine_deps,
    install         : true,
    install_dir     : join_paths(meson.project_source_root(), 'runtime'),
    win_subsystem   : 'console',
    cpp_args        : ['-DEDITOR=1']
)
