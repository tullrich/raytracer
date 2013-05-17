#ifndef _OCTREE_H_
#define _OCTREE_H_ 

#include <list>
#include "common.h"
#include "scenegraph.h"

namespace raytracer {

/**
 * Represents an axially-aligned spatial subdivision centered at center and extending 
 * halfWidth along axis.
 */
class OctNode
{
public:
	~OctNode();

	glm::vec3 center;
	float halfWidth;
	OctNode *child[8];
	EntityList entities;
};

/**
 *  Octree scenegraph implementation
 */
class OctreeSceneGraphImp : public SceneGraph
{
public:
	OctreeSceneGraphImp();
	virtual ~OctreeSceneGraphImp();

	/**
	 * Top down octree ray trace
	 * @param  r      ray to trace
	 * @param  result contains trace result upon success
	 * @return        true if this trace collided with something
	 */
	virtual bool traceRay(const Ray &r, TraceResult &result) const;

	/**
	 * Temporarily attaches all entities to the root until build() is
	 * next called
	 * @param entity entity to add to this scenegraph
	 */
	virtual void addEntity(Entity::entity_ptr entity);

	/**
	 * construct the octree tree, pushing entities from the root node
	 * down as far as possible
	 */
	virtual void build();
private:
	std::list<Entity::entity_ptr> allEntities; // list of all entities, not used for ray trace calculations

	OctNode *root;
};


} /* raytracer */

#endif /* _OCTREE_H_ */
