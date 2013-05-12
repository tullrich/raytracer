#ifndef _SCENE_GRAPH_H_
#define _SCENE_GRAPH_H_

#include "cgutils/import.h"
#include "entity.h"

namespace raytracer {

/**
 * Holds scene information and facilitates quick ray 
 */
class SceneGraph
{
public:
	SceneGraph() {};
	virtual ~SceneGraph() {};
protected:
	virtual void addObject() = 0;
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
