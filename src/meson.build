fs = import('fs')
python = import('python').find_installation('python3')

main = 'main/Main.cpp'

engine_src = [
    'assets/import/MeshOBJ.cpp',
    'platform/sdl/WindowSDL.cpp',
    'platform/sdl/CursorSDL.cpp',
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
        fs.name(file) + '.meta.json',
        output: fs.name(file) + '.meta.json',
        input: ['../tools/meta.py', file],
        command: [python, '@INPUT@', '@OUTPUT@']
    )
    if editor_src.contains(file)
        editor_meta += [target]
    endif
    if engine_src.contains(file)
        engine_meta += [target]
    endif
endforeach

editor_rtti = custom_target(
    'rtti.cpp',
    output: 'rtti.cpp',
    input: ['../tools/rtti.py', engine_meta, editor_meta],
    command: [python, '@INPUT@', '@OUTPUT@']
)

# Deps which can be only built in engine_lib
engine_deps = [
    sdl_dep,
    bgfx_dep,
]

shared_deps = [
    fmt_dep,
    #vulkan_dep,
    #vookoo_dep,
    glm_dep,
    imgui_dep,
    imguizmo_dep,
    entt_dep
]

engine_lib = static_library('engine', files(engine_src),
    dependencies    : engine_deps + shared_deps,
    include_directories: include_directories('../submodules')
)

engine = executable('engine', main,
    dependencies  : shared_deps,
    link_with     : [engine_lib],
    install       : true,
    install_dir   : join_paths(meson.project_source_root(), 'runtime'),
    win_subsystem : 'console',
)

editor = executable('editor', files(editor_src), main, editor_rtti,
    dependencies    : shared_deps,
    link_with       : [engine_lib],
    install         : true,
    install_dir     : join_paths(meson.project_source_root(), 'runtime'),
    win_subsystem   : 'console',
    cpp_args        : ['-DEDITOR=1']
)
