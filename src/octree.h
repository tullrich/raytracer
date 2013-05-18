#ifndef _OCTREE_H_
#define _OCTREE_H_ 

#include <list>
#include "common.h"
#include "scenegraph.h"
#include "primitive.h"

namespace raytracer {


/**
 * Stored octree primitive;
 */
struct OctPrimitive
{
	Triangle tri;
	Entity::entity_ptr entity;
	mesh_data::mesh_ptr mesh;
	OctPrimitive *next;
};

/**
 * Represents an axially-aligned spatial subdivision centered at center and extending 
 * halfWidth along axis.
 */
class OctNode
{
public:
	~OctNode();
	void append(Primitive *prim);

	glm::vec3 center;
	float halfWidth;
	OctNode *child[8];
	std::list<Primitive*> primitives;
};

/**
 *  Octree scenegraph implementation
 */
class OctreeSceneGraphImp : public SceneGraph
{
public:
	OctreeSceneGraphImp(int treeDepth);
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

	OctNode* buildOctree_r(glm::vec3 center,  float halfWidth, int stopDepth);

private:
	std::list<Entity::entity_ptr> allEntities; // list of all entities, not used for ray trace calculations

	OctNode *root;

	int maxDepth;
};


} /* raytracer */

#endif /* _OCTREE_H_ */
