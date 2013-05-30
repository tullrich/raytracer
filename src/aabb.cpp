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
	if (aabb.max.x < vertex.x) aabb.max.x = vertex.x;
	if (aabb.min.x > vertex.x) aabb.min.x = vertex.x;

	if (aabb.max.y < vertex.y) aabb.max.y = vertex.y;
	if (aabb.min.y > vertex.y) aabb.min.y = vertex.y;

	if (aabb.max.z < vertex.z) aabb.max.z = vertex.z;
	if (aabb.min.z > vertex.z) aabb.min.z = vertex.z;
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

	return rtfmax(dx, rtfmax(dy, dz));
}

bool AABBintersectsPlane(const AABB &aabb, const Plane &p)
{
	glm::vec3 c = (aabb.max + aabb.min) * 0.5f; // aabb center
	glm::vec3 e = aabb.max - c; // aabb extents

	// max radius on separating axis, n
	float r = e.x * abs(p.n.x) + e.y * abs(p.n.y) + e.z * abs(p.n.z); 
	// distance along separating axis between aabb center and the plane
	float s = glm::dot(p.n, c) - p.d;

	//std::cout << " s " << s << " <= r " << p.d << "  p.n " << p.n << std::endl; 

	// if s <= r, the box must be overlapping the plane
	return abs(s) <= r; 
}

static inline bool quickAxisIsSeparating(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &f,  const float &r)
{
	float p0, p1, p2;

	p0 = glm::dot(v0, f);
	p1 = glm::dot(v1, f);
	p2 = glm::dot(v2, f);


	//std::cout << "p0 " << p0 << " p1 " << p1 << " p2 " << p2 << " r " << r << std::endl;
	return (rtfmax(-rtfmax(p0, rtfmax(p1, p2)), rtfmin(p0, rtfmin(p1, p2))) > r);
}

bool AABBintersectsTriangle(const AABB &aabb, const Triangle &t)
{

	float r;
	glm::vec3 f;

	glm::vec3 c = (aabb.max + aabb.min) * 0.5f; // aabb center
	glm::vec3 e;// = aabb.max - c; // aabb extents
	e.x = (aabb.max.x - aabb.min.x) * 0.5;
	e.y = (aabb.max.y - aabb.min.y) * 0.5;
	e.z = (aabb.max.z - aabb.min.z) * 0.5;

	// translate triangle verts to origin
	glm::vec3 v0 = t.A - c;
	glm::vec3 v1 = t.B - c;
	glm::vec3 v2 = t.C - c;

	// triangle faces
	glm::vec3 f0 = v1 - v0;
	glm::vec3 f1 = v2 - v1;
	glm::vec3 f2 = v0 - v2;

	// a00
	f = glm::vec3(0, -f0.z, f0.y);
	r = e.y * abs(f0.z) + e.z * abs(f0.y);
	if (quickAxisIsSeparating(v0, v1, v2, f, r)) 
	{
		//std::cout << "outing at a00" << std::endl;
		return false;
	}

	// a01
	f = glm::vec3(0, -f1.z, f1.y);
	r = e.y * abs(f1.z) + e.z * abs(f1.y);
	if (quickAxisIsSeparating(v0, v1, v2, f, r)) 
	{
		//std::cout << "outing at a01" << std::endl;
		return false;
	}

	// a02
	f = glm::vec3(0, -f2.z, f2.y);
	r = e.y * abs(f2.z) + e.z * abs(f2.y);
	if (quickAxisIsSeparating(v0, v1, v2, f, r)) 
	{
		//std::cout << "outing at a02" << std::endl;
		return false;
	}

	// a10
	f = glm::vec3(f0.z, 0, -f0.x);
	r = e.x * abs(f0.z) + e.z * abs(f0.x);
	if (quickAxisIsSeparating(v0, v1, v2, f, r)) 
	{
		//std::cout << "outing at a10" << std::endl;
		return false;
	}

	// a11
	f = glm::vec3(f1.z, 0, -f1.x);
	r = e.x * abs(f1.z) + e.z * abs(f1.x);
	if (quickAxisIsSeparating(v0, v1, v2, f, r)) 
	{
		//std::cout << "outing at a11" << std::endl;
		return false;
	}

	// a12
	f = glm::vec3(f2.z, 0, -f2.x);
	r = e.x * abs(f2.z) + e.z * abs(f2.x);
	if (quickAxisIsSeparating(v0, v1, v2, f, r)) 
	{
		//std::cout << "outing at a12" << std::endl;
		return false;
	}

	// a20
	f = glm::vec3(-f0.y, f0.x, 0);
	r = e.x * abs(f0.y) + e.y * abs(f0.x);
	if (quickAxisIsSeparating(v0, v1, v2, f, r)) 
	{
		//std::cout << "outing at a20" << std::endl;
		return false;
	}

	// a21
	f = glm::vec3(-f1.y, f1.x, 0);
	r = e.x * abs(f1.y) + e.y * abs(f1.x);
	if (quickAxisIsSeparating(v0, v1, v2, f, r)) 
	{
		//std::cout << "outing at a21" << std::endl;
		return false;
	}

	// a22
	f = glm::vec3(-f2.y, f2.x, 0);
	r = e.x * abs(f2.y) + e.y * abs(f2.x);
	if (quickAxisIsSeparating(v0, v1, v2, f, r)) 
	{
		//std::cout << "outing at a22" << std::endl;
		return false;
	}


	// test face normal axis separation
	if(rtfmax(rtfmax(v0.x, v1.x), v2.x) < -e.x || rtfmin(rtfmin(v0.x, v1.x), v2.x) > e.x)
	{
		//std::cout << " out at first x face normal " << std::endl;
		return false;
	}
	if(rtfmax(rtfmax(v0.y, v1.y), v2.y) < -e.y || rtfmin(rtfmin(v0.y, v1.y), v2.y) > e.y)
	{
		//std::cout << " out at y face normal " << std::endl;
		return false;
	}
	if(rtfmax(rtfmax(v0.z, v1.z), v2.z) < -e.z || rtfmin(rtfmin(v0.z, v1.z), v2.z) > e.z)
	{
		//std::cout << " out at z face normal " << std::endl;
		return false;
	}

	// check the aabb intersects the plane of the triangle
	Plane p;
	p.n = glm::normalize(glm::cross(t.B - t.A, t.C - t.B));
	p.d = dot(p.n, t.A);
	if (!AABBintersectsPlane(aabb, p))
	{
		//std::cout << " out at aabbintersectsplane " << std::endl;
		return false;
	}
	else
	{
		return true;
	}
}

std::ostream& operator<<(std::ostream& o, const AABB& b)
{
	glm::vec3 c = (b.max + b.min) * 0.5f; // aabb center
	return o << "AABB {\n"
		<< "\tcenter: " << c << "\n" \
		<< "\tmax: " << b.max << "\n" \
		<< "\tmin: " << b.min << "\n" \
		<< "}";
}

} /* raytracer */
