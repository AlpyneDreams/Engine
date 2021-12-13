
src = files(
    'main/Main.cpp',
    #'platform/glfw/WindowGLFW.cpp',
    'platform/sdl/WindowSDL.cpp',
    'render/bgfx/RenderBGFX.cpp'

)

engine = executable('engine', src,
    dependencies    : [sdl_dep, bgfx_dep],
    install         : true,
    install_dir     : join_paths(meson.project_source_root(), 'runtime')
)

executable('test_vector', 'tests/test_vector.cpp')
