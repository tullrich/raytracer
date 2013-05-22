#ifndef _SCENE_GRAPH_H_
#define _SCENE_GRAPH_H_

#include "common.h"
#include "entity.h"
#include "light.h"
#include <list>

namespace raytracer {


typedef std::list<Light::light_ptr> LightList;
typedef std::list<Entity::entity_ptr> EntityList;

/**
 * Holds scene information and facilitates quick ray 
 */
class SceneGraph
{
public:
	SceneGraph() {};
	virtual ~SceneGraph() {};
	virtual void addEntity(Entity::entity_ptr entity) = 0;
	virtual void addLight(Light::light_ptr light);
	virtual bool traceRay(const Ray &r, TraceResult &result) const = 0;
	virtual void build() = 0;

	bool testVisibility(const Ray &r, TraceResult &result) const;

	
	LightList lights;
};

/**
 * Brute force scenegraph implementation
 */
class ButeForceSceneGraphImp : public SceneGraph
{
public:
	ButeForceSceneGraphImp();
	virtual ~ButeForceSceneGraphImp() {};

	virtual bool traceRay(const Ray &r, TraceResult &result) const;
	virtual void addEntity(Entity::entity_ptr entity);
	virtual void build() {};
private:
	EntityList entities;
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
