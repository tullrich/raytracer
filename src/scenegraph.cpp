#include <assimp/Importer.hpp> 
#include <assimp/scene.h>
#include <assimp/postprocess.h> 

#include "scenegraph.h"


namespace raytracer {


SceneGraph::SceneGraph()
{
	builder = new SceneBuilder(this);
}

SceneGraph::~SceneGraph()
{
	delete builder;
}

const SceneBuilder * SceneGraph::getSceneBuilder()
{
	return builder;
}

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

const void SceneBuilder::visit(const mesh_data &mesh)
{
    std::cout << " inside visit() " << std::endl;
}

} /* namespace raytracer */
