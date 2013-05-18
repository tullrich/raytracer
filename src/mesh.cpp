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


bool mesh_data::closestIntersection(const Ray &r, TraceResult &result) const
{
	TraceResult temp_result;
	temp_result.material = mat;
	float t = -1;

	for (int i = 0; i < numFaces; ++i)
	{
		const prim_tri *indices = &faces[i];

		temp_result.tri = Triangle(verts[indices->x], verts[indices->y], verts[indices->z]);
		if (r.intersects(temp_result.tri, temp_result.intersection))
		{
			if (temp_result.intersection.w < result.intersection.w || t == -1)
			{
				result = temp_result;
				t = result.intersection.w;
			}
		}
	}
/*
	for (int i = 0; i < numFaces; ++i)
	{
		const prim_tri *indices = &faces[i];
		//std::cout << "x " << indices->x << " y " << indices->y << " z " << indices->z << std::endl;
		temp_result.tri = Triangle(verts[indices->x], verts[indices->z], verts[indices->y]);
		//std::cout << temp_tri << std::endl;
		if (r.intersects(temp_result.tri, temp_result.intersection))
		{
			if (temp_result.intersection.w < result.intersection.w || t == -1)
			{
				result = temp_result;
				t = result.intersection.w;
			}
		}
	}*/

	return t != -1;
}


std::ostream& operator<<(std::ostream& o, const mesh_data& b)
{
	return o << "Mesh '" << b.name << "' {\n" \
		<< "\tnumVertices: " << b.numVertices << "\n" \
		<< "\tnumFaces:" << b.numFaces << "\n" \
		<< "\tmaterial:" << *b.mat << "\n" \
		<< "}";
}

}; /* namespace raytracer */
