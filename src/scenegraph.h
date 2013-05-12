#ifndef _SCENE_GRAPH_H_
#define _SCENE_GRAPH_H_

#include "cgutils/import.h"

namespace raytracer {

class SceneGraph
{
public:
	SceneGraph() {};
	virtual ~SceneGraph() {};
protected:
	virtual void addObject() = 0;
};


class OctreeSceneGraphImp : public SceneGraph
{
public:
	OctreeSceneGraphImp();
	virtual ~OctreeSceneGraphImp() {};
protected:
	virtual void addObject();
};



} /* namespace raytracer */

#endif /* _SCENE_GRAPH_H_ */
