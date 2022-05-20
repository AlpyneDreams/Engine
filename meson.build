project('engine', 'cpp', default_options: ['cpp_std=c++20'])

add_project_arguments('-std=c++20', language: 'cpp')
windows    = build_machine.system() == 'windows'

fmt_dep    = dependency('fmt')
sdl_dep    = dependency('sdl2', default_options: ['use_sdl2main=false'])
bgfx_dep   = dependency('bgfx', method: 'cmake')
vulkan_dep = dependency('vulkan')
vookoo_dep = declare_dependency(include_directories: 'submodules/vookoo/include')
hlslpp_dep = declare_dependency(include_directories: 'submodules/hlslpp/include')
imgui_dep  = dependency('imgui')

entt_dep   = dependency('entt', required: not windows)

# On Windows, we have to use CMake. required: false stops meson
# from trying to find a meson.build file in the wrapped project.
if not entt_dep.found() and windows
    cmake      = import('cmake')
    entt       = cmake.subproject('entt')
    entt_dep   = entt.dependency('EnTT')
endif

subdir('src')
