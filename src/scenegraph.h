#ifndef _SCENE_GRAPH_H_
#define _SCENE_GRAPH_H_

#include "cgutils/import.h"
#include "entity.h"

namespace raytracer {

using namespace::cgutils;

class SceneGraph; // forward declaration

class SceneBuilder : public AssetVisitor
{
public:
	SceneBuilder(SceneGraph *s) : scene(s) { };
	const void visit(const mesh_data &mesh);

private:
	SceneGraph *scene;
};

/**
 * Holds scene information and facilitates quick ray 
 */
class SceneGraph
{
public:
	SceneGraph();

	virtual const SceneBuilder* getSceneBuilder();

	virtual ~SceneGraph();
	virtual void addObject() = 0;

private:
	SceneBuilder *builder;
};

/**
 * Octree scenegraph implementation
 */
class OctreeSceneGraphImp : public SceneGraph
{
public:
	OctreeSceneGraphImp();
	virtual ~OctreeSceneGraphImp() {};
protected:
	virtual void addObject();
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


} /* namespace raytracer */

#endif /* _SCENE_GRAPH_H_ */
