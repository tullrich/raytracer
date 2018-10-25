#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "scenegraph.h"
#include "octree.h"

namespace raytracer {

glm::vec3 TraceResult::biasedIntersectionPoint() const
{
	return adjustFloatingPointToward(p->barycentricToPoint(intersection), p->surfaceNormal());
}


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

void SceneGraph::addEntity(Entity::entity_ptr entity)
{
	for(mesh_data::mesh_ptr mesh : entity->meshes)
	{
		bool hasUVs = mesh->getUVCount() != 0;
		for (int i = 0; i < mesh->numFaces; i++)
		{
			const prim_tri *indices = &(mesh->faces[i]);
			Triangle t(mesh->verts[indices->x], mesh->verts[indices->y], mesh->verts[indices->z]);
			if(hasUVs)
			{
				UVTriangle uv(mesh->uvs[indices->x], mesh->uvs[indices->y], mesh->uvs[indices->z]);
				addPrimitive(new UVTrianglePrimitive(t, uv, mesh->mat));
			}
			else
			{
				addPrimitive(new TrianglePrimitive(t, mesh->mat));
			}
		}
	}

}

void ButeForceSceneGraphImp::addPrimitive(Primitive *p)
{
	primitives.push_back(p);
}


bool ButeForceSceneGraphImp::traceRay(const Ray &r, TraceResult &result) const
{
	glm::vec4 intersection;
	bool found_one = false;
	for(Primitive *p : primitives)
	{
		if(p->intersects(r, intersection ))
		{
			if (!found_one || intersection.w < result.intersection.w)
			{
				found_one = true;
				result.intersection = intersection;
				result.p = p;
			}
		}
	}

	result.nodesTraversed = 1;
	result.primitiveIntersections = (int)primitives.size();
	return found_one;
}

SceneGraph* SceneGraphFactory::getSceneGraph()
{
	// preset for now
	return new OctreeSceneGraphImp(18);
}


void SceneGraphInjector::visit(Entity::entity_ptr &entity) const
{
	RAYTRACER_ASSERT(scene != NULL);

	scene->addEntity(entity);
}

void SceneGraphInjector::visit(Light::light_ptr &light) const
{
	RAYTRACER_ASSERT(scene != NULL);

	scene->addLight(light);
}


} /* namespace raytracer */
