project('engine', 'cpp', default_options: ['cpp_std=c++20'])

add_project_arguments('-std=c++20', language: 'cpp')

#glfw_dep  = dependency('glfw3')
sdl_dep    = dependency('sdl2')
bgfx_dep   = dependency('bgfx', method: 'cmake')
hlslpp_dep = declare_dependency(include_directories: 'submodules/hlslpp/include')

subdir('src')
