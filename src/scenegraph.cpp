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

bool SceneGraph::testVisibility(const Ray &r, TraceResult &result) const
{
	float d = glm::length(r.q - r.p);

	if (traceRay(r, result))
	{
		vec3 intersect_point = result.p->barycentricToPoint(result.intersection); 
		if (glm::length(intersect_point - r.p) < d)
		{
			return false;
		}
	}
	
	return true;
}

ButeForceSceneGraphImp::ButeForceSceneGraphImp()
{}

void ButeForceSceneGraphImp::addEntity(Entity::entity_ptr entity)
{
	entities.push_back(entity);

	for(mesh_data::mesh_ptr mesh : entity->meshes)
	{
		Primitive **fresh_primitives = mesh->allocatePrimitives();

		for (int i = 0; i < mesh->numFaces; i++)
		{
			primitives.push_back(fresh_primitives[i]);
		}

		delete[] fresh_primitives;
	}
}


bool ButeForceSceneGraphImp::traceRay(const Ray &r, TraceResult &result) const
{
	TraceResult temp_result;
	bool found_one = false;

	for(Primitive *p : primitives)
	{
		if(p->intersects(r, temp_result))
		{
			if (!found_one || temp_result.intersection.w < result.intersection.w)
			{
				found_one = true;
				result = temp_result;
			}
		}
	}

	return found_one;
}

SceneGraph* SceneGraphFactory::getSceneGraph()
{
	// preset for now
	return new OctreeSceneGraphImp(12);
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
