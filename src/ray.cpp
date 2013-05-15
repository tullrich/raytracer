#include "ray.h"

namespace raytracer {

Ray::Ray(glm::vec3 p, glm::vec3 q)
{
	this->p = p;
	this->q = q;
}


bool Ray::intersects(const Triangle &tri, glm::vec4 &intersection) const
{
	vec3 ab = tri.B - tri.A;
	vec3 ac = tri.C - tri.A;
	vec3 qp = p - q; // points backward along the ray

	// triangle normal
	vec3 n = cross(ab, ac);

	float d = dot(qp, n);
	// if deno <= 0, ray is parallel or point away from the triangle
	if(d <= 0.0f)
	{	
		return false;
	}

	vec3 ap = p - tri.A;
	float t = dot(ap, n);
	if(t <= 0.0f) 
	{	
		return false;
	}

	vec3 e = cross(qp, ap);
	float v = dot(ac, e);
	if (v <0.0f || v > d)
	{	
		return false;
	}
	float w = -dot(ab, e);
	if (w < 0.0f || v + w > d)
	{	
		return false;
	}

	float ood = 1.0f / d;

	intersection.w = t;
	intersection.y = v * ood;
	intersection.z = w * ood;
	intersection.x = 1.0f - intersection.y - intersection.z;

	return true;
}

} /* namespace raytracer */
