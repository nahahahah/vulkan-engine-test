#version 460

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
    vec3 cameraPosition;
    float time;
    vec2 resolution;
} ubo;

layout(binding = 1) uniform sampler2D textureSampler;
layout(binding = 2) uniform sampler2D normalTextureSampler;

layout(location = 0) in vec3 inColor;
layout(location = 1) in vec2 inUv;
layout(location = 2) in vec3 inViewDirection;

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

    vec3 normalEncoded = texture(normalTextureSampler, inUv).rgb;
    vec3 N = normalize(normalEncoded);
    vec3 V = normalize(inViewDirection);

    vec3 baseColor = texture(textureSampler, inUv).rgb;
    float diffuseFactor = 1.0;
    float specularFactor = 0.5;
    float roughness = 0.5;

    vec3 resultingColor = vec3(0.0);

    vec3 lightColor = vec3(1.0);
    vec3 lightDirection = vec3(0.5, -0.3, 0.8);
    vec3 L = normalize(lightDirection);
    vec3 R = reflect(-L, N);    
    
    vec3 diffuse = max(0.0, dot(L, N)) * lightColor;

    float RoV = max(0.0, dot(R, V));
    vec3 specular = vec3(
        pow(RoV, roughness),
        pow(RoV, roughness),
        pow(RoV, roughness)
    );
    resultingColor = baseColor * diffuseFactor * diffuse + specularFactor * specular;

    float correctionFactor = 2.2;
    vec3 correctedColor = vec3(
        pow(resultingColor.r, correctionFactor),
        pow(resultingColor.g, correctionFactor),
        pow(resultingColor.b, correctionFactor)
    );

    //outColor = vec4(inUv, 1.0, 1.0); // display uv as colors
    //outColor = texture(normalTextureSampler, inUv); // display normal texture
    //outColor = vec4(inColor, 1.0) * texture(textureSampler, inUv); // display texture
    outColor = vec4(correctedColor, 1.0); // display normal texture
}