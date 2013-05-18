#ifndef _AABB_H_
#define _AABB_H_ 

#include "common.h"

namespace raytracer {

using namespace glm;

class Primitive; // forward declaration

/**
 * Axially aligned bounding box
 */
typedef struct 
{
	vec3 max;
	vec3 min;
} AABB;

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
void AABBcontainPrimitive(AABB &aabb, const Primitive &primitive);

/**
 * Insert stream operator for AABB
 */
std::ostream& operator<<(std::ostream& o, const AABB& b);

} /* raytracer */

#endif /* _AABB_H_ */
