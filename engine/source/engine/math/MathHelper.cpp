#include "engine-precompiled-header.h"
#include "MathHelper.h"

/*
Return the lookat angle in radian
*/
float gswy::LookAt(const vec2& delta)
{
	return (delta.y) > 0 ? -atanf(delta.x / (delta.y + 1e-4f)) : PI - atanf(delta.x / (delta.y + 1e-4f));
}
