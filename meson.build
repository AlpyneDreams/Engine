project('engine', 'cpp', default_options: ['cpp_std=c++20'])

add_project_arguments('-std=c++20', language: 'cpp')

fmt_dep    = dependency('fmt')
sdl_dep    = dependency('sdl2')
bgfx_dep   = dependency('bgfx', method: 'cmake')
vulkan_dep = dependency('vulkan')
vookoo_dep = declare_dependency(include_directories: 'submodules/vookoo/include')
hlslpp_dep = declare_dependency(include_directories: 'submodules/hlslpp/include')
imgui_dep  = dependency('imgui')

entt_dep   = dependency('entt')

subdir('src')
