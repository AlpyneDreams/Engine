bgfx-shaderc -f vs_cubes.glsl -o ../runtime/core/shaders/spirv/vs_cubes.bin --type v --platform linux -p spirv
bgfx-shaderc -f fs_cubes.glsl -o ../runtime/core/shaders/spirv/fs_cubes.bin --type f --platform linux -p spirv
bgfx-shaderc -f vs_cubes.glsl -o ../runtime/core/shaders/glsl/vs_cubes.bin --type v --platform linux
bgfx-shaderc -f fs_cubes.glsl -o ../runtime/core/shaders/glsl/fs_cubes.bin --type f --platform linux
