#include "primitive.h"

namespace raytracer {

bool TrianglePrimitive::intersects(const Ray &r,  TraceResult &result) const
{
	glm::vec4 intersection(0);

	if (r.intersects(face, intersection))
	{
		result = TraceResult(intersection, face, material);
		return true;
	}

	return false;
}

AABB TrianglePrimitive::bounds() const
{
	AABB contains;
	AABBContainTriangle(contains, triangle);

	return contains;
}

} /* namespace raytracer */
