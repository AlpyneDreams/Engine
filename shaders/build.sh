for f in vs_*.glsl; do
    echo $f
    bgfx-shaderc -f $f -o ../runtime/core/shaders/spirv/${f%.*}.bin --type v --platform linux -p spirv
done

for f in fs_*.glsl; do
    echo $f
    bgfx-shaderc -f $f -o ../runtime/core/shaders/spirv/${f%.*}.bin --type f --platform linux -p spirv
done

