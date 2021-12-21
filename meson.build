project('engine', 'cpp', default_options: ['cpp_std=c++20'])

add_project_arguments('-std=c++20', language: 'cpp')

deps = []

windowlib = get_option('windowlib')
if windowlib == 'glfw'
    deps += dependency('glfw3')
elif windowlib == 'sdl'
    deps += dependency('sdl2')
endif

graphicslib = get_option('graphicslib')
#if graphicslib == 'bgfx'
    deps += dependency('bgfx', method: 'cmake')
if graphicslib == 'vulkan'
    deps += dependency('vulkan')
    deps += declare_dependency(include_directories: 'submodules/vookoo/include')
endif

deps += declare_dependency(include_directories: 'submodules/hlslpp/include')

subdir('src')
