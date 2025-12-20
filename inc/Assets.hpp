#ifndef ASSETS_HPP
#define ASSETS_HPP

#include <vector>
#include <cstdint>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Math/Vector2.hpp"
#include "Math/Vector3.hpp"
#include "Math/Matrix4x4.hpp"

struct Vertex {
    Math::Vector2 position {};
    Math::Vector3 color {};
};

struct UniformBufferObject {
/*
    Math::Matrix4x4 model {};
    Math::Matrix4x4 view {};
    Math::Matrix4x4 projection {};
*/
    alignas(16) glm::mat4 model {};
    alignas(16) Math::Matrix4x4 view {};
    alignas(16) glm::mat4 proj {};
    alignas(16) float time {};
};

#endif // ASSETS_HPP
