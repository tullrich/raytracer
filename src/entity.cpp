#ifndef _IMPORTER_H_
#define _IMPORTER_H_

#include "entity.h"
#include "resources.h"

namespace raytracer {


Entity::Entity() : position(0), scale(0), name("") {}

void Entity::addMeshComponent(mesh_data::mesh_ptr mesh)
{
	if (mesh)
	{
		meshes.push_back(mesh);
		mesh->AABBContainMesh(aabb);

		//std::cout << "adding mesh updating aabb " << aabb << std::endl;
	}
	else
	{
		std::cout << "Warning: adding null mesh component" << std::endl;
	}
}


bool Entity::closestIntersection(const Ray &r, TraceResult &result) const
{
	TraceResult temp_result;
	float temp_t= -1;

	for (mesh_data::mesh_ptr mesh : meshes)
	{
		if (mesh->closestIntersection(r, temp_result))
		{
			if(temp_t == -1 || temp_result.intersection.w < result.intersection.w)
			{
				result = temp_result;
				result.mesh = mesh;
				temp_t = result.intersection.w;
			}
		}
	}

	return temp_t != -1;
}



std::ostream& operator<<(std::ostream& o, const Entity& b)
{
	o << "Entity '" << b.name << "' {\n" \
		<< "\tposition: " << b.position << "\n" \
		<< "\tscale: " << b.scale << "\n" \
		<< "\taabb: " << b.aabb << "\n" \
		<< "\tmeshes: \n";

		for (mesh_data::mesh_ptr m : b.meshes)
		{
			o << "\t\t" << *m;
		}

		o << "\n}";

	return o;
}

} /* namespace raytracer */

#endif /* _IMPORTER_H_ */
