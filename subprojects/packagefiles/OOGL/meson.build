project('OOGL', 'cpp', 'c')

deps_sources = run_command('deps.sh', check: true).stdout().strip().split('\n')

sources = files(
    'src/GL/GL/Context.cpp',
    #'src/GL/GL/Context_Win32.cpp',
    'src/GL/GL/Context_X11.cpp',
    'src/GL/GL/Extensions.cpp',
    'src/GL/GL/Framebuffer.cpp',
    'src/GL/GL/Program.cpp',
    'src/GL/GL/Renderbuffer.cpp',
    'src/GL/GL/Shader.cpp',
    'src/GL/GL/Texture.cpp',
    'src/GL/GL/VertexArray.cpp',
    'src/GL/GL/VertexBuffer.cpp',

    'src/GL/Math/Mat3.cpp',
    'src/GL/Math/Mat4.cpp',
    'src/GL/Math/Vec2.cpp',
    'src/GL/Math/Vec3.cpp',
    'src/GL/Math/Vec4.cpp',

    'src/GL/Util/Image.cpp',
    'src/GL/Util/Mesh.cpp',

    'src/GL/Window/Window.cpp',
    #'src/GL/Window/Window_Win32.cpp',
    'src/GL/Window/Window_X11.cpp',
)

oogl = static_library('OOGL', [sources, deps_sources],
    include_directories: include_directories('include', 'include/GL', 'src'),
    cpp_args: ['-O3', '-Wall', '-Wextra', '-Werror', '-std=c++0x'],
    c_args: '-O3'
)

oogl_dep = declare_dependency(
    include_directories: include_directories('include'),
    link_with: [oogl]
)
