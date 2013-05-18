#include "primitive.h"

namespace raytracer {
	
bool TrianglePrimitive::intersects(const Ray &r,  TraceResult &result)
{
	glm::vec3 intersection(0);

	if (r.intersects(triangle, intersection))
	{
		result = TraceResult(intersection, triangle, entity, mesh, material);
		return true;
	}

	return false;
}

} /* namespace raytracer */
