#include "scenegraph.h"

#include <assimp/Importer.hpp> 
#include <assimp/scene.h>
#include <assimp/postprocess.h> 


namespace raytracer {

OctreeSceneGraphImp::OctreeSceneGraphImp()
{

}

void OctreeSceneGraphImp::addObject()
{

}

SceneGraph* SceneGraphFactory::getSceneGraph()
{
	// preset for now
	return new OctreeSceneGraphImp();
}


} /* namespace raytracer */
