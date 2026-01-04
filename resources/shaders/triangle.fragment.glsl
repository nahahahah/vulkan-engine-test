#version 460

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
    vec2 resolution;
    float time;
} ubo;

layout(binding = 1) uniform sampler2D textureSampler;

layout(location = 0) in vec3 inColor;
layout(location = 1) in vec2 inUv;

layout(location = 0) out vec4 outColor;

vec2 skew(vec2 fragPos) {
    vec2 skewedPos = vec2(0.0);
    skewedPos.x = 1.1547 * fragPos.x;
    skewedPos.y = fragPos.y + 0.5 * fragPos.x;
    return skewedPos;
}

vec3 simplexGrid(vec2 fragPos) {
    vec3 position = vec3(0.0);

    vec2 point = fract(skew(fragPos));
    if (point.x > point.y) {
        position.xy = 1.0 - vec2(point.x, point.y - point.x);
        position.z = point.y;
    }

    else {
        position.yz = 1.0 - vec2(point.x - point.y, point.y);
        position.x = point.x;
    }

    return fract(position);
}

void main() {
    bool gammaComparison = false;

    vec2 fragPos = gl_FragCoord.xy / ubo.resolution.xy;
    vec3 color = vec3(0.0);
    
    /*
    // Scale the space to see the grid
    fragPos *= 300.0; // * sin(0.75 * ubo.time);

    // Show the 2D grid
    color.rg = fract(fragPos);

    // Skew the 2D grid
    color.rg = fract(skew(fragPos));

    // Subdivide the grid into equilateral triangles
    color = simplexGrid(fragPos);

    vec3 originalColor = color;
    vec3 linearColor = vec3(
        pow(color.r ,2.2),
        pow(color.g, 2.2),
        pow(color.b, 2.2)
    );
    
    color = linearColor;

    if (gammaComparison && ((int(ubo.time) / 3) % 2) == 0) {
        color = originalColor;
    }
    */

    //outColor = vec4(inUv, 1.0, 1.0); // display uv as colors
    outColor = vec4(inColor, 1.0) * texture(textureSampler, inUv); // display texture
}