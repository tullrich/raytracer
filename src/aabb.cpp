#include "aabb.h"

namespace raytracer {


void AABBContainVertices(AABB &aabb, int  numVertices, const vec3 *buffer)
{
	for (int i = 0; i < numVertices; ++i)
	{
		if (aabb.max.x > buffer[i].x) aabb.max.x = buffer[i].x;
		if (aabb.min.x < buffer[i].x) aabb.min.x = buffer[i].x;

		if (aabb.max.y > buffer[i].y) aabb.max.y = buffer[i].y;
		if (aabb.min.y < buffer[i].y) aabb.min.y = buffer[i].y;

		if (aabb.max.z > buffer[i].z) aabb.max.z = buffer[i].z;
		if (aabb.min.z < buffer[i].z) aabb.min.z = buffer[i].z;
	}
}

std::ostream& operator<<(std::ostream& o, const AABB& b)
{
	return o << "AABB {\n"
		<< "\tmax: " << b.max << "\n" \
		<< "\tmin: " << b.min << "\n" \
		<< "}";
}

} /* raytracer */
