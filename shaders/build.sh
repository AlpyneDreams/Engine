bgfx-shaderc -f vs_cubes.glsl -o ../runtime/core/shaders/spirv/vs_cubes.bin --type v --platform linux -p spirv
bgfx-shaderc -f fs_cubes.glsl -o ../runtime/core/shaders/spirv/fs_cubes.bin --type f --platform linux -p spirv
bgfx-shaderc -f vs_basic.glsl -o ../runtime/core/shaders/spirv/vs_basic.bin --type v --platform linux -p spirv
bgfx-shaderc -f fs_wireframe.glsl -o ../runtime/core/shaders/spirv/fs_wireframe.bin --type f --platform linux -p spirv
bgfx-shaderc -f fs_debug_ids.glsl -o ../runtime/core/shaders/spirv/fs_debug_ids.bin --type f --platform linux -p spirv
