project('engine', 'cpp', 'c', default_options: ['cpp_std=c++20'])

add_project_arguments('-std=c++20', language: 'cpp')

#glfw_dep  = dependency('glfw3')
sdl_dep    = dependency('sdl2')
bgfx_dep   = dependency('bgfx', method: 'cmake')
opengl_dep = dependency('gl')
glad_dep   = declare_dependency(include_directories: 'submodules/glad/include', sources: ['submodules/glad/src/glad.c'])
vulkan_dep = dependency('vulkan')
vookoo_dep = declare_dependency(include_directories: 'submodules/vookoo/include')
hlslpp_dep = declare_dependency(include_directories: 'submodules/hlslpp/include')

subdir('src')
