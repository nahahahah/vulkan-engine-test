#ifndef ASSETS_HPP
#define ASSETS_HPP

#include "Math/Vector2.hpp"
#include "Math/Vector3.hpp"
#include "Math/Matrix4x4.hpp"

struct Vertex {
    Math::Vector2 position {};
    Math::Vector3 color {};
};

struct UniformBufferObject {
    Math::Matrix4x4 model {};
    Math::Matrix4x4 view {};
    Math::Matrix4x4 projection {};
};

#endif // ASSETS_HPP
