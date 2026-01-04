#ifndef ASSETS_HPP
#define ASSETS_HPP

#include <vector>
#include <cstdint>
#include <vector>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include "Math/Vector2.hpp"
#include "Math/Vector3.hpp"
#include "Math/Matrix4x4.hpp"

struct Vertex {
    glm::vec3 position {};
    glm::vec3 color {};
    glm::vec2 uv {};

    bool operator == (Vertex const& other) const;
};

template <>
struct std::hash <Vertex> {
    size_t operator () (Vertex const& vertex) const {
        return ((hash<glm::vec3>()(vertex.position) ^ 
                (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ 
                (hash<glm::vec2>()(vertex.uv) << 1);
    }
};

struct UniformBufferObject {
    alignas(16) glm::mat4 model {};
    alignas(16) glm::mat4 view {};
    alignas(16) glm::mat4 projection {};
                glm::vec3 cameraPosition {};
                float time = 0.0f;
                Math::Vector2 resolution {};
                float padding_[10];
};

static_assert("UBOs must be 16-byte aligned" &&
              sizeof(UniformBufferObject) % 16 == 0);

#endif // ASSETS_HPP
