$input v_normal

#include "bgfx_shader.sh"

void main()
{
    vec3 light = normalize(vec3(0.5, 0.5, -0.5));
    gl_FragColor = vec4(vec3(dot(light, v_normal.xyz)), 1);
}
