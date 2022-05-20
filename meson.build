project('engine', 'cpp', default_options: ['cpp_std=c++20'])

windows    = build_machine.system() == 'windows'

fmt_dep    = dependency('fmt')
sdl_dep    = dependency('sdl2', default_options: ['use_sdl2main=false'])
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

if windows
    bgfx_dep = declare_dependency(
        sources: files(
            'submodules/bgfx/src/amalgamated.cpp',
            'submodules/bx/src/amalgamated.cpp',
            'submodules/bimg/src/image.cpp',
            'submodules/bimg/src/image_gnf.cpp',
            'submodules/bimg/3rdparty/astc-codec/src/decoder/astc_file.cc',
            'submodules/bimg/3rdparty/astc-codec/src/decoder/codec.cc',
            'submodules/bimg/3rdparty/astc-codec/src/decoder/endpoint_codec.cc',
            'submodules/bimg/3rdparty/astc-codec/src/decoder/footprint.cc',
            'submodules/bimg/3rdparty/astc-codec/src/decoder/integer_sequence_codec.cc',
            'submodules/bimg/3rdparty/astc-codec/src/decoder/intermediate_astc_block.cc',
            'submodules/bimg/3rdparty/astc-codec/src/decoder/logical_astc_block.cc',
            'submodules/bimg/3rdparty/astc-codec/src/decoder/partition.cc',
            'submodules/bimg/3rdparty/astc-codec/src/decoder/physical_astc_block.cc',
            'submodules/bimg/3rdparty/astc-codec/src/decoder/quantization.cc',
            'submodules/bimg/3rdparty/astc-codec/src/decoder/weight_infill.cc',
        ),
        include_directories: include_directories(
            'submodules/bgfx/include',
            'submodules/bgfx/3rdparty',
            'submodules/bgfx/3rdparty/khronos',
            'submodules/bx/include',
            'submodules/bx/include/compat/msvc',
            'submodules/bx/3rdparty',
            'submodules/bimg/include',
            'submodules/bimg/3rdparty/astc-codec',
            'submodules/bimg/3rdparty/astc-codec/include',
        ),
        compile_args: [
            '-DBX_CONFIG_DEBUG=0',
            '-DBGFX_CONFIG_RENDERER_OPENGL=0',
            '-DBGFX_CONFIG_RENDERER_VULKAN=1',
            '-DPRIxPTR="llx"', # This should not be needed
            '/Zc:__cplusplus',
        ]
    )
else
    bgfx_dep   = dependency('bgfx', method: 'cmake')
endif


subdir('src')
