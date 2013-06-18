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
 * class containing the result of a success full ray cast
 */
class TraceResult
{
public:
	TraceResult() : intersection(0), p(NULL) {};
	TraceResult(const glm::vec4 &intersection, const Primitive *p);

	glm::vec3 biasedIntersectionPoint() const;

	glm::vec4 intersection; // barycentric coordinates representing a point on p->face
	const Primitive *p; // primitive of intersection
};

/**
 * Holds scene information and facilitates quick ray 
 */
class SceneGraph
{
public:
	SceneGraph() {};
	virtual ~SceneGraph() {};
	virtual void addEntity(Entity::entity_ptr entity);
	virtual void addLight(Light::light_ptr light);
	virtual bool traceRay(const Ray &r, TraceResult &result) const = 0;
	virtual void build() = 0;

	bool testVisibility(const Ray &r, TraceResult &result) const;

protected:
	virtual void addPrimitive(Primitive *p) = 0;
public:
	LightList lights;
};

/**
 * Brute force scenegraph implementation
 */
class ButeForceSceneGraphImp : public SceneGraph
{
public:
	ButeForceSceneGraphImp() {};
	virtual ~ButeForceSceneGraphImp() {};

	virtual bool traceRay(const Ray &r, TraceResult &result) const;
	virtual void build() {};
protected:
	virtual void addPrimitive(Primitive *p);
private:
	EntityList entities;
	std::list<Primitive*> primitives;
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
