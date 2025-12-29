#ifndef ASSETS_HPP
#define ASSETS_HPP

#include <vector>
#include <cstdint>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Math/Vector2.hpp"
#include "Math/Vector3.hpp"
#include "Math/Matrix4x4.hpp"

struct Vertex {
    Math::Vector3 position {};
    Math::Vector3 color {};
    Math::Vector2 uv {};
};

struct UniformBufferObject {
    alignas(16) glm::mat4 model {};
    alignas(16) glm::mat4 view {};
    alignas(16) glm::mat4 projection {};
                Math::Vector2 resolution {};
                float time = 0.0f;
                float padding_[13];
};

static_assert("UBOs must be 16-byte aligned" &&
              sizeof(UniformBufferObject) % 16 == 0);

#endif // ASSETS_HPP
