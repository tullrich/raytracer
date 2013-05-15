#include "mesh.h"
#include "aabb.h"
#include "material.h"

namespace raytracer {

mesh_data::~mesh_data()
{
	if(verts)
	{
		delete[] verts;
	}

	if(faces)
	{
		delete[] faces;
	}
}

void mesh_data::AABBContainMesh(AABB &aabb) const
{

	AABBContainVertices(aabb, numVertices, verts);
}

void mesh_data::setVertices(int count, const glm::vec3 *ptr)
{
	numVertices = count;
	verts       = ptr;
}

void mesh_data::setFaces(int count, const prim_tri *ptr)
{
	numFaces = count;
	faces    = ptr;
}


bool mesh_data::closestIntersection(const Ray &r, glm::vec4 &intersection, Triangle &tri) const
{
	glm::vec4 temp_intersection;
	float t = -1;

	for (int i = 0; i < numFaces; ++i)
	{
		const prim_tri *indices = &faces[i];
		//std::cout << "x " << indices->x << " y " << indices->y << " z " << indices->z << std::endl;
		Triangle temp_tri(verts[indices->x], verts[indices->y], verts[indices->z]);
		//std::cout << temp_tri << std::endl;
		if (r.intersects(temp_tri, temp_intersection))
		{
			if (temp_intersection.w < intersection.w || t == -1)
			{
				intersection = temp_intersection;
				tri = temp_tri;
				t = intersection.w;
			}
		}
	}

	return t != -1;
}


std::ostream& operator<<(std::ostream& o, const mesh_data& b)
{
	return o << "Mesh " << b.name << "' {\n" \
		<< "\tnumVertices: " << b.numVertices << "\n" \
		<< "\tnumFaces:" << b.numFaces << "\n" \
		<< "\tmaterial:" << *b.mat << "\n" \
		<< "}";
}

}; /* namespace raytracer */
