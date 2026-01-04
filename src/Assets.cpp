#include "Assets.hpp"

bool Vertex::operator == (Vertex const& other) const {
    return position == other.position && color == other.color && uv == other.uv;
}
