#ifndef _OCTREE_H_
#define _OCTREE_H_ 

#include <list>
#include "common.h"
#include "scenegraph.h"
#include "primitive.h"

namespace raytracer {

enum Quadrant
{
	BSW = 0, // back-south-west
	FSW = 1, // front-south-west
	BNW = 2, // back-north-west
	FNW = 3, // front-north-west
	BSE = 4,  // back-south-east
	FSE = 5, // front-south-east
	BNE = 6, // back-north-east
	FNE = 7 // front-north-east
};


/**
 * axially-aligned spatial subdivision centered at center and extending 
 * halfWidth along each axis.
 */
class OctNode
{
public:
	OctNode(const glm::vec3 &center, float halfWidth) : center(center), halfWidth(halfWidth), primitives(), child{NULL} {};
	OctNode() : center(0), halfWidth(0), primitives(), child{NULL} {};
	~OctNode();

	void append(Primitive *prim);
	bool isLeaf() const; 
	void pushDown();
	glm::vec3 centerForQuadrant(unsigned int );
	void allocateChildren();
	bool testPrimitives(const Ray &r, TraceResult &result);
	bool traceRay(const Ray &r, TraceResult &result, float tx0,float ty0, float tz0, float tx1, float ty1, float tz1, unsigned char a);

	/**
	 * Compute a bounding box containing all of the primitives attached to this node
	 * @return the aabb
	 */
	void aabb(AABB &aabb);

	void childAabb(AABB &aabb, unsigned int zone);
	void allocateChild(unsigned int zone);

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
	static long numCompares;

private:

	void buildOctree_r(OctNode *n, int stopDepth);
	void pushDown_r(OctNode *n, int stopDepth);
	float optimalRootWidth();

	std::list<Entity::entity_ptr> allEntities; // list of all entities, not used for ray trace calculations

	OctNode *root;

	int maxDepth;
	int numNodes;
};


} /* raytracer */

#endif /* _OCTREE_H_ */
