#include <assimp/Importer.hpp> 
#include <assimp/scene.h>
#include <assimp/postprocess.h> 

#include "scenegraph.h"


namespace raytracer {


OctreeSceneGraphImp::OctreeSceneGraphImp()
{

}

void OctreeSceneGraphImp::addEntity(Entity::entity_ptr entity)
{
	entities.push_back(entity);
}


void OctreeSceneGraphImp::addLight(Light::light_ptr light)
{
	lights.push_back(light);
}


bool OctreeSceneGraphImp::traceRay(Ray &r, glm::vec4 &intersection, Triangle &tri) const
{
	glm::vec4 temp_intersection;
	float temp_t= -1;
	Triangle temp_tri;

	for (Entity::entity_ptr entity : entities)
	{
		if(entity->closestIntersection(r, temp_intersection, temp_tri))
		{
			if(temp_t == -1 || temp_intersection.w < intersection.w)
			{
				intersection = temp_intersection;
				tri = temp_tri;
				temp_t = intersection.w;
			}
		}
	}

	return temp_t != -1;
}

SceneGraph* SceneGraphFactory::getSceneGraph()
{
	// preset for now
	return new OctreeSceneGraphImp();
}


void SceneGraphInjector::visit(Entity::entity_ptr &entity) const
{
	CGUTILS_ASSERT(scene != NULL)

	scene->addEntity(entity);
}

void SceneGraphInjector::visit(Light::light_ptr &light) const
{
	CGUTILS_ASSERT(scene != NULL)

	scene->addLight(light);
}


} /* namespace raytracer */
