#ifndef _OCTREE_H_
#define _OCTREE_H_ 

#include <list>
#include "common.h"
#include "scenegraph.h"
#include "primitive.h"

namespace raytracer {

enum Quadrant
{
	FNW, // front-north-west
	BNW, // back-north-west
	FNE, // front-north-east
	BNE, // back-north-east
	FSW, // front-south-west
	BSW, // back-south-west
	FSE, // front-south-east
	BSE  // back-south-east
};


/**
 * axially-aligned spatial subdivision centered at center and extending 
 * halfWidth along each axis.
 */
class OctNode
{
public:
	OctNode(const glm::vec3 &center, float halfWidth) : center(center), halfWidth(halfWidth) {};
	OctNode() : center(0), halfWidth(0) {};
	~OctNode();

	void append(Primitive *prim);
	bool isLeaf() const; 
	void pushDown();
	glm::vec3 centerForQuadrant(Quadrant quadrent);
	void allocateChildren();

	/**
	 * Compute a bounding box containing all of the primitives attached to this node
	 * @return the aabb
	 */
	AABB aabb();

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

private:

	void buildOctree_r(OctNode *n, int stopDepth);
	void pushDown_r(OctNode *n);


	std::list<Entity::entity_ptr> allEntities; // list of all entities, not used for ray trace calculations

	OctNode *root;

	int maxDepth;
	int numNodes;
};


} /* raytracer */

#endif /* _OCTREE_H_ */
