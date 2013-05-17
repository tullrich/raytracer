#include "octree.h"

namespace raytracer {

OctNode::~OctNode()
{
	for(int i = 0; i < 8; i++)
	{
		if (child[i])
		{
			delete child[i];
		}
	}
}

OctreeSceneGraphImp::OctreeSceneGraphImp()
{
	root = new OctNode(); 
}

OctreeSceneGraphImp::~OctreeSceneGraphImp()
{
	delete root;
}

void OctreeSceneGraphImp::build()
{

}

bool OctreeSceneGraphImp::traceRay(const Ray &r, TraceResult &result) const
{

}

void OctreeSceneGraphImp::addEntity(Entity::entity_ptr entity)
{
	allEntities.push_back(entity);
	root->entities.push_back(entity);
}

} /* namespace raytracer */
