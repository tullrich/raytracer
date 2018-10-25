#include "ray.h"
#include <cfloat>

namespace raytracer {


Ray::Ray(glm::vec3 p, glm::vec3 q)
{
	this->p = p;
	this->q = q;

	this->n = glm::normalize(q - p);
	this->d = glm::length(q - p);
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
	if(t < 0.0f) 
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


bool Ray::intersects(const AABB &a, glm::vec3 &result) const
{
	float tmin = 0.0f;
	float tmax = FLT_MAX; // max float

	for(int i = 0; i < 3; i++)
	{
		if(abs(n[i]) < FLOAT_ERROR) // w/ an allowable error
		{
			if(p[i] < a.min[i] || p[i] > a.max[i]) // within the plane
			{
				return false;
			}
		}
		else
		{
			float ood = 1.0f / n[i];
			float t1 = (a.min[i] - p[i]) * ood; // intersection with max
			float t2 = (a.max[i] - p[i]) * ood; // intersection with min

			if (t1 > t2) // ensure t1 is the "close" plane, the plane of the slab first intersected by the ray
			{
				float temp = t1;
				t2 = t1;
				t1 = temp;
			}


			if (t1 > tmin)
				tmin = t1;	// furtherst intersection from a "close" plane

			if (t2 < tmax)
				tmax = t2; // furtherst intersection from a "far" plane

			if (tmin > tmax)
				return false;
		}
	}

	result = p + tmin * n;
	return true;
}

std::ostream& operator<<(std::ostream& o, const Ray& b)
{
	return o << "Ray {\n" \
		<< "\tp: " << b.p << "\n" \
		<< "\tq:" << b.q << "\n" \
		<< "\tn:" << b.n << "\n" \
		<< "}";
}


} /* namespace raytracer */
