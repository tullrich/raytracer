#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <list>
#include "common.h"
#include "mesh.h"

namespace raytracer {

using namespace::glm;


class Entity
{
private:
	typedef std::list<mesh_data::mesh_ptr> mesh_list;
public:
	Entity();

	typedef std::shared_ptr<Entity> entity_ptr;

	/**
	 * Set the name of this entity
	 * @param name the new name
	 */
	void setName(const std::string& name) { this->name = name; };

	/**
	 * Add a mesh to this entity
	 * @param mesh the mesh component to be added
	 */
	void addMeshComponent(mesh_data::mesh_ptr mesh);

	/**
	 * position of the entity in world space
	 */
	glm::vec3 position;

	/**
	 * scale of the entity in world space
	 */
	glm::vec3 scale;

	/**
	 * rotation of the entity in world space
	 */
	glm::quat rotation;


	/**
	 * Axial-aligned bounding box encompssing all meshes of this model
	 */
	AABB aabb;

	/**
	 * list of composite meshes making up this entity
	 */
	mesh_list meshes;

private:
	/**
	 * name of the mesh
	 */
	std::string name;

	/**
	 * access to private members
	 */
	friend std::ostream& operator<<(std::ostream& o, const Entity& b);
};

/**
 * Insert stream operator for Entity
 */
std::ostream& operator<<(std::ostream& o, const Entity& b);

/**
 * Helper class for visiting Entities
 */
typedef cgutils::Visitor<Entity::entity_ptr> EntityVisitor;

} /* namespace raytracer */

#endif /* _ENTITY_H_ */
