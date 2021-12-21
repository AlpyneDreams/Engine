
src = files(
    'main/Main.cpp',
    'render/pipelines/forward/Forward.cpp'
)

if windowlib == 'glfw'
    src += files('platform/glfw/WindowGLFW.cpp')
elif windowlib == 'sdl'
    src += files('platform/sdl/WindowSDL.cpp')
endif

if graphicslib == 'bgfx'
    src += files('render/bgfx/RenderBGFX.cpp')
elif graphicslib == 'vulkan'
    src += files('render/vku/RenderVKU.cpp')
endif

engine = executable('engine', src,
    dependencies    : deps,
    install         : true,
    install_dir     : join_paths(meson.project_source_root(), 'runtime')
)
