#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
    vec2 resolution;
    float time;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inUv;

layout(location = 0) out vec3 outColor;
layout(location = 1) out vec2 outUv;

void main() {
    float ratio = ubo.resolution.x / ubo.resolution.y;
    float factor = abs(sin(ubo.time));
    // Apply positional offsets in view space to avoid invalid clip-space z/w relations
    vec4 worldPos = ubo.model * vec4(inPosition, 1.0);
    vec4 viewPos = ubo.view * worldPos;
    //viewPos.y += 10.0 * sin(1.0 * ubo.time + 5.0) * ratio;
    //viewPos.x += 10.0 * cos(2.0 * ubo.time + 2.0);
    //viewPos.z += 10.0 * cos(3.0 * ubo.time - 1.0);
    gl_Position = ubo.projection * viewPos;
    outColor = inColor;
    outUv = inUv;
}
