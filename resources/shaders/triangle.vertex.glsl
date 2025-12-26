#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
    float time;
} ubo;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inUv;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragUv;

void main() {
    float factor = abs(sin(ubo.time));
    gl_Position = ubo.projection * ubo.view * ubo.model * vec4(inPosition, 0.0, 1.0);
    gl_Position.y += sin(ubo.time) * 2.0;
    gl_Position.x += cos(ubo.time) * 2.0;
    fragColor = inColor * factor;
    fragColor = inColor;
    fragUv = inUv;
}
