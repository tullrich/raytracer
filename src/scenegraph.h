#ifndef _SCENE_GRAPH_H_
#define _SCENE_GRAPH_H_

#include "common.h"
#include "entity.h"
#include "light.h"
#include <list>

namespace raytracer {


typedef std::list<Light::light_ptr> LightList;

/**
 * Holds scene information and facilitates quick ray 
 */
class SceneGraph
{
public:
	SceneGraph() {};

	virtual ~SceneGraph() {};
	virtual void addEntity(Entity::entity_ptr entity) = 0;
	virtual void addLight(Light::light_ptr light) = 0;
	virtual bool traceRay(Ray &r, TraceResult &result) const = 0;

	bool testVisibility(const glm::vec3 &point1, const glm::vec3 &point2, float &distance) const;

	
	LightList lights;
};

/**
 * Octree scenegraph implementation
 */
class OctreeSceneGraphImp : public SceneGraph
{
public:
	OctreeSceneGraphImp();
	virtual ~OctreeSceneGraphImp() {};

	virtual bool traceRay(Ray &r, TraceResult &result) const;
	virtual void addLight(Light::light_ptr light);
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



class SceneGraphInjector : public EntityVisitor, public LightVisitor
{
public:
	SceneGraphInjector(SceneGraph *s) : scene(s) { };
	virtual void visit(Entity::entity_ptr &entity) const;
	virtual void visit(Light::light_ptr &light) const;

private:
	SceneGraph *scene;
};


} /* namespace raytracer */

#endif /* _SCENE_GRAPH_H_ */
