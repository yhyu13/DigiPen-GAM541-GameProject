#include "engine-precompiled-header.h"
#include "MathHelper.h"

/*
Return the lookat angle in radian
*/
float gswy::LookAt(const vec2& delta)
{
	return (delta.y) > 0 ? -atanf(delta.x / (delta.y + 1e-4f)) : PI - atanf(delta.x / (delta.y + 1e-4f));
}

vec2 gswy::ToVec(float rotation)
{
	return vec2(-sinf(rotation),cosf(rotation));
}

std::ostream& operator<<(std::ostream& o, const glm::vec3& n) {
	o << '(' << n.x << ',' << n.y << ',' << n.z << ") ";
	return o;
}

std::ostream& operator<<(std::ostream& o, const glm::vec2& n) {
	o << '(' << n.x << ',' << n.y << ") ";
	return o;
}