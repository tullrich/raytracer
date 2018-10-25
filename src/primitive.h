#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_ 

#include <list>
#include "common.h"
#include "aabb.h"
#include "ray.h"

namespace raytracer {

/**
 * Abstract primitive for intersection testing
 */
class Primitive
{
public:
	Primitive() {};
	virtual bool intersects(const Ray &r, glm::vec4 &intersectiont) const = 0;
	virtual bool intersects(const AABB &aabb) const = 0;
	virtual glm::vec3 barycentricToPoint(const glm::vec4 &berycentric) const = 0;
	virtual AABB bounds() const = 0;

	virtual float geometricTerm(const glm::vec3 &direction) const = 0;
	virtual glm::vec3 surfaceNormal() const = 0;
	virtual glm::vec3 BRDF(const glm::vec4 &berycentric) const = 0;
	virtual glm::vec3 Le(const glm::vec4 &berycentric) const = 0;
	virtual void updateAABB(AABB &aabb) const = 0;
};

/**
 * Triangle primitive for intersection testing
 */
class TrianglePrimitive : public Primitive
{
public:
	TrianglePrimitive() : material(NULL) {};
	TrianglePrimitive(const Triangle &face, const Material *material) : face(face), material(material) {};
	virtual bool intersects(const Ray &r, glm::vec4 &intersection) const;
	bool intersects2( const Ray &r, glm::vec4 &intersection ) const;
	virtual bool intersects(const AABB &aabb) const;
	virtual glm::vec3 barycentricToPoint(const glm::vec4 &berycentric) const;
	virtual AABB bounds() const;

	virtual float geometricTerm(const glm::vec3 &direction) const;
	virtual glm::vec3 surfaceNormal() const;
	virtual glm::vec3 BRDF(const glm::vec4 &berycentric) const;
	virtual glm::vec3 Le(const glm::vec4 &berycentric) const;
	virtual void updateAABB(AABB &aabb) const;
protected:
	Triangle face;
	const Material *material;
};

/**
 * Triangle primitive for intersection testing
 */
class UVTrianglePrimitive : public TrianglePrimitive
{
public:
	UVTrianglePrimitive() {};
	UVTrianglePrimitive(const Triangle &face, const UVTriangle &uv, const Material *material) : TrianglePrimitive(face, material), uv(uv) {};
	virtual glm::vec3 BRDF(const glm::vec4 &berycentric) const;
	virtual glm::vec3 Le(const glm::vec4 &berycentric) const;
private:
	UVTriangle uv;
};

} /* raytracer */

#endif /* _PRIMITIVE_H_ */
