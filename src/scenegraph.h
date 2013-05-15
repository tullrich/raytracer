#ifndef _SCENE_GRAPH_H_
#define _SCENE_GRAPH_H_

#include "common.h"
#include "entity.h"
#include <list>

namespace raytracer {

/**
 * Holds scene information and facilitates quick ray 
 */
class SceneGraph
{
public:
	SceneGraph() {};

	virtual ~SceneGraph() {};
	virtual void addEntity(Entity::entity_ptr entity) = 0;
	virtual bool traceRay(Ray &r, glm::vec4 &intersection, Triangle &tri) const = 0;
};

/**
 * Octree scenegraph implementation
 */
class OctreeSceneGraphImp : public SceneGraph
{
public:
	OctreeSceneGraphImp();
	virtual ~OctreeSceneGraphImp() {};

	virtual bool traceRay(Ray &r, glm::vec4 &intersection, Triangle &tri) const;
	virtual void addEntity(Entity::entity_ptr entity);
private:
	std::list<Entity::entity_ptr> entities;
};

/**
 * SceneGraph implementation factory
 */
class SceneGraphFactory
{
public:
	/**
	 * SceneGraph implementation factory
	 */
	static SceneGraph* getSceneGraph();
};



class SceneGraphInjector : public EntityVisitor
{
public:
	SceneGraphInjector(SceneGraph *s) : scene(s) { };
	virtual void visit(Entity::entity_ptr &entity) const;

private:
	SceneGraph *scene;
};


} /* namespace raytracer */

#endif /* _SCENE_GRAPH_H_ */
