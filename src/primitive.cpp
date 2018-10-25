#include "primitive.h"
#include "material.h"

namespace raytracer {

bool TrianglePrimitive::intersects(const Ray &r, glm::vec4 &intersection) const
{
	return r.intersects( face, intersection );
}

bool TrianglePrimitive::intersects2( const Ray &r, glm::vec4 &intersection ) const
{
	return r.intersects( face, intersection );
}

glm::vec3 TrianglePrimitive::barycentricToPoint(const glm::vec4 &berycentric) const
{
	return face.intersectionToPoint(berycentric);
}


glm::vec3 TrianglePrimitive::surfaceNormal() const
{
	return face.normal();
}

float TrianglePrimitive::geometricTerm(const glm::vec3 &direction) const
{
	// geometricTerm term = dot(surface normal,  reflecting direction)
	return fmaxf(glm::dot(face.normal(), direction), 0);
}

glm::vec3 TrianglePrimitive::Le(const glm::vec4 &berycentric) const
{
	return material->Le(NULL);
}

glm::vec3 TrianglePrimitive::BRDF(const glm::vec4 &berycentric) const
{
	return material->BRDF(NULL);
}

glm::vec3 UVTrianglePrimitive::Le(const glm::vec4 &berycentric) const
{
	TexCoord uv_at_point = uv.interpolateUV(berycentric);

	return material->Le(&uv_at_point);
}

glm::vec3 UVTrianglePrimitive::BRDF(const glm::vec4 &berycentric) const
{
	TexCoord uv_at_point = uv.interpolateUV(berycentric);

	return material->BRDF(&uv_at_point);
}

bool TrianglePrimitive::intersects(const AABB &aabb) const
{
	return AABBintersectsTriangle(aabb, face);
}

AABB TrianglePrimitive::bounds() const
{
	AABB contains;
	AABBContainTriangle(contains, face);

	return contains;
}

void TrianglePrimitive::updateAABB(AABB &aabb) const
{
	AABBContainTriangle(aabb, face);
}

} /* namespace raytracer */
