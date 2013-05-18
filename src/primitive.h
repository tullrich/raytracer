#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_ 

#include "common.h"
#include "ray.h"

namespace raytracer {

/**
 * Abstract primitive for intersection testing
 */
class Primitive
{
	virtual bool intersects(const Ray &r,  TraceResult &result) = 0;
};

/**
 * Triangle primitive for intersection testing
 */
class TrianglePrimitive : public Primitive
{
public:

	virtual bool intersects(const Ray &r,  TraceResult &result);

	Triangle face;
	Entity::entity_ptr entity;
	mesh_data::mesh_ptr mesh;
};


} /* raytracer */

#endif /* _PRIMITIVE_H_ */
