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
