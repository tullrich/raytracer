#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_ 

#include <list>
#include "common.h"
#include "ray.h"

namespace raytracer {

/**
 * Abstract primitive for intersection testing
 */
class Primitive
{
public:
	Primitive() {};
	virtual bool intersects(const Ray &r,  TraceResult &result) const = 0;
};

/**
 * Triangle primitive for intersection testing
 */
class TrianglePrimitive : public Primitive
{
public:
	TrianglePrimitive(const Triangle &face, const Material *material) : face(face), material(material) {};
	virtual bool intersects(const Ray &r,  TraceResult &result) const;

protected:
	Triangle face;
	const Material *material;
};

} /* raytracer */

#endif /* _PRIMITIVE_H_ */
