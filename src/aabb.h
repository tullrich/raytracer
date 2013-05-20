#ifndef _AABB_H_
#define _AABB_H_ 

#include "common.h"

namespace raytracer {

using namespace glm;

class Primitive; // forward declaration

/**
 * Axially aligned bounding box
 */
struct AABB
{
	AABB() : max(0), min(0) {};
	AABB(const glm::vec3 &max, const glm::vec3 &min) : max(max), min(min) {};
	vec3 max;
	vec3 min;
};

/**
 * Grow aabb to contain vertex
 * @param aabb   Axially-Aligned bounding box to grow
 * @param vertex point to encompass
 */
void AABBContainVertex(AABB &aabb, const glm::vec3 &vertex);

/**
 * Grow aabb to encompass all vertices in buffer
 * @param aabb        the Axially Aligned BB to grow
 * @param numVertices number of vertices in buffer
 * @param buffer      vertex buffer
 */
void AABBContainVertices(AABB &aabb, int  numVertices, const vec3 *buffer);

/**
 * Grow aabb to contain tri
 * @param aabb   Axially-Aligned bounding box to grow
 * @param vertex triangle to encompass
 */
void AABBContainTriangle(AABB &aabb, const Triangle &tri);

/**
 * Grow aabb to contain another AABB aabb2
 * @param aabb  Axially-Aligned bounding box to grow
 * @param aabb2 Axially-Aligned bounding box to encompass
 */
void AABBContainAABB(AABB &aabb, const AABB &aabb2);

/**
 * Grow aabb to contain any primitive
 * @param aabb  Axially-Aligned bounding box to grow
 * @param aabb2 primitive to encompass
 */
void AABBContainPrimitive(AABB &aabb, const Primitive &primitive);

/**
 * Compute the halfwidth necessary to contain this aabb
 * @param  aabb AABB to compute for
 * @return      halfwidth, always >= 0
 */
float AABBMaxHalfWidth(const AABB &aabb);

/**
 * Test if aabb intersects the plane given by the plane equation dot(X, n) = d
 * This is done via a separating axis test w/ p.n
 * @param  aabb the AABB
 * @param  p    the plane
 * @return      true if the AABB intersects the plane, false otherwise
 */
bool AABBintersectsPlane(const AABB &aabb, const Plane &p);

/**
 * Test if aabb intersects the Triangle t
 * @param  aabb the AABB
 * @param  t    the Triangle
 * @return      true if the AABB intersects the triangle, false otherwise
 */
bool AABBintersectsTriangle(const AABB &aabb, const Triangle &t);

/**
 * Insert stream operator for AABB
 */
std::ostream& operator<<(std::ostream& o, const AABB& b);

} /* raytracer */

#endif /* _AABB_H_ */
