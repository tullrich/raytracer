#include "aabb.h"
#include "primitive.h"

namespace raytracer {

static inline float rtfmin(float a, float b)
{
	return (a < b) ? a : b;
}

static inline float rtfmax(float a, float b)
{
	return (a > b) ? a : b;
}

void AABBContainVertex(AABB &aabb, const glm::vec3 &vertex)
{
	if (aabb.max.x > vertex.x) aabb.max.x = vertex.x;
	if (aabb.min.x < vertex.x) aabb.min.x = vertex.x;

	if (aabb.max.y > vertex.y) aabb.max.y = vertex.y;
	if (aabb.min.y < vertex.y) aabb.min.y = vertex.y;

	if (aabb.max.z > vertex.z) aabb.max.z = vertex.z;
}



void AABBContainVertices(AABB &aabb, int  numVertices, const vec3 *buffer)
{
	for (int i = 0; i < numVertices; ++i)
	{
		AABBContainVertex(aabb, buffer[i]);
	}
}

void AABBContainTriangle(AABB &aabb, const Triangle &tri)
{
	AABBContainVertex(aabb, tri.A);
	AABBContainVertex(aabb, tri.B);
	AABBContainVertex(aabb, tri.C);
}

void AABBContainAABB(AABB &aabb, const AABB &aabb2)
{
	AABBContainVertex(aabb, aabb2.max);
	AABBContainVertex(aabb, aabb2.min);
}

void AABBContainPrimitive(AABB &aabb, const Primitive &primitive)
{
	AABB bounds = primitive.bounds();
	AABBContainAABB(aabb, bounds);
}

float AABBMaxHalfWidth(const AABB &aabb)
{
	float dx = abs(aabb.max.x - aabb.min.x);
	float dy = abs(aabb.max.y - aabb.min.y);
	float dz = abs(aabb.max.z - aabb.min.z);

	return fmax(dx, fmax(dy, dz));
}

bool AABBintersectsPlane(const AABB &aabb, const Plane &p)
{
	glm::vec3 c = (aabb.max - aabb.min) * 0.5f; // aabb center
	glm::vec3 e = aabb.max - c; // aabb extents

	// max radius on separating axis, n
	float r = e.x * abs(p.n.x) + e.y * abs(p.n.y) + e.z + abs(p.n.z); 
	// distance along separating axis between aabb center and the plane
	float s = abs(dot(c, p.n) - p.d);

	// if s <= r, the box must be overlapping the plane
	return s <= r;
}


std::ostream& operator<<(std::ostream& o, const AABB& b)
{
	return o << "AABB {\n"
		<< "\tmax: " << b.max << "\n" \
		<< "\tmin: " << b.min << "\n" \
		<< "}";
}

} /* raytracer */
