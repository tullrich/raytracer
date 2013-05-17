#include <assimp/Importer.hpp> 
#include <assimp/scene.h>
#include <assimp/postprocess.h> 

#include "scenegraph.h"
#include "octree.h"



namespace raytracer {

bool debugvar = false;

void SceneGraph::addLight(Light::light_ptr light)
{
	lights.push_back(light);
}


bool SceneGraph::testVisibility(const glm::vec3 &point1, const glm::vec3 &point2, float &distance) const
{
	Ray r(point1, point2);
	TraceResult result;
	float d = glm::length(point2 - point1);

	if (traceRay(r, result))
	{
		vec3 intersect_point = result.tri.intersectionToPoint(result.intersection); 
		if (glm::length(intersect_point - point1) < d)
		{

			//std::cout << " non light hit !"  << std::endl;
			return false;
		}
	}

	distance = d;
	return true;
}

ButeForceSceneGraphImp::ButeForceSceneGraphImp()
{}

void ButeForceSceneGraphImp::addEntity(Entity::entity_ptr entity)
{
	entities.push_back(entity);

	//std::cout << "scene size now " << entities.size() << std::endl;
	//std::cout << "\t adding " << *entity << std::endl;
}


bool ButeForceSceneGraphImp::traceRay(const Ray &r, TraceResult &result) const
{
	TraceResult temp_result;
	float temp_t= -1;

	for (Entity::entity_ptr entity : entities)
	{
		if(entity->closestIntersection(r, temp_result))
		{
			if(temp_t == -1 || temp_result.intersection.w < result.intersection.w)
			{
				result = temp_result;
				result.entity = entity;
				temp_t = result.intersection.w;
			}
		}
	}

	return temp_t != -1;
}

SceneGraph* SceneGraphFactory::getSceneGraph()
{
	// preset for now
	return new ButeForceSceneGraphImp();
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
